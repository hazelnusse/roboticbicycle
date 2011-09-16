#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void GPIOSetup(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PC6 to output pushpull mode, 10MHz */
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  //GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PD12, PD13 (TIM4 CH1) to alternate function input floating
   * 50MHz */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
}

void RCC_Configuration(void)
{
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOC and GPIOD clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
}

void TIM4Setup(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  // TIM_TimeBaseInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0x3FFF;  // 2^14 - 1
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_EncoderInterfaceConfig(TIM4,
                             TIM_EncoderMode_TI12,
                             TIM_ICPolarity_BothEdge,
                             TIM_ICPolarity_BothEdge);
  // Enable digital filtering, see page 397 of RM0008
  TIM4->CCMR1 |= 0x2020;
}

int main(void)
{
  RCC_Configuration();
  GPIOSetup();
  TIM4Setup();
  TIM_Cmd(TIM4, ENABLE);
  while (1) {};
  return 0;
}
