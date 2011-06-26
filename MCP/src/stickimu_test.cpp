#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_i2c.h"

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PB6, PB7 to alternate function open drain mode, 50MHz */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void RCC_Configuration(void)
{
  /* I2C1 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void I2C_Configuration(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  // Disable I2C1 before changing any of the settings
  I2C_DeInit(I2C1);
  I2C_Cmd(I2C1, DISABLE);

  // Populate 6 fields of I2C_InitStructure
  I2C_InitStructure.I2C_ClockSpeed = 100000;  // 100kHz
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // I2C mode
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  // Doesn't matter
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;           // Doesn't matter
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;         // Enable ACK on 9th clock cycle
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  // Write settings to I2C Registers
  I2C_Init(I2C1, &I2C_InitStructure);

  // I2C Error interrupt mask
  // I2C_ITConfig(I2C1, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);

  // Enable I2C1
  //I2C_SoftwareResetCmd(I2C1, ENABLE);
  //I2C_SoftwareResetCmd(I2C1, DISABLE);
  I2C_Cmd(I2C1, ENABLE);
}

int main(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  I2C_Configuration();

  while (1) {
    // Wait for I2C bus to be free
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

    // START == Master pulls SDA Low then pulls SCL LOW
    I2C_GenerateSTART(I2C1, ENABLE);
    // Second START condition should reset I2C bus, shouldn't be needed
    //I2C_GenerateSTART(I2C1, ENABLE);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}
    I2C_Send7bitAddress(I2C1, 0x3C, I2C_Direction_Transmitter);
    // Getting stuck here -- probably due to Slave missing the address and not
    // sending the ACK.  Need to verify with logic analyser and see if sending
    // a stop condition will fix this.
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    I2C_SendData(I2C1, 0x02);  // Write to MODE register
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C1, 0x00);  // Set to continuous conversion mode
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

    // STOP == Master releases SCL, then releases SDA, pull up-resistors relied
    // upon to pull these lines high.
    I2C_GenerateSTOP(I2C1, ENABLE);
  }

  return 0;
}
