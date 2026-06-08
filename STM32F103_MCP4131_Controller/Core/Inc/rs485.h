#ifndef RS485_H
#define RS485_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"

void RS485_Init(UART_HandleTypeDef *uart);
bool RS485_Task(uint8_t *frame);
HAL_StatusTypeDef RS485_Send(const uint8_t *data, uint8_t len);
void RS485_OnRxByte(uint8_t byte);
void RS485_StartReceive(void);

#endif
