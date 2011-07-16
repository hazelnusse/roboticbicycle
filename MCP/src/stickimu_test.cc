#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_i2c.h"
#include "hmc5843_reg.h"
#include "itg3200_reg.h"
#include "adxl345_reg.h"
/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)
/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

// extern void I2C1_EV_IRQHandler(void);
//void I2C1_ER_IRQHandler(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void I2C_Configuration(void);

void HMC_Configuration(void);
void HMC_Verify_Configuration(uint8_t * data);
void HMC_ReadXYZ(int16_t * data);

void ITG_Configuration(void);
void ITG_Verify_Configuration(uint8_t * data);
void ITG_ReadTXYZ(int16_t * data);

void ADXL_Configuration(void);
void ADXL_Verify_Configuration(uint8_t * data);
void ADXL_ReadXYZ(int16_t * data);

int main(void)
{
  static int i;
  RCC_Configuration();
  GPIO_Configuration();
  I2C_Configuration();

  // Setup HMC
  HMC_Configuration();
  // Setup ITG
  ITG_Configuration();
  // Setup ADXL
  ADXL_Configuration();

  // Read data
  int16_t HMC5843_XYZ[3];
  int16_t ITG3200_TXYZ[4];
  int16_t ADXL345_XYZ[3];
  float m_xyz[3];
  float g_txyz[4];
  float a_xyz[3];
  while (1) {
    // HMC_Verify_Configuration(HMC5843_data);
    // ITG_Verify_Configuration(ITG3200_data);
    HMC_ReadXYZ(HMC5843_XYZ);
    ITG_ReadTXYZ(ITG3200_TXYZ);
    ADXL_ReadXYZ(ADXL345_XYZ);
    g_txyz[0] = (ITG3200_TXYZ[0] + 13200) / 280.0f;
    for (i = 0; i < 3; ++i) {
      m_xyz[i] = HMC5843_XYZ[i] / 1620.0f;
      g_txyz[i+1] = ITG3200_TXYZ[i+1] / 14.375f;
      a_xyz[i] = ADXL345_XYZ[i] / 256.0f;
    }
  }

  return 0;
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
  I2C_InitStructure.I2C_ClockSpeed = 100000;        // I2C clock frequency
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;        // I2C mode
  // Changing this actually changes the attainable clock frequencies
  // due to some internal I2C clock divide
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  // I2C_InitStructure.I2C_OwnAddress1 = 0x00;           // Doesn't matter
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
  using namespace hmc5843;
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of Configuration Register A
  I2C1->DR = CRA;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // 50 Hz, Normal Mode
  I2C1->DR = FIFTY_HZ | NORMAL_MODE;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Gain
  I2C1->DR = GAIN_1620;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Gain
  I2C1->DR = MODE_CONTINUOUS; // MODE_SINGLE once we get FreeIMU
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Generate a I2C Stop condition (Master releases SDA while SCL is HIGH)
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void HMC_Verify_Configuration(uint8_t * data)
{
  using namespace hmc5843;
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of Configuration Register A
  I2C1->DR = CRA;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 8; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    data[i] = (uint8_t) I2C1->DR;
  }
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  data[8] = (uint8_t) I2C1->DR;

  I2C_GenerateSTOP(I2C1, ENABLE);
}

void HMC_ReadXYZ(int16_t * data)
{
  static uint8_t rawdata[6], i;
  using namespace hmc5843;
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

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}

  // Need to check that master sends an ACK (low byte)
  for (i = 0; i < 5; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    rawdata[i] = (uint8_t) I2C1->DR;
  }

  //I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C1->CR1 &= CR1_ACK_Reset;
  I2C1->CR1 |= CR1_STOP_Set;

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  rawdata[5] = (uint8_t) I2C1->DR;

  for (i = 0; i < 3; ++i) {
    data[i] = ((rawdata[2*i] << 8) | rawdata[2*i + 1]);
  } // for i

  // I2C_GenerateSTOP(I2C1, ENABLE);
}


void ITG_Configuration(void)
{
  using namespace itg3200;
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ITG Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of SMPLRT_DIV
  I2C1->DR = SMPLRT_DIV;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Sample Rate Divider
  I2C1->DR = (uint8_t) 0; // no sample rate division
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Digital Low Pass Filter
  I2C1->DR = DLPF_188;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Digital Low Pass Filter
  I2C1->DR = INT_ANYRD_2CLEAR | ACTL;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
  // Configure INT_CFG here....

  // Generate a I2C Repeated start condition
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of PWR_MGM
  I2C1->DR = PWR_MGM;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Select the X Gyro as the source for the PLL
  I2C1->DR = CLK_SEL_PLL_X_GYRO;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Generate a I2C Stop condition (Master releases SDA while SCL is HIGH)
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void ITG_Verify_Configuration(uint8_t * data)
{
  using namespace itg3200;
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the HMC Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of Sample Rate Divider
  I2C1->DR = SMPLRT_DIV;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Send a repeated start
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ITG Read address
  I2C1->DR = ADDR_R;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 11; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    data[i] = (uint8_t) I2C1->DR;
  }
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  data[11] = (uint8_t) I2C1->DR;
}

void ITG_ReadTXYZ(int16_t * data)
{
  static uint8_t rawdata[8], i;
  using namespace itg3200;
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

  // Need to check that master sends an ACK (low byte)
  for (i = 0; i < 7; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    rawdata[i] = (uint8_t) I2C1->DR;
  }
  //I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C1->CR1 &= CR1_ACK_Reset;
  I2C1->CR1 |= CR1_STOP_Set;

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  rawdata[7] = (uint8_t) I2C1->DR;

  for (i = 0; i < 4; ++i) {
    data[i] = ((rawdata[2*i] << 8) | rawdata[2*i + 1]);
  } // for i

  //I2C_GenerateSTOP(I2C1, ENABLE);
}

void ADXL_Configuration(void)
{
  using namespace adxl345;
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  // Wait for the I2C lines to be free
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {}

  // Generate a I2C Start condition (Master pulls SDA LOW while SCL is HIGH)
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

  // Send the ADXL Write address
  I2C1->DR = ADDR_W;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}

  // Send the address of SMPLRT_DIV
  I2C1->DR = BW_RATE;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Sate sample rate
  I2C1->DR = Rate_200_HZ;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Set Power Control
  I2C1->DR = Measure;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Configure INT_ENABLE, INT_MAP, INT_SOURCE here....
  // For now, just write all 0x00 to each register
  // INT_ENABLE
  I2C1->DR = 0x00;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
  // INT_MAP
  I2C1->DR = 0x00;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
  // INT_SOURCE
  I2C1->DR = 0x00;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Data Format and measurement range
  I2C1->DR = FULL_RES | Range_2g;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

  // Generate a I2C Stop condition (Master releases SDA while SCL is HIGH)
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void ADXL_Verify_Configuration(uint8_t * data)
{

}

void ADXL_ReadXYZ(int16_t * data)
{
  using namespace adxl345;
  static uint8_t rawdata[6];

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

  // Need to check that master sends an ACK (low byte)
  for (int i = 0; i < 5; ++i) {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    rawdata[i] = (uint8_t) I2C1->DR;
  }
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  rawdata[5] = (uint8_t) I2C1->DR;

  I2C_GenerateSTOP(I2C1, ENABLE);
  // The bytes are received LSB then MSB, we need to do some bit shifting to
  // properly fill our data array.
  for (int i = 0; i < 3; ++i) {
    data[i] = ((uint16_t) rawdata[2*i] ) | (((uint16_t)rawdata[2*i+1]) << 8);
  }
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
