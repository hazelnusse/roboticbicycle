// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// STM32 Library includes
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void configureUSART2();
void writebytesTask(void *params);

int main(void)
{
	configureUSART2();
  xTaskCreate(writebytesTask, (signed char *) "Task0", 100, (void *) 0, 1, NULL);
	vTaskStartScheduler();
	while(1){}
}

void writebytesTask(void *params)
{
	long i = (long) params;
  while (1) {
      USART2->DR = i;
      while (!(USART2->SR & USART_FLAG_TC)) {}
  }
}

void configureUSART2(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable USART2 (APB1)
  RCC->APB1ENR |= RCC_APB1Periph_USART2;
  // Enable GPIOD and AFIO  (APB2)
  RCC->APB2ENR |= (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD);

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
  USART_Init(USART2, &USART_InitStructure);

  // Enable USART
  USART_Cmd(USART2, ENABLE);
}
