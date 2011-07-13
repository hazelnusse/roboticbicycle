#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_i2c.h"
#include "HMC5843_REG.h"
#include "ITG3200_REG.h"

// extern void I2C1_EV_IRQHandler(void);
//void I2C1_ER_IRQHandler(void);
void GPIO_Configuration(void);
void HMC_Configuration(void);
void ITG_Configuration(void);
void HMC_Verify_Configuration(uint8_t * data);
void HMC_ReadXYZ(uint8_t * data);
void RCC_Configuration(void);
void I2C_Configuration(void);
void delay(int N);

int main(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  I2C_Configuration();

  // Setup HMC
  HMC_Configuration();
  // Setup ITG
  ITG_Configuration();

  // Read data
  uint8_t HMC5843_data[9];
  uint8_t ITG3200_data[8];
  while (1) {
    HMC_Verify_Configuration(HMC5843_data);
    // HMC_ReadXYZ((uint8_t *) data);
    delay(1000);
  }

  return 0;
}

void delay(int N)
{
  for (int i = 0; i < N; ++i)
    asm("nop");
}

void I2C_Configuration(void)
{
  // Uncomment to turn on interrupts
  // NVIC_EnableIRQ(I2C1_EV_IRQn);
  // NVIC_EnableIRQ(I2C1_ER_IRQn);
  I2C_InitTypeDef I2C_InitStructure;

  // Disable I2C1 before changing any of the settings
  // I2C_DeInit(I2C1);
  I2C_Cmd(I2C1, DISABLE);

  // Populate 6 fields of I2C_InitStructure
  I2C_InitStructure.I2C_ClockSpeed = 400000;        // I2C clock frequency
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;        // I2C mode
  // Changing this actually changes the attainable clock frequencies
  // due to some internal I2C clock divide
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;           // Doesn't matter
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;         // Enable ACK on 9th clock cycle
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  // Write settings to I2C Registers
  I2C_Init(I2C1, &I2C_InitStructure);

  // I2C Error interrupt mask
  I2C_ITConfig(I2C1, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);

  // Enable I2C1
  I2C_Cmd(I2C1, ENABLE);
}

void RCC_Configuration(void)
{
  // I2C1 clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  // GPIOB clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure PB6, PB7 to alternate function
  // - alternate function
  // - open drain mode
  // - 2MHz, rise time is controlled by external pull-ups, fall time is still <
  // 125ns when in 2MHz mode, and will have lower overshoot and "ringing"
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Set SCL and SDA, this should release both lines to be high
  // GPIOB->BSRR = 0x00C0;
}

// Writes to CRA, CRB, and MR to put the HMC5843L in the following mode
// -- 50 Hz Output Data Rate
// -- Normal measurement
// -- Gain 390 / +/- 4.5Ga sensitivity
// -- Continuous - Conversion mode
void HMC_Configuration(void)
{
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = HMC_ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of Configuration Register A
  I2C1->DR = HMC_CRA;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // 50 Hz, Normal Mode
  I2C1->DR = FIFTY_HZ | NORMAL_MODE;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Gain
  I2C1->DR = GAIN_390;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Gain
  I2C1->DR = MODE_CONTINUOUS; // MODE_SINGLE once we get FreeIMU
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Generate a I2C Stop condition (Master releases SDA while SCL is HIGH)
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void HMC_Verify_Configuration(uint8_t * data)
{
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = HMC_ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of Configuration Register A
  I2C1->DR = HMC_CRA;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Read address
  I2C1->DR = HMC_ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 8; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    data[i] = (uint8_t) I2C1->DR;
  }
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  data[8] = (uint8_t) I2C1->DR;
}

void HMC_ReadXYZ(uint8_t * data)
{
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Read address
  I2C1->DR = HMC_ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 5; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    data[i] = (uint8_t) I2C1->DR;
  }
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  data[5] = (uint8_t) I2C1->DR;
}


void ITG_Configuration(void)
{
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ITG Write address
  I2C1->DR = ITG_ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of SMPLRT_DIV
  I2C1->DR = ITG_SMPLRT_DIV;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Sample Rate Divider
  I2C1->DR = (uint8_t) 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Digital Low Pass Filter
  I2C1->DR = ITG_DLPF_188;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Generate a I2C Repeated start condition
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = ITG_ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of PWR_MGM
  I2C1->DR = ITG_PWR_MGM;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Select the X Gyro as the source for the PLL
  I2C1->DR = ITG_CLK_SEL_PLL_X_GYRO;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Generate a I2C Stop condition (Master releases SDA while SCL is HIGH)
  I2C_GenerateSTOP(I2C1, ENABLE);
}

extern "C" {
void I2C1_EV_IRQHandler(void)
{
  // volatile uint32_t flag = I2C_GetLastEvent(I2C1);
  while (1) {}
}
}
//void I2C1_ER_IRQHandler(void)
//{
//  volatile uint32_t flag = I2C_GetLastEvent(I2C1);
//  while (1) {}
//}
