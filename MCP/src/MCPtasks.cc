#include <cstdint>
#include "MCPtasks.h"
#include "sample.h"

#include "stm32f10x_usart.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"

void writeUSART2SampleTask(void *params)
{
  Sample *data = (Sample *) params;

	portTickType xLastWakeTime = xTaskGetTickCount();

  while (1) {  // task is always trying to write data
    data->Time(xLastWakeTime);
    for(volatile uint32_t i = 0; i < sizeof(Sample); ++i) {
      USART2->DR = (*data)[i];
			while (!(USART2->SR & USART_FLAG_TC)) {}
    } // for i
    vTaskDelayUntil(&xLastWakeTime,  5 / portTICK_RATE_MS);
  } // while
}
