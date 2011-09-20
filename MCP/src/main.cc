//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"

#include <cstdint>
#include "stm32f10x_usart.h"
#include "peripheral_init.h"

//#include "MCPtasks.h"

#include "sample.h"


int main(void)
{
  MCPSystemInit();
  uint32_t ul;
  static Sample a;

  // populate Sample with some arbitrary data
  a.magX(0); a.magY(1); a.magZ(2);
  a.gyroT(3); a.gyroX(4); a.gyroY(5); a.gyroZ(6);
  a.accT(7); a.accX(8); a.accY(9); a.accZ(10);
  a.Time(11); a.Steer(12); a.RearWheel(13); a.FrontWheel(14);

  while (1) {

    for(ul = 0; ul < sizeof(a); ++ul) {
      USART2->DR = a[ul];
      while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) { }
    }

    // Delay a bit
    for(ul = 0; ul < 10000; ++ul) { }
  }

  /*
  int32_t status;
  status = xTaskCreate(dummyTask, (const signed char *) "Task0",
                       240, (void *) "Hello", 1, NULL);

  if (status == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
    while(1) { }
  }

  status = xTaskCreate(dummyTask, (const signed char *) "Task1",
                       240, (void *) "Howdy", 1, NULL);

  if (status == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
    while(1) { }
  }

  vTaskStartScheduler();

  while (1) { }
  */
  return 0;
}
