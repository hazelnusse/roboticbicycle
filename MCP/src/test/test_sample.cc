#include "sample.h"

int main(void)
{
  Sample a;
  int x, y, z;

  while(1)
  {
    z = x*y;
    x = a.magX();
    y = x + a.magY();
    z = x + y;
  }

  return 0;
}
