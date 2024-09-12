/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include <RTT/SEGGER_RTT.h>
#include <string.h>


#include "stm32f1xx_hal.h"

uint8_t aTxBuffer[] = " **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling **** ";
uint8_t aRxBuffer[96];


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

UART_HandleTypeDef UartHandle;

int main(void) {
    HAL_Init();
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    

    GPIO_InitTypeDef GPIOC_InitStruct = 
    {
        GPIO_PIN_13,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(GPIOC, &GPIOC_InitStruct);

    GPIO_InitTypeDef GPIOA_InitStruct_UART_TX = 
    {
        GPIO_PIN_9,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH
    };

    GPIO_InitTypeDef GPIOA_InitStruct_UART_RX = 
    {
        GPIO_PIN_10,
        GPIO_MODE_AF_INPUT,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(GPIOA, &GPIOA_InitStruct_UART_TX);
    HAL_GPIO_Init(GPIOA, &GPIOA_InitStruct_UART_RX);
    
    UART_InitTypeDef UART1_InitStruct = 
    {
        .BaudRate = 115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = UART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE               
    };

    UartHandle.Instance = USART1;
    UartHandle.Init = UART1_InitStruct;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        while(1)
        {
          
        }
     
    }



    const char outBuf[] = "hello!\r\n";
 
    while (1)
    {
    /*
        if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, sizeof(aTxBuffer), 5000)!= HAL_OK)
        {             
        }
    */
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        SEGGER_RTT_Write(0, outBuf, strlen(outBuf));
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_Delay(1000);
    }

}

/*************************** End of file ****************************/


