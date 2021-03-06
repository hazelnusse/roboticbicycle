#include <cstdint>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#define BITBAND(addr,bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *) (addr))

struct steer_info {
  uint8_t i;
  uint8_t state;
  int16_t ticks;
};

steer_info data[20];
uint8_t i = 0;
EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;

void EXTI10_Config(void);
void EXTI15_10_IRQHandler(void);
void TIM3_Config();

int main()
{
  for (int i = 0; i < 20; ++i) {
    data[i].i = i;
    data[i].state = 0;
    data[i].ticks = 0;
  }
  TIM3_Config();
  EXTI10_Config();
  while (1) { }
}

void EXTI15_10_IRQHandler(void)
{
  if((EXTI->PR & EXTI_Line10) == SET)
  {
    data[i].state = MEM_ADDR(BITBAND(GPIOD->IDR,10));
    data[i].ticks = TIM3->CNT;
    ++i;
    // store state, store counter value, 
    // need to bit band read the GPIOD->IDR IDR10
    EXTI->PR = EXTI_Line10;  // Clear the bit
  }
}

void EXTI10_Config(void)
{
  /* Enable GPIOD clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  
  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI10 Line to PA.00 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10);

  /* Configure EXTI10 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Config(void)
{
  RCC->APB1ENR |= RCC_APB1Periph_TIM3;
  RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
  // TIM3 GPIO Configuration
  // PA6, PA7, Input Floating, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Set the Auto Reload Value for all 3 encoder timers
  TIM3->ARR = (uint16_t) 0xFFFF;

  // Configure TIM3
  TIM_EncoderInterfaceConfig(TIM3,
                             TIM_EncoderMode_TI12,
                             TIM_ICPolarity_BothEdge,
                             TIM_ICPolarity_BothEdge);

  // Enable digital filtering, see page 397 of RM0008
  // Inputs are sampled at 36.0 MHz, need 8 samples at hi/low to validate a
  // transition.  This translates to a .22 micro second delay.
  TIM3->CCMR1 |= 0x3030;

  // Clear the counters
  TIM3->CNT = (uint16_t) 0;

  // Enable all three counters
  TIM3->CR1 |= TIM_CR1_CEN;
} // configureTimers()
