#include "hardware.h"



void (*DMA1_TransmitComplete_Callback) (void) = NULL;
void (*DMA1_ReceiveComplete_Callback) (void) = NULL;

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
  // LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}


void Configure_GPIO(void)
{
  /* Enable the LED2 Clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

void Configure_USART(void)
{

  /* (1) Enable GPIO clock and configures the USART pins **********************/

  /* Enable the peripheral clock of GPIO Port */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /* Enable USART2 peripheral clock and clock source *****************/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
  

  /* (2) Configure USART2 functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USART2);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
  // LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);

  /* Set Baudrate to 115200 using APB frequency set to 36000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
  
      In this example, Peripheral Clock is expected to be equal to 36000000 Hz => equal to SystemCoreClock/2
  */
  LL_USART_SetBaudRate(USART2, SystemCoreClock/2, 115200); 

  /* (3) Enable USART2 **********************************************************/
  LL_USART_Enable(USART2);
}





void Configure_DMA_Transmitter(
    const void *pTxBuffer, 
    size_t txBufferSize, 
    void(*DMA1_TransmitComplete_UserCallback)(void)
)
{
    /* DMA1 used for USART2 Transmission
     */
    /* (1) Enable the clock of DMA1 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* (2) Configure NVIC for DMA transfer complete/error interrupts */
    NVIC_SetPriority(DMA1_Channel7_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    /* (3) Configure the DMA functional parameters for transmission */
    LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_7, 
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                          LL_DMA_PRIORITY_HIGH              | 
                          //LL_DMA_MODE_NORMAL                | 
                          LL_DMA_MODE_CIRCULAR              |
                          LL_DMA_PERIPH_NOINCREMENT         | 
                          LL_DMA_MEMORY_INCREMENT           | 
                          LL_DMA_PDATAALIGN_BYTE            | 
                          LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7,
                           (uint32_t)pTxBuffer,
                           LL_USART_DMA_GetRegAddr(USART2),
                           LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7));
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, txBufferSize);
    DMA1_TransmitComplete_Callback = DMA1_TransmitComplete_UserCallback;
    /* (5) Enable DMA transfer complete/error interrupts  */
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);
}



void Configure_DMA_Receiver(
    void *pRxBuffer, 
    size_t rxBufferSize,
    void(*DMA1_ReceiveComplete_UserCallback)(void)
)
{
    /* DMA1 used for USART2 Reception
     */
    /* (1) Enable the clock of DMA1 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* (2) Configure NVIC for DMA transfer complete/error interrupts */
    NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel6_IRQn);

    /* (4) Configure the DMA functional parameters for reception */
    LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_6, 
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY | 
                          LL_DMA_PRIORITY_HIGH              | 
                          //LL_DMA_MODE_NORMAL                | 
                          LL_DMA_MODE_CIRCULAR              |
                          LL_DMA_PERIPH_NOINCREMENT         | 
                          LL_DMA_MEMORY_INCREMENT           | 
                          LL_DMA_PDATAALIGN_BYTE            | 
                          LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_6,
                           LL_USART_DMA_GetRegAddr(USART2),
                           (uint32_t)pRxBuffer,
                           LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6));
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, rxBufferSize);

    /* (5) Enable DMA transfer complete/error interrupts  */
    DMA1_ReceiveComplete_Callback = DMA1_ReceiveComplete_UserCallback;
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_6);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_6);
}

void StartTransmission(void)
{
    /* Enable DMA TX Interrupt */
    LL_USART_EnableDMAReq_TX(USART2);
      /* Enable DMA Channel Tx */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
}

void StopTransmission(void)
{
    /* Enable DMA TX Interrupt */
    LL_USART_DisableDMAReq_TX(USART2);
      /* Enable DMA Channel Tx */
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
}

void StartReception(void)
{
    /* Enable DMA RX Interrupt */
    LL_USART_EnableDMAReq_RX(USART2);
    /* Enable DMA Channel Rx */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
}

    


