#include <QtGui>
#include <QDir>
// Serial port stuff
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "xbeedialog.h"
#include "mainwindow.h"
#include "datareader.h"

XBeeDialog::XBeeDialog(QDataStream *stream, QWidget *par)
  : QDialog(par),
    xbStream(stream)
{
  fileNameLabel = new QLabel;

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(fileNameLabel);

  OpenButton = new QPushButton(tr("&Open"));
  OpenButton->setEnabled(true);
  OpenButton->setDefault(true);
  connect(OpenButton, SIGNAL(clicked()), this, SLOT(OpenClicked()));

  CloseButton = new QPushButton(tr("&Close"));
  CloseButton->setEnabled(false);
  connect(CloseButton, SIGNAL(clicked()), this, SLOT(CloseClicked()));

  ResetButton = new QPushButton(tr("&Reset"));
  ResetButton->setEnabled(false);
  connect(ResetButton, SIGNAL(clicked()), this, SLOT(ResetClicked()));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(OpenButton);
  buttonLayout->addWidget(CloseButton);
  buttonLayout->addWidget(ResetButton);

  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  setWindowTitle(tr("Open/Close/Reset XBee"));
  setFixedHeight(sizeHint().height());
}

void XBeeDialog::OpenClicked()
{
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setFilter(QDir::System | QDir::AllEntries | QDir::Hidden | QDir::Readable | QDir::Writable);
  dialog.setViewMode(QFileDialog::Detail);

  QStringList fileNames;
  if (dialog.exec())
    fileNames = dialog.selectedFiles();

  // Get the file associated with the data stream
  QFile *file = static_cast<QFile *>(xbStream->device());
  if (fileNames.size()) {
    // Close the file and disconnect the datastream if it is already open.
    MainWindow * par = static_cast<MainWindow *>(parent());
    if (file->isOpen()) {
      par->dataReader()->disconnectFromStream();
      par->timer()->stop();
      file->close();
    }

    // Set the filename and open the file
    file->setFileName(fileNames.at(0));
    if (file->open(QIODevice::ReadWrite)) {
      setupSerialPort();
      CloseButton->setEnabled(true);
      ResetButton->setEnabled(true);
      fileNameLabel->setText(file->fileName());
      // Turn on the Timer that checks for new samples
      par->dataReader()->connectToStream(xbStream);
      par->timer()->start();
    } else {
      QMessageBox msgBox;
      msgBox.setText(tr("Unable to open file: ").append(file->fileName()));
      msgBox.exec();
      file->setFileName("");
      par->dataReader()->disconnectFromStream();
      par->timer()->stop();
      CloseButton->setEnabled(false);
      ResetButton->setEnabled(false);
    }
  }
} // OpenClicked()

void XBeeDialog::ResetClicked()
{
  static_cast<MainWindow *>(parent())->timer()->stop();
  QFile *file = static_cast<QFile *>(xbStream->device());
  if (file->isOpen()) {
    file->flush();
    resetXBee();
  } else {
    std::cerr << "File is closed!!!???" << std::endl;
  }
  static_cast<MainWindow *>(parent())->timer()->start();
} // ResetClicked()

void XBeeDialog::CloseClicked()
{
  static_cast<MainWindow *>(parent())->timer()->stop();

  QFile *file = static_cast<QFile *>(xbStream->device());
  file->close(); 
  if (file->isOpen()) {
    std::cerr << "Unable to close file." << std::endl;
  } else {
    CloseButton->setEnabled(false);
    ResetButton->setEnabled(false);
    fileNameLabel->setText("");
  }
} // CloseClicked()

void XBeeDialog::setupSerialPort() const
{
  int fd;
  {
    QFile *file = static_cast<QFile *>(xbStream->device());
    fd = file->handle();
  }

  // Get the current attributes of this port
  termios serialConfig;  // termios structure for configuring serial port
  tcgetattr(fd, &serialConfig);

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

  // Flush input and output buffers and make changes
  tcsetattr(fd, TCSAFLUSH, &serialConfig);

  // Send the reset commands to the XBee
  resetXBee();

  tcflush(fd, TCIOFLUSH); // Flush input and output buffers
} // setupSerialPort()

void XBeeDialog::resetXBee() const
{
  int fd;
  {
    QFile *file = static_cast<QFile *>(xbStream->device());
    fd = file->handle();
  }

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
} // resetXBee()
