#include <cstdint>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "MCPtasks.h"

// System initialization
#include "peripheral_init.h"

// Data sample class
#include "sample.h"

int main(void)
{
  MCPSystemInit();

  int32_t status = xTaskCreate(writeUSART2SampleTask,
    (const signed char *) "Task0", 100, NULL, 1, NULL);

  if (status == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
    while(1) {};

  vTaskStartScheduler();
  while(1) {};
}
