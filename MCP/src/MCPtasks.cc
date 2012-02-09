#include <cstdint>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "MCPtasks.h"

// Sample and sensor includes
#include "sample.h"
#include "itg3200_reg.h"
#include "adxl345_reg.h"
#include "hmc5843_reg.h"

// STM Peripheral library includes
#include "stm32f10x_usart.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"

#include "sinwave.h"

void collectData(Sample * data);
void ITG_ReadTXYZ(Sample * data);
void ADXL_ReadXYZ(Sample * data);
void HMC_ReadXYZ(Sample * data);

void writeUSART2SampleTask(void *params)
{
  Sample data;
  uint32_t i = 0, j = 0;

  portTickType xLastWakeTime = xTaskGetTickCount();

  while (1) {
    taskENTER_CRITICAL();
    collectData(&data);

    for(j = 0; j < sizeof(data); ++j) {
      USART2->DR = data[j];
      while (!(USART2->SR & USART_FLAG_TC)) {}
    } // for j

    if (i < 512) {    // 0 <= i < 512
      GPIOC->BSRR = GPIO_Pin_8;    // Positive rotation
      TIM1->CCR1 = sin_half_period_512[i++];
    } else if (i < 1024) {  // 512 <= i < 1024
      GPIOC->BRR = GPIO_Pin_8;     // Negative rotation
      TIM1->CCR1 = sin_half_period_512[i++ - 512];
    } else {
      GPIOC->BSRR = GPIO_Pin_8;    // Positive rotation
      i = 0;                       // Wrap i around
      TIM1->CCR1 = sin_half_period_512[i++];
    }

    taskEXIT_CRITICAL();

    vTaskDelayUntil(&xLastWakeTime,  5 / portTICK_RATE_MS);
  } // for i
}

void collectData(Sample * data)
{
  static uint32_t i = 0;
  static uint16_t rwPrev = 0, fwPrev = 0;
  static uint16_t rwCurrent = 0, fwCurrent = 0;

  ITG_ReadTXYZ(data);
  ADXL_ReadXYZ(data);
  // Main loop is running at 200 Hz, magnetometer generates data at 50 Hz
  if ((i % 4) == 0)
    HMC_ReadXYZ(data);

  data->Steer(TIM3->CNT);
  rwPrev = rwCurrent;   // Store the current measurement
  fwPrev = fwCurrent;   // Store the current measurement
  rwCurrent = TIM4->CNT;   // Update the current measurement
  fwCurrent = TIM5->CNT;   // Update the current measurement
  data->RearWheel((int16_t) (rwCurrent - rwPrev));
  data->FrontWheel((int16_t) (fwCurrent - fwPrev));
  data->Time(i++);
}

void ITG_ReadTXYZ(Sample * data)
{
  using namespace itg3200;
  
  // Enable the acknowledge bit
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ITG Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the ITG Read address
  I2C1->DR = TEMP_OUT_H;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ITG Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
  // Slave should start sending data now

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 7; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    (*data)[i] = (uint8_t) I2C1->DR;
  }
  
  // Disable the ACK and Generate a STOP at the end of the current transfer
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  (*data)[7] = (uint8_t) I2C1->DR;
}  // ITG_ReadTXYZ()

void ADXL_ReadXYZ(Sample * data)
{
  using namespace adxl345;

  // Enable the acknowledge bit
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ADXL Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the ADXL Register to Read
  I2C1->DR = DATAX0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ADXL Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
  // Slave should start sending data now

  // Need to check that master sends an ACK (low byte)
  for (int i = 8; i < 13; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    (*data)[i] = (uint8_t) I2C1->DR;
  }

  // Disable the ACK and Generate a STOP at the end of the current transfer
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  (*data)[13] = (uint8_t) I2C1->DR;
} // ADXL_ReadXYZ()

void HMC_ReadXYZ(Sample * data)
{
  using namespace hmc5843;

  // Enable the acknowledge bit
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Slave Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the HMC Register to be read
  I2C1->DR = DXRA;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
  // Slave should start sending data now

  // Need to check that master sends an ACK (low byte)
  for (int i = 14; i < 19; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    (*data)[i] = (uint8_t) I2C1->DR;
  }

  // Disable the ACK and Generate a STOP at the end of the current transfer
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  (*data)[19] = (uint8_t) I2C1->DR;
} // HMC_ReadXYZ()
