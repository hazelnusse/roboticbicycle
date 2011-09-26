#include <iostream>
#include <string>
#include <vector>
#include <termios.h>
#include <fcntl.h>
#include <pthread.h>

pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
struct serialinfo {
  std::string buf;  // command
  std::string file; // filename
  int fd;           // file descriptor (used for open, close, read, write)
};

void setupPorts(serialinfo * device);
void * write_function(void *arg);
void * read_function(void *arg);


int main(int argc, char **argv)
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " ReadDevice WriteDevice\n";
    abort();
  }

  pthread_t thread[2];
  serialinfo device[2];
  device[0].file = argv[1];  // Read device
  device[1].file = argv[2];  // Write device

  setupPorts(device);

  device[1].buf = "hello";


  if (pthread_create(&thread[0], NULL, read_function, &(device[0]))) {
    std::cout << "Error creating serial read thread.\n";
    abort();
  }
  sleep(1);

  if (pthread_create(&thread[1], NULL, write_function, &(device[1]))) {
    std::cout << "Error creating serial write thread.\n";
    abort();
  }

  if (pthread_join(thread[1], NULL)) {
    std::cout << "Error joining write thread.\n";
    abort();
  }
  if (pthread_join(thread[0], NULL)) {
    std::cout << "Error joining read thread.\n";
    abort();
  }

  return 0;
}

void setupPorts(serialinfo * device)
{
  termios serialConfig;         // termios structure for configuring ports
  for (int i = 0; i < 2; ++i) {
    device[i].fd = open(device[i].file.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (device[i].fd == -1) {
      std::cout << "Unable to open " << device[i].file << '\n';
      abort();
    } else {
      std::cout << "Successfully opened " << device[i].file << '\n';
    } // if

    // Get the current attributes
    tcgetattr(device[i].fd, &serialConfig);

    // Set input and output speeds
    cfsetispeed(&serialConfig, B115200);
    cfsetospeed(&serialConfig, B115200);

    serialConfig.c_cflag |= (CLOCAL | CREAD);         // Don't become owner of port, enable receiver
    serialConfig.c_cflag &= ~PARENB;                  // Disable parity
    serialConfig.c_cflag &= ~CSTOPB;                  // 1 Stop Bit
    serialConfig.c_cflag &= ~CSIZE;                   // Mask the character size bits
    serialConfig.c_cflag |= CS8;                      // 8 data bits
    serialConfig.c_cflag &= ~CRTSCTS;                 // Disable hardware flow control
    serialConfig.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);   // Raw input
    serialConfig.c_iflag &= ~(IXON | IXOFF | IXANY);  // Disable Software Flow Control
    serialConfig.c_oflag &= ~OPOST;                   // Raw output

    // This should make read block until 5 bytes are read
    //if (i == 0) {
    //  serialConfig.c_cc[VMIN] = 5;
    //  serialConfig.c_cc[VTIME] = 0;
    //}
    tcsetattr(device[i].fd, TCSANOW, &serialConfig);         // Set outputs
    // tcsetattr(device[i].fd, TCSAFLUSH, &serialConfig);         // Set outputs
  }

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

void * write_function(void *arg)
{
  serialinfo * input = (serialinfo *) arg;

  ssize_t n = write(input->fd, input->buf.c_str(), input->buf.size());
  pthread_mutex_lock(&mymutex);
  std::cout << n << " bytes written" << '\n';
  pthread_mutex_unlock(&mymutex);

  return NULL;
}

void * read_function(void *arg)
{
  serialinfo * input = (serialinfo *) arg;
  char byte_read;
  ssize_t bytes_read = 0, n;

  while (bytes_read < 5) {
    n = read(input->fd, (void *) &byte_read, 1);
    if (n == 1) {
      input->buf += byte_read;
      ++bytes_read;
    }
  }

  return NULL;
}
