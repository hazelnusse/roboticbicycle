#include "stm32f10x.h"
#define STACK_TOP 0x20002000

// Forward declarations
int main(void);
void nmi_handler(void);
void hardfault_handler(void);
void delay(void);

// This is essentially the vector table.  The
// __attribute__((section("vectors")) ensures the linker puts the stack and the
// address to main in the first two memory address of flash, which is how the
// STM32 wants to boot.
// The compiler will autogenerate nmi_handler and hardfault_handler functions
// if we don't provide them, so lets provide them as void functions that take
// no parameters, do nothing, and return nothing.
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
  // For blinking STAT 1 led
  RCC->APB2ENR |= (1 << 4);  // Enable the GPIOC (bit 4)
  GPIOC->CRL = (1 << 24);  // Set GPIOC PC6 to output push-pull, max speed 10MHz

  // Enable Alternate function IO Clock, bit 0
  RCC->APB2ENR |= (1 << 0);  // can't write to AFIO.MAPR without this
  // Enable GPIOB, bit 3
  RCC->APB2ENR |= (1 << 3);
  // Enable TIM4, bit 2
  RCC->APB1ENR |= (1 << 2);

  // Configure PB6, PB7 as alternate function output push pull, 10Mhz
  GPIOB->CRL = 0x99000000;
  // Configure PB8, PB9 as alternate function output push pull, 10Mhz
  GPIOB->CRH = 0x00000099;

  // TIM1 Remap to PORT E
  // AFIO->MAPR = (0b11 << 6);  // Full remap of TIM1 to Port E pins

  TIM3->CNT = 0;  // Zero out the counter
  TIM3->CR1 = (0b111 << 5);  // Center-aligned mode 3
  TIM3->ARR = 0x00FF;  // Set the PWM Frequency
  TIM3->CCR1 = 0x000F; // Set duty cycle to 25%
  TIM3->CCMR1 = (1 << 3);  // Set OC1PE
  TIM3->EGR = 1; // Update generation bit
  TIM3->CCER = 1;

  // Now turn the timer on
  TIM3->CR1 |= TIM_CR1_CEN;


  while (1) {
    delay();                    // A short delay
    GPIOC->BSRR = 1 << 22;      // Clear PC6
    delay();                    // A short delay
    GPIOC->BSRR = 1 << 6;       // Set PC6
  } // while
} // main

void delay(void)
{
  // About 1/4 second delay
  for (int i = 0; i < 100000; ++i);
} // delay


void nmi_handler(void)
{
  return;
}

void hardfault_handler(void)
{
  return;
}
