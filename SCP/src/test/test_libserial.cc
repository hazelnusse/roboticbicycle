#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(void)
{
  //
  // Open the serial port.
  //
  using namespace LibSerial;
  SerialStream serial_port;
  const char *device = "/dev/ttyUSB0";
  serial_port.Open(device);
  if (serial_port.good())
  {
    std::cout << "SerialStream successfully opened on "
              << device << '\n';
  } else {
    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
              << "Error: Could not open " << device;
    exit(1);
  }

  //
  // Set the baud rate of the serial port.
  //
  serial_port.SetBaudRate(SerialStreamBuf::BAUD_115200);
  if (serial_port.good())
  {
    std::cout << "Baud rate: 11500 bps\n";
  } else {
    std::cerr << "Error: Could not set the baud rate.\n";
    exit(1) ;
  }

  //
  // Set the number of data bits.
  //
  serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
  if ( ! serial_port.good() ) 
  {
      std::cerr << "Error: Could not set the character size." << std::endl ;
      exit(1) ;
  }
  //
  // Disable parity.
  //
  serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
  if ( ! serial_port.good() ) 
  {
      std::cerr << "Error: Could not disable the parity." << std::endl ;
      exit(1) ;
  }
  //
  // Set the number of stop bits.
  //
  serial_port.SetNumOfStopBits( 1 ) ;
  if ( ! serial_port.good() ) 
  {
      std::cerr << "Error: Could not set the number of stop bits."
                << std::endl ;
      exit(1) ;
  }
  //
  // Turn off hardware flow control.
  //
  serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
  if ( ! serial_port.good() ) 
  {
      std::cerr << "Error: Could not use hardware flow control."
                << std::endl ;
      exit(1) ;
  }
  //
  // Do not skip whitespace characters while reading from the
  // serial port.
  //
  // serial_port.unsetf( std::ios_base::skipws ) ;
  //
  // Wait for some data to be available at the serial port.
  //
  //while( serial_port.rdbuf()->in_avail() == 0 ) 
  //{
  //    usleep(100) ;
  //}
  //
  // Keep reading data from serial port and print it to the screen.
  //
  //while( serial_port.rdbuf()->in_avail() > 0  ) 
  //{
  //    char next_byte;
  //    serial_port.get(next_byte);
  //    std::cerr << std::hex << static_cast<int>(next_byte) << " ";
  //    usleep(100) ;
  //} 

  std::vector<std::string> commands;
  serial_port.write("+++", 3);
  while (serial_port.rdbuf()->in_avail() == 0) {};
  while (serial_port.rdbuf()->in_avail() > 0)
  {
      char next_byte;
      serial_port.get(next_byte);
      std::cout << next_byte;
      usleep(10);
  }
  std::cout << '\n';
  return EXIT_SUCCESS ;
}
