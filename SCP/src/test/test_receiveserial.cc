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
void resetXBee(int fd);

pthread_mutex_t endTransmissionMutex=PTHREAD_MUTEX_INITIALIZER;

// Structure to hold data
struct serialinfo {
  std::queue<Sample> buf;   // Queue of Sample instances
  std::string filename;     // device filename
  int fd;                   // file descriptor
	bool endTransmission;     // End the transmission
};

// Function declarations
void setupReadPort(serialinfo * device);
void * PortRead(void * arg);

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " Device\n";
    std::cout << "Example: $ " << argv[0] << " /dev/ttyUSB0\n";
    return 0;
  }
	serialinfo usbport;
	usbport.filename = argv[1]; 		// Set the filename
	usbport.endTransmission = false;

  // Configure usbport for Reading Raw Data
	setupReadPort(&usbport);

  // Generate a read thread
  pthread_t data_receive_thread;
  if (pthread_create(&data_receive_thread, NULL, PortRead, &usbport)) {
    std::cout << "Error creating data read thread.\n";
    abort();
  }

  //sleep(5);
	//pthread_mutex_lock(&endTransmissionMutex);
	//usbport.endTransmission = true;
	//pthread_mutex_unlock(&endTransmissionMutex);

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
  serialinfo *port = (serialinfo *) arg;
	int SamplesRead = 0;
  static const int N = sizeof(Sample);
	timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 40000000;  // Delay 40 ms -- 25Hz

	while (1) {
		int bytes;
		ioctl(port->fd, FIONREAD, &bytes);  // see how many bytes are in the buffer
    if (bytes >= N) {    // we have at least one Sample's worth of bytes in the buffer
      int SamplesInBuffer = bytes / N;
      uint8_t * buf = new uint8_t[N * SamplesInBuffer];
      int n = read(port->fd, (void *) buf, N*SamplesInBuffer);

      if (n != N*SamplesInBuffer) {
        std::cout << bytes << "read() attempted to read " << N*SamplesInBuffer
                  << " bytes, but only obtained " << n << " bytes\n";
        abort();
      }
      // At this point we have copied exactly SamplesInBuffer Samples from the
      // serial port to buf
      for (int i = 0; i < SamplesInBuffer; ++i) {
        Sample temp;
        for (int j = 0; j < N; ++j) {
          temp[j] = buf[i*N + j];
        }
        port->buf.push(temp);
        printSample(temp);
      }
      SamplesRead += SamplesInBuffer;
      delete [] buf;
    }
    clock_nanosleep(CLOCK_REALTIME, 0, &sleeptime, NULL);
  }
	return NULL;
}

void setupReadPort(serialinfo * device)
{
  std::string response;
  termios serialConfig;  // termios structure for configuring serial port
  // Open the device in the following mode
  // Read/Write
  // don't become this process's controlling terminal,  
  // Reads will not block, see VTIME and VMIN below for details
	device->fd = open(device->filename.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (device->fd == -1) {
		std::cout << "Unable to open " << device->filename << '\n';
		exit(1);
	} else {
		std::cout << "Successfully opened " << device->filename << '\n';
	} // if

	// Get the current attributes of this port
	tcgetattr(device->fd, &serialConfig);

	// Set input and output baud rate
	cfsetispeed(&serialConfig, B115200);
	cfsetospeed(&serialConfig, B115200);

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
	tcsetattr(device->fd, TCSAFLUSH, &serialConfig);
  
  resetXBee(device->fd);

  tcflush(device->fd, TCIOFLUSH); // Flush input and output buffers

	std::cout << device->filename << " successfully configured.\n";
}

void printSample(Sample &s)
{
  static int i = 0;
  if (i == 0) {
    std::cout << "Gyro T,Gyro X,Gyro Y,Gyro Z,Acc X,Acc Y,Acc Z,Mag X,Mag Y,"
                 "Mag Z,Steer,Rear Wheel,Front Wheel,Time,Extra" << std::endl;
    i = 1;
  }
  std::cout << s.gyroT() << ","
            << s.gyroX() << ","
            << s.gyroY() << ","
            << s.gyroZ() << ","
            << s.accX() << ","
            << s.accY() << ","
            << s.accZ() << ","
            << s.magX() << ","
            << s.magY() << ","
            << s.magZ() << ","
            << s.Steer() << ","
            << s.RearWheel() << ","
            << s.FrontWheel() << ","
            << s.Time() << ","
            << s.Extra() << std::endl;
}

void resetXBee(int fd)
{
  char buf[10];
  // Steps:
  //  - issue '+++' to enter command mode
  //  - wait for 'OK\r'
  //  - issue 'ATFR\r' to reset the XBee module
  //  - wait for 'OK\r'
  //  - wait 1 second
  //  - return
  std::cout << "Issuing XBee reset ..." << std::endl;

  // Enter command mode
  // buf[0] = buf[1] = buf[2] = '+';
  int n = write(fd, "+++", 3);
  if (n == -1) {
    std::cout << "write() returned -1 when writing '+++' to XBee\n";
    abort();
  } else if (n != 3) {
    std::cout << "write() was unable to write '+++' to XBee\n";
    abort();
  }
  std::cout << "+++" << std::endl;

  // Wait until "OK\r" shows up 
  n = 0;
  while (n < 3) {
    ioctl(fd, FIONREAD, &n);
  }
  buf[n] = '\0';
  int m = read(fd, (void *) buf, n);
  if (m == -1) {
    std::cout << "read() returned -1 when reading XBee response.\n";
    abort();
  } else if (m != n) {
    std::cout << "read() tried to read " << n << "characters but only read " << m << '\n';
    abort();
  }
  std::cout << buf << std::endl;

  // Issue an XBee Reset
  n = write(fd, "ATFR\r", 5);
  if (n == -1) {
    std::cout << "write() returned -1 when writing 'ATFR\r' to XBee\n";
    abort();
  } else if (n != 5) {
    std::cout << "write() was unable to write 'ATFR\r' to XBee\n";
    abort();
  }
  std::cout << "ATFR" << std::endl;
  
  // Wait until "OK\r" shows up 
  n = 0;
  while (n < 3) {
    ioctl(fd, FIONREAD, &n);
  }
  buf[n] = '\0';
  m = read(fd, (void *) buf, n);
  if (m == -1) {
    std::cout << "read() returned -1 when reading XBee response.\n";
    abort();
  } else if (m != n) {
    std::cout << "read() tried to read " << n << "characters but only read " << m << '\n';
    abort();
  }
  std::cout << buf << std::endl;

  timespec sleeptime;
  sleeptime.tv_sec = 1;
  sleeptime.tv_nsec = 0;
  clock_nanosleep(CLOCK_REALTIME, 0, &sleeptime, NULL);
}
