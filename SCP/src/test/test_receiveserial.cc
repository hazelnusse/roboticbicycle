#include <iostream>
#include <queue>
#include <string>

// Serial port stuff
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// Thread support
#include <pthread.h>
#include <time.h>

// Standard C library stuff
#include <cstdint>  // for things like uint8_t, int16_t, etc.
#include <cstdlib>	// for atoi

// Sample
#include "sample.h"
void printSample(Sample &s);

pthread_mutex_t endTransmissionMutex=PTHREAD_MUTEX_INITIALIZER;

// Structure to hold data
struct serialinfo {
  std::queue<Sample> buf;   // Queue of Sample instances
  std::string filename;     // device filename
  int fd;                   // file descriptor
	unsigned int Fs;   		    // Data sample frequency
	unsigned int Fp;   				// Data plot update frequency
	bool endTransmission;     // End the transmission
};

// Function declarations
void setupReadPort(serialinfo * device);
void * PortRead(void * arg);

int main(int argc, char **argv)
{
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " Device SampleRate PlotRate\n";
    std::cout << "SampleRate is data rate of incoming Samples\n";
		std::cout << "FrameRate is plot rate\n";
    std::cout << "Example: $ " << argv[0] << " /dev/ttyUSB0 200 25\n";
    return 0;
  }
	serialinfo usbport;
	usbport.filename = argv[1]; 		// Set the filename
	usbport.Fs = atoi(argv[2]); // Set the sample frequency
	usbport.Fp = atoi(argv[3]); // Set the plot update frequency
	usbport.endTransmission = false;

	if (usbport.Fs % usbport.Fp) {
		std::cout << "SampleRate must be an integer multiple of PlotRate\n";
		return 0;
	}
	if ((usbport.Fp <= 0) | (usbport.Fs < usbport.Fp)) {
		std::cout << "Rate's must be non-negative and SampleRate > PlotRate\n";
		return 0;
	}

  // Configure usbport for Reading Raw Data
	setupReadPort(&usbport);

  // Generate a read thread
  pthread_t data_receive_thread;
  if (pthread_create(&data_receive_thread, NULL, PortRead, &usbport)) {
    std::cout << "Error creating data read thread.\n";
    abort();
  }

  sleep(10);
	pthread_mutex_lock(&endTransmissionMutex);
	usbport.endTransmission = true;
	pthread_mutex_unlock(&endTransmissionMutex);

	void * status;
  if (pthread_join(data_receive_thread, &status)) {
    std::cout << "Error joining data read thread.\n";
  } else {
		std::cout << "Read thread joined after reading " << (size_t) status << " Samples.\n";
	}

  close(usbport.fd);
	return 0;
}

void * PortRead(void * arg)
{
  serialinfo &port = *((serialinfo *) arg);
	const unsigned int SamplesPerRead = port.Fs / port.Fp;
	ssize_t M = SamplesPerRead * sizeof(Sample);
	uint8_t *buf = new uint8_t[M];
	size_t SamplesRead = 0;

	timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = (1000000000 / port.Fp);
	while (1) {
		ssize_t bytes;

		ioctl(port.fd, FIONREAD, &bytes);  // see how many bytes are in the buffer
		if (bytes < M) {  //  we didn't get all of the data
			pthread_mutex_lock(&endTransmissionMutex);
			if (port.endTransmission)
				pthread_exit((void *)SamplesRead);
			pthread_mutex_unlock(&endTransmissionMutex);
			clock_nanosleep(CLOCK_REALTIME, 0, &sleeptime, NULL);
			continue;
		}
		
		// Since we know there are M or more bytes in the buffer,
		// hopefully we can read all of them in one call to read.
		ssize_t n = read(port.fd, (void *) buf, M);
		if (n == -1) {   // Error occurred
			std::cout << "read() returned -1.\n";
			abort();
		} else if (n != M) {
			std::cout << "read() returned without retrieving " << M << "bytes\n";
			abort();
		}
		// If we get here, we've read M bytes
		SamplesRead += SamplesPerRead;
		for (unsigned int i = 0; i < SamplesPerRead; ++i) {
			// Create a temporary Sample instance and populated it with the data in buf
			Sample temp;
			for (unsigned int j = 0; j < sizeof(Sample); ++j)
				temp[j] = buf[i*sizeof(Sample) + j];
			port.buf.push(temp);		// push it into the Queue
			printSample(temp);
		}
	}
	return NULL;
}

