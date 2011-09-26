#include <cstdint>
#include "FreeRTOS.h"

using namespace std;

int main(void)
{
  size_t sizes[] = {sizeof(signed char), sizeof(unsigned char),
			 sizeof(signed short), sizeof(unsigned short),
			 sizeof(signed int), sizeof(unsigned int),
			 sizeof(signed long), sizeof(unsigned long),
  			 sizeof(portTickType)};
  
  return 0;
}
