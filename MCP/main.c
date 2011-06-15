#include "stm32f10x.h"

int main(void)
{
  int i, x;

  for (i = 0; i < 10; ++i) {
    x += i;
  } // for i

  return 0;
}
