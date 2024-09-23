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
#include "hardware.h"

static void DMA1_TransmitComplete_Callback(void);
static void DMA1_ReceiveComplete_Callback(void);

typedef struct
{ 
    uint32_t outputs;
    uint32_t inputs;
    float temperature;
    float pressure;
    float humidity;
}host__data_exchange_t;

host__data_exchange_t to_host = 
{
    .outputs = 0xDEAD,
    .inputs = 0xBEAF,
    .temperature = -20.0f,
    .pressure = 100.3f,
    .humidity = 70.3f
};

host__data_exchange_t from_host;



int main(void) 
{
    SystemClock_Config();
    Configure_GPIO();
    Configure_USART();

    
    Configure_DMA_Receiver(
        &from_host, 
        sizeof(from_host),
        &DMA1_ReceiveComplete_Callback
    );
    

    Configure_DMA_Transmitter(
        &to_host, 
        sizeof(to_host), 
        &DMA1_TransmitComplete_Callback
    );
    
    
    while(1)
    {        
        StartTransmission();
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        for (uint32_t i = 0; i < 10000000; ++i) { /*delay*/ }
    }

}

/*************************** End of file ****************************/


static void DMA1_TransmitComplete_Callback(void)
{
    printf("transmission is completed\n");
    StopTransmission();




}

static void DMA1_ReceiveComplete_Callback(void)
{
    printf("reception is completed\n");
}