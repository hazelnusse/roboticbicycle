#include "peripheral_init.h"

// STM Peripheral Library Includes
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

// Sensor Register Includes
#include "itg3200_reg.h"
#include "adxl345_reg.h"
#include "hmc5843_reg.h"

// Private functions
static void configureRCC(void);
static void configureMotors(void);
static void configureXBee(void);
static void configureI2C1(void);
static void configureEncoders(void);
static void configureSPI2(void);
static void configureITG(void);
static void configureADXL(void);
static void configureHMC(void);

void MCPSystemInit(void)
{
  // 100 ms delay, to let everything "warm up"
  for (uint32_t j = 0; j < 7200000; ++j) {
  }
  configureRCC();      // turn on system clock to all used peripherals
  configureEncoders(); // Configure TIM3,TIM4,TIM5 and related GPIO Pins
  configureMotors();   // Configure TIM1 and related GPIO Pins
  configureXBee();     // Configure USART2 and related GPIO Pins
  configureI2C1();     // Configure I2C1 and related GPIO Pins
  configureSPI2();     // Configure SPI2 and related GPIO Pins
  configureITG();      // Configure Rate Gyroscope
  configureADXL();     // Configure Accelerometer
  configureHMC();      // Configure Magnetometer
  // 100 ms delay, to let everything "warm up"
  for (uint32_t j = 0; j < 7200000; ++j) {
  }
} // MCPSystemInit()


static void configureRCC(void)
{
  // Enable I2C1, USART2, TIM3, TIM4, TIM5
  RCC->APB1ENR |= (RCC_APB1Periph_I2C1
               |   RCC_APB1Periph_USART2
               |   RCC_APB1Periph_TIM3
               |   RCC_APB1Periph_TIM4
               |   RCC_APB1Periph_TIM5);

  // Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, TIM1, AFIO
  RCC->APB2ENR |= (RCC_APB2Periph_GPIOA
               |   RCC_APB2Periph_GPIOB
               |   RCC_APB2Periph_GPIOC
               |   RCC_APB2Periph_GPIOD
               |   RCC_APB2Periph_GPIOE
               |   RCC_APB2Periph_TIM1
               |   RCC_APB2Periph_AFIO);
}

static void configureMotors(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  // TIM1 GPIO Configuration
  // PE9, PE11, Alternate Function Push Pull, 2Mhz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  // TIM1 Remap
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

  // TIM1 PWM Configuration
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) 3;    // controls PWM period
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) 0x3FFF;  // 2^14 - 1
  TIM_TimeBaseStructure.TIM_ClockDivision = (uint16_t) 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  // Initially, set PWM to 0% Duty cycle
  TIM_OCStructInit(&TIM_OCInitStructure);

  // PWM1 Mode configuration: CH1, CH2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = (uint16_t) 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

  // Enable CH1 and CH2
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  // Enable Preload on OC1 and OC2
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

  // Enable Auto Reload and TIM1 Counter
  TIM1->CR1 |= (TIM_CR1_ARPE | TIM_CR1_CEN);

  // TIM1 Main Output Enable
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  // GPIOC Enable 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9
                             | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  // When this executes, GPIOC pins 8, 9, 11, and 12 all get pulled to ground.
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Fault input lines
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  // Steer Encoder Index input PD10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
} // configureMotors()

static void configureXBee(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure PC3 Pin as reset pin for XBee
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, GPIO_Pin_3); // Set the pin high

  // Enable USART2 GPIO Remap.
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

  // Delay (about 200ns) to let the XBee reset
  GPIO_ResetBits(GPIOC, GPIO_Pin_3); // Pull low to start reset
  for (int i = 0; i < 2000; ++i){}   // Delay a while
  GPIO_SetBits(GPIOC, GPIO_Pin_3);   // Push it back high

  // Enable USART
  USART_Cmd(USART2, ENABLE);

  // Send a break character to clear the line
  USART_SendBreak(USART2);
  while(!(USART2->SR & USART_FLAG_TC)) {}
  //while(!(USART2->SR & USART_FLAG_TXE)) {}
}

static void configureI2C1(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure PB6, PB7 to alternate function
  // - alternate function
  // - open drain mode
  // - 2MHz, rise time is controlled by external pull-ups, fall time is still <
  // 125ns when in 2MHz mode, and will have lower overshoot and "ringing"
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Populate 6 fields of I2C_InitStructure
  I2C_InitStructure.I2C_ClockSpeed = (uint32_t) 100000; // I2C clock frequency
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;            // I2C mode
  // Changing this actually changes the attainable clock frequencies
  // due to some internal I2C clock divide
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = (uint16_t) 0;   // Doesn't matter
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;         // Enable ACK on 9th clock cycle
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  // Write settings to I2C Registers
  I2C_Init(I2C1, &I2C_InitStructure);

  // Enable I2C1
  I2C_Cmd(I2C1, ENABLE);
} // configureI2C1()

static void configureEncoders(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // TIM3 GPIO Configuration
  // PA6, PA7, Input Floating, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PB0, PB1, Input Pull Down, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // TIM4 GPIO Configuration
  // PD12, PD13, Input Floating, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // PD14, PD15, Input Pull Down, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  // TIM4 Remap
  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

  // TIM5 GPIO Configuration
  // PA0, PA1, Input Floating, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PA2, PA3, Input Pull Down, 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Set the Auto Reload Value for all 3 encoder timers
  TIM3->ARR = TIM4->ARR = TIM5->ARR = (uint16_t) 0xFFFF;

  // Configure TIM3, TIM4, TIM5 in Encoder Count Up/Down mode
  TIM_EncoderInterfaceConfig(TIM3,
                             TIM_EncoderMode_TI12,
                             TIM_ICPolarity_BothEdge,
                             TIM_ICPolarity_BothEdge);
  TIM_EncoderInterfaceConfig(TIM4,
                             TIM_EncoderMode_TI12,
                             TIM_ICPolarity_BothEdge,
                             TIM_ICPolarity_BothEdge);
  TIM_EncoderInterfaceConfig(TIM5,
                             TIM_EncoderMode_TI12,
                             TIM_ICPolarity_BothEdge,
                             TIM_ICPolarity_BothEdge);

  // Enable digital filtering, see page 397 of RM0008
  // Inputs are sampled at 36.0 MHz, need 8 samples at hi/low to validate a
  // transition.  This translates to a .22 micro second delay.
  TIM3->CCMR1 |= 0x3030;
  TIM4->CCMR1 |= 0x3030;
  TIM5->CCMR1 |= 0x3030;

  // Clear the counters
  TIM3->CNT = TIM4->CNT = TIM5->CNT = (uint16_t) 0;

  // Enable all three counters
  TIM3->CR1 |= TIM_CR1_CEN;
  TIM4->CR1 |= TIM_CR1_CEN;
  TIM5->CR1 |= TIM_CR1_CEN;
} // configureTimers()

static void configureSPI2(void)
{

} // configureSPI2()

void configureITG(void)
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
  I2C1->DR = DLPF_42;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}

  // Configure INT_CFG
  // I2C1->DR = INT_ANYRD_2CLEAR | ACTL;
  // Latch interrupts until any register is read, enable data interrupts.
  // Default interrupt pin mode is push-pull, active high
  I2C1->DR = LATCH_INT_EN | INT_ANYRD_2CLEAR | RAW_RDY_EN;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}

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
} // configureITG())

void configureADXL(void)
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
} // configureADXL()

static void configureHMC(void)
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
} // configureHMC()
