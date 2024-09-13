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

int main(void) 
{
    SystemClock_Config();
    Configure_GPIO();

    while(1)
    {
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        for (uint32_t i = 0; i < 1000000; ++i) { /*delay*/ }
    }

}

/*************************** End of file ****************************/


