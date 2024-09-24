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
#include <string.h>
#include <stdbool.h>



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
    .outputs = 0x000D,
    .inputs = 0xBEAF,
    .temperature = -20.0f,
    .pressure = 100.3f,
    .humidity = 70.3f
};

static void DMA1_TransmitComplete_Callback(void);
static void DMA1_ReceiveComplete_Callback(void);
static char* GetFormattedHostData(const host__data_exchange_t* data);
bool TransmittionIsComleted = true;



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
        GetFormattedHostData(&to_host), 
        strlen(GetFormattedHostData(&to_host)), 
        &DMA1_TransmitComplete_Callback
    );
    StartReception();
    
    while(1)
    {       
        while(!TransmittionIsComleted)
        {
        }
        
        GetFormattedHostData(&to_host);   
        TransmittionIsComleted = false;     
        StartTransmission();
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        for (uint32_t i = 0; i < 2000000; ++i) { /*delay*/ }
    }

}

/*************************** End of file ****************************/


static void DMA1_TransmitComplete_Callback(void)
{
    TransmittionIsComleted = true;
    StopTransmission();
}

static void DMA1_ReceiveComplete_Callback(void)
{
    printf("reception is completed\n");
}


static char* GetFormattedHostData(const host__data_exchange_t* data)
{
    static char out_buffer[160];
    sprintf(out_buffer, 
      "{ outputs = %#06X, inputs = %#06X, temperature = %07.2f, pressure = %07.2f, humidity = %07.2f }\n",      
      data->outputs,
      data->inputs,
      data->temperature,
      data->pressure,
      data->humidity
    );
    return out_buffer;
}