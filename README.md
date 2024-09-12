# segger_stm32f103c8_template
segger embedded studio project template for stm32f103c8 (blue pill)

for correct work you should go to project options->build->Project Macros and create project macros: STM32CubeF1_DIR=Path to STM32CubeF1 folder
(ex.: STM32CubeF1_DIR=C:/stm32/STM32CubeF1)
Also you should set properties for project folder STM32F1xx_HAL_Driver which usually is STM32CubeF1\Drivers\STM32F1xx_HAL_Driver
Filter specification: *.c,*.h
Exclude specification: _htmresc;Legacy;*_template.c;*_template.h
Recurse into subdirectories - ON
