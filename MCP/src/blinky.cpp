#include "stm32f10x.h"

#define STACK_TOP 0x20002000    /* This can move quite a lot ! */

void nmi_handler(void);
void hardfault_handler(void);
void delay(void);
int main(void);

unsigned int * myvectors[4]
  __attribute__ ((section("vectors"))) =
  {
    (unsigned int *)    STACK_TOP,
    (unsigned int *)    main,
    (unsigned int *)    nmi_handler,
    (unsigned int *)    hardfault_handler
  };


int main(void)
{
  RCC->APB2ENR |= 0x10;  // Enable the GPIOC (bit 4)
  GPIOC->CRL = (0x01 << 24);  // Set GPIOC PC6 to output push-pull, max speed 10MHz

  while (1) {
    delay();                    // A short delay
    GPIOC->BSRR = 1 << 22;      // Clear PC6
    delay();                    // A short delay
    GPIOC->BSRR = 1 << 6;       // Set PC6
    } // while
}

void delay(void)
{
  int i = 100000;   // About 1/4 second delay
  while (i-- > 0)
    asm("nop");     // This stops it optimising code out
}

void nmi_handler(void)
{
  return;
}

void hardfault_handler(void)
{
  return;
}

