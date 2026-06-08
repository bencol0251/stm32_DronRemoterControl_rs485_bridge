#include "dip_switch.h"
#include "pinmap.h"

void DipSwitch_Init(void)
{
}

uint8_t DipSwitch_GetAddress(void)
{
    uint8_t b3 = (HAL_GPIO_ReadPin(PINMAP_DIP_B3_PORT, PINMAP_DIP_B3_PIN) == GPIO_PIN_SET) ? 1u : 0u;
    uint8_t b2 = (HAL_GPIO_ReadPin(PINMAP_DIP_B2_PORT, PINMAP_DIP_B2_PIN) == GPIO_PIN_SET) ? 1u : 0u;
    uint8_t b1 = (HAL_GPIO_ReadPin(PINMAP_DIP_B1_PORT, PINMAP_DIP_B1_PIN) == GPIO_PIN_SET) ? 1u : 0u;
    uint8_t b0 = (HAL_GPIO_ReadPin(PINMAP_DIP_B0_PORT, PINMAP_DIP_B0_PIN) == GPIO_PIN_SET) ? 1u : 0u;

    return (uint8_t)((b3 << 3) | (b2 << 2) | (b1 << 1) | b0);
}
