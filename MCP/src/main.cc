#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <cstdint>
#include "stm32f10x_usart.h"
#include "peripheral_init.h"

#include "MCPtasks.h"
#include "sample.h"

int main(void)
{
  MCPSystemInit();
  static Sample a; // [100], b[100];

  // populate Sample with some arbitrary data
  a.magX(0); a.magY(1); a.magZ(2);
  a.gyroT(3); a.gyroX(4); a.gyroY(5); a.gyroZ(6);
  a.accX(8); a.accY(9); a.accZ(10);
  a.Steer(11); a.RearWheel(12);// a.FrontWheel(13);
  a.Time(xTaskGetTickCount()); 

  int32_t status = xTaskCreate(writeUSART2SampleTask,
												        (const signed char *) "Task0",
																100, (void *) &a, 1, NULL);
  if (status  == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		while(1) {};

  vTaskStartScheduler();
	while(1) {};
}