void setupReadPort(serialinfo * device)
{
  termios serialConfig;  // termios structure for configuring serial port
	// Open the device in the following mode
	     // Read/Write
			 // don't become this process's controlling terminal,  
			 // Reads will not  block, see VTIME and VMIN below for details
	device->fd = open(device->filename.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (device->fd == -1) {
		std::cout << "Unable to open " << device->filename << '\n';
		exit(1);
	} else {
		std::cout << "Successfully opened " << device->filename << '\n';
	} // if

	// Set input and output baud rate
	cfsetispeed(&serialConfig, B115200);
	cfsetospeed(&serialConfig, B115200);

	// Get the current attributes of this port
	tcgetattr(device->fd, &serialConfig);

  // Control modes (8 data bits, No parity bit, 1 stop bit)
  // Reset the following bits:
	// 		2 stop bit
	// 		character size bits
	// 		RTS/CTS bit
	// 		parity enable bit
	serialConfig.c_cflag &= ~(CSTOPB | CSIZE | CRTSCTS | PARENB);
	// Set the following bits
	//    Don't become owner of port, i.e. Ignore modem control lines bit
	//    Enable receiver
	//    8 data bits
	serialConfig.c_cflag |= CLOCAL | CREAD | CS8;

  // Local modes  (Noncanonical "raw" data mode, no echoing, no signals)
  // Reset the following bits
	//    Generate signal on special characters bit
	//    Canonical Mode bit
	//    Echo bit
	serialConfig.c_lflag &= ~(ISIG | ICANON | ECHO);

  // Input modes  (Disable software flow control))
  // Reset the following bits,
	//    Flow control on output bit
	//    Flow control on input bit
	//    Allow any character to start flow again
	serialConfig.c_iflag &= ~(IXON | IXOFF | IXANY);

  // Output modes (No postprocessing of data)
	// Reset the following bits
	//    Postprocess output
	serialConfig.c_oflag &= ~OPOST;// Raw output, all other flags in oflag ignored

	// This make read block until we get Fs/Fp*sizeof(Sample) bytes)
	// open() must be called without O_NDELAY
	// instead of blocking, we can use clock_nanosleep() to put the thread to sleep for an appropriate amount of time.
	// serialConfig.c_cc[VMIN] = device->Fs / device->Fp * sizeof(Sample);
	// serialConfig.c_cc[VTIME] = 10;

  // Flush input and output buffers and make changes
	tcsetattr(device->fd, TCSAFLUSH, &serialConfig);         // Set outputs

	std::cout << device->filename << " successfully configured.\n";

  //fcntl(device[0].fd, F_SETOWN, getpid());  // Tell the OS we own this file descriptor
  //if (fcntl(device[0].fd, F_SETOWN, getpid()) == -1) {
  //  std::cout << "Unable to set the owner PID\n";
  //};  // Set the FASYNC bit
  //int oflags = fcntl(device[0].fd, F_GETFL);    // Get the existing flags
  //if (fcntl(device[0].fd, F_SETFL, oflags | FASYNC) == -1) {
  //  std::cout << "Unable to set the FASYNC bit\n";
  //};  // Set the FASYNC bit
  // fcntl(device[0].fd, F_SETFL, ~FNONBLOCK);  // Device 0 (read) should block
}


void printSample(Sample &s)
{
	std::cout << "Sample Received: {";
	for (size_t i = 0; i < sizeof(Sample); ++i)
		std::cout << (int) s[i] << ", ";
	std::cout << "}\n";
}
