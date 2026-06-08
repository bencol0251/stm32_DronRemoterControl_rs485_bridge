#ifndef MCP4131_H
#define MCP4131_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

typedef enum
{
    MCP_U1 = 0,
    MCP_U2,
    MCP_U3,
    MCP_U4,
    MCP_U5,
    MCP_COUNT
} MCP4131_Device_t;

void MCP4131_Init(SPI_HandleTypeDef *spi);
HAL_StatusTypeDef MCP4131_WriteCode(MCP4131_Device_t dev, uint8_t code);
HAL_StatusTypeDef MCP4131_ResetAllInitial(void);
uint8_t MCP4131_GetCurrentCode(MCP4131_Device_t dev);
void MCP4131_ForceCachedCode(MCP4131_Device_t dev, uint8_t code);

#endif
