#include "sample.h"

int main(void)
{
  Sample a;
  int x, y, z;

  while(1)
  {
    x = a.magX_MSB();
    y = x + a.magX_LSB();
    z = x + y;
  }

  return 0;
}
