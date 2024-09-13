/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HARDWARE_H
#define HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"






void SystemClock_Config(void);
void Configure_GPIO(void);








#endif