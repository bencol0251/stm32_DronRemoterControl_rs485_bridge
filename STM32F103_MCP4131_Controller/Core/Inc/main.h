#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;

void Error_Handler(void);

#endif
