#include "mcp4131.h"
#include "app_config.h"
#include "pinmap.h"
#include "sim_hooks.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} MCP4131_CsPin_t;

static SPI_HandleTypeDef *s_spi;
static uint8_t s_current[MCP_COUNT] =
{
    MCP4131_INITIAL_U1,
    MCP4131_INITIAL_U2,
    MCP4131_INITIAL_U3,
    MCP4131_INITIAL_U4,
    MCP4131_INITIAL_U5
};

static const uint8_t s_initial[MCP_COUNT] =
{
    MCP4131_INITIAL_U1,
    MCP4131_INITIAL_U2,
    MCP4131_INITIAL_U3,
    MCP4131_INITIAL_U4,
    MCP4131_INITIAL_U5
};

static const MCP4131_CsPin_t s_cs[MCP_COUNT] =
{
    { PINMAP_MCP_U1_CS_PORT, PINMAP_MCP_U1_CS_PIN },
    { PINMAP_MCP_U2_CS_PORT, PINMAP_MCP_U2_CS_PIN },
    { PINMAP_MCP_U3_CS_PORT, PINMAP_MCP_U3_CS_PIN },
    { PINMAP_MCP_U4_CS_PORT, PINMAP_MCP_U4_CS_PIN },
    { PINMAP_MCP_U5_CS_PORT, PINMAP_MCP_U5_CS_PIN }
};

static void MCP4131_SetCs(MCP4131_Device_t dev, GPIO_PinState state)
{
    HAL_GPIO_WritePin(s_cs[dev].port, s_cs[dev].pin, state);
}

void MCP4131_Init(SPI_HandleTypeDef *spi)
{
    s_spi = spi;

    for (uint8_t i = 0u; i < MCP_COUNT; i++)
    {
        MCP4131_SetCs((MCP4131_Device_t)i, GPIO_PIN_SET);
        s_current[i] = s_initial[i];
    }

    (void)MCP4131_ResetAllInitial();
}

HAL_StatusTypeDef MCP4131_WriteCode(MCP4131_Device_t dev, uint8_t code)
{
    if ((dev >= MCP_COUNT) || (s_spi == 0))
    {
        return HAL_ERROR;
    }

    uint8_t tx[2] = { 0x00u, code };

    MCP4131_SetCs(dev, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(s_spi, tx, sizeof(tx), APP_SPI_TIMEOUT_MS);
    MCP4131_SetCs(dev, GPIO_PIN_SET);

    if (status == HAL_OK)
    {
        s_current[dev] = code;
        SimHooks_OnSpiWrite((uint8_t)dev, code);
    }

    return status;
}

HAL_StatusTypeDef MCP4131_ResetAllInitial(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    for (uint8_t i = 0u; i < MCP_COUNT; i++)
    {
        if (MCP4131_WriteCode((MCP4131_Device_t)i, s_initial[i]) != HAL_OK)
        {
            status = HAL_ERROR;
        }
    }

    return status;
}

uint8_t MCP4131_GetCurrentCode(MCP4131_Device_t dev)
{
    if (dev >= MCP_COUNT)
    {
        return 0u;
    }

    return s_current[dev];
}

void MCP4131_ForceCachedCode(MCP4131_Device_t dev, uint8_t code)
{
    if (dev < MCP_COUNT)
    {
        s_current[dev] = code;
    }
}
