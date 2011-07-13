#include "stm32f10x.h"
#include "math.h"

int main(void)
{
  int i;
  double x, y, z;
  float m, n, o;
  x = 1.0;
  y = 2.0;
  m = 1.0;
  n = 2.0;
  while (1) {
    for (i = 0; i < 10000; ++i)
      z = sin(x*y);
    for (i = 0; i < 10000; ++i)
      o = m*n;
  }
}
