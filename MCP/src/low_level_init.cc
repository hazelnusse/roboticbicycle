#include "low_level_init.h"

void configureUSART2(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOD and AFIO
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);
  RCC->APB2ENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD;

  // Enable GPIO Remap.
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

  // Configure USART2 Tx Pin as alternate function push-pull
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Configure USART2 Rx Pin as input floating
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Configure USART2 with
  // 115200 baud, 8N1, no hardware flow control
  USART_InitStructure.USART_BaudRate = (uint32_t) 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, USART_InitStruct);

  // Enable USART
  USART_Cmd(USART2, ENABLE);
}


void peripheral_init(void)
{
  configureUSART2();

}
