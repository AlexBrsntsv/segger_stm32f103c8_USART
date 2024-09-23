/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HARDWARE_H
#define HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"






void SystemClock_Config(void);
void Configure_GPIO(void);
void Configure_USART(void);
void Configure_DMA_Transmitter(
    const void *pTxBuffer, 
    size_t txBufferSize, 
    void(*DMA1_TransmitComplete_UserCallback)(void)
);

void Configure_DMA_Receiver(
    void *pRxBuffer, 
    size_t rxBufferSize,
    void(*DMA1_ReceiveComplete_UserCallback)(void)
);

void StartTransmission(void);
void StartReception(void);

void StartTransfers(void);
void StopTransmission(void);







#endif