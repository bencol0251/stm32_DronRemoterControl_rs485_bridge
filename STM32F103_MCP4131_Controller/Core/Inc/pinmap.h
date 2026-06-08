#ifndef PINMAP_H
#define PINMAP_H

#include "stm32f1xx_hal.h"

#define PINMAP_MCP_U1_CS_PORT            GPIOA
#define PINMAP_MCP_U1_CS_PIN             GPIO_PIN_6
#define PINMAP_MCP_U2_CS_PORT            GPIOA
#define PINMAP_MCP_U2_CS_PIN             GPIO_PIN_4
#define PINMAP_MCP_U3_CS_PORT            GPIOA
#define PINMAP_MCP_U3_CS_PIN             GPIO_PIN_3
#define PINMAP_MCP_U4_CS_PORT            GPIOA
#define PINMAP_MCP_U4_CS_PIN             GPIO_PIN_2
#define PINMAP_MCP_U5_CS_PORT            GPIOB
#define PINMAP_MCP_U5_CS_PIN             GPIO_PIN_0

#define PINMAP_RS485_DE_RE_PORT          GPIOB
#define PINMAP_RS485_DE_RE_PIN           GPIO_PIN_1

#define PINMAP_MANUAL_PA15_PORT          GPIOA
#define PINMAP_MANUAL_PA15_PIN           GPIO_PIN_15
#define PINMAP_MANUAL_PB3_PORT           GPIOB
#define PINMAP_MANUAL_PB3_PIN            GPIO_PIN_3

#define PINMAP_DIP_B3_PORT               GPIOB
#define PINMAP_DIP_B3_PIN                GPIO_PIN_12
#define PINMAP_DIP_B2_PORT               GPIOB
#define PINMAP_DIP_B2_PIN                GPIO_PIN_13
#define PINMAP_DIP_B1_PORT               GPIOB
#define PINMAP_DIP_B1_PIN                GPIO_PIN_14
#define PINMAP_DIP_B0_PORT               GPIOB
#define PINMAP_DIP_B0_PIN                GPIO_PIN_15

#endif
