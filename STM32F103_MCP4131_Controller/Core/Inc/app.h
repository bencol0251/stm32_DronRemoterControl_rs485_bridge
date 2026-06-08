#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

void App_Init(UART_HandleTypeDef *uart, SPI_HandleTypeDef *spi);
void App_Task(void);
void App_UpdateFiveKeys(uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4, uint8_t k5);

#endif
