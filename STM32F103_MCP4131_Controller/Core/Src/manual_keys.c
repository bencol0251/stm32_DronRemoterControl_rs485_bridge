#include "manual_keys.h"
#include "app_config.h"
#include "mcp4131.h"
#include "pinmap.h"
#include "timer_service.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool stable_active;
    bool last_sample_active;
    uint32_t changed_ms;
} ManualKey_t;

static ManualKey_t s_keys[2];
static bool s_reverse_active;

static bool ManualKeys_ReadActive(const ManualKey_t *key)
{
    return HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET;
}

void ManualKeys_Init(void)
{
    s_keys[0].port = PINMAP_MANUAL_PA15_PORT;
    s_keys[0].pin = PINMAP_MANUAL_PA15_PIN;
    s_keys[1].port = PINMAP_MANUAL_PB3_PORT;
    s_keys[1].pin = PINMAP_MANUAL_PB3_PIN;

    for (uint8_t i = 0u; i < 2u; i++)
    {
        s_keys[i].stable_active = false;
        s_keys[i].last_sample_active = ManualKeys_ReadActive(&s_keys[i]);
        s_keys[i].changed_ms = TimerService_Millis();
    }

    s_reverse_active = false;
}

void ManualKeys_Task(void)
{
    bool any_active = false;

    for (uint8_t i = 0u; i < 2u; i++)
    {
        bool sample = ManualKeys_ReadActive(&s_keys[i]);

        if (sample != s_keys[i].last_sample_active)
        {
            s_keys[i].last_sample_active = sample;
            s_keys[i].changed_ms = TimerService_Millis();
        }

        if (TimerService_Expired(s_keys[i].changed_ms, APP_MANUAL_KEY_DEBOUNCE_MS))
        {
            s_keys[i].stable_active = s_keys[i].last_sample_active;
        }

        any_active = any_active || s_keys[i].stable_active;
    }

    s_reverse_active = any_active;

    if (s_reverse_active && (MCP4131_GetCurrentCode(MCP_U5) != MCP4131_U5_REVERSE_CODE))
    {
        (void)MCP4131_WriteCode(MCP_U5, MCP4131_U5_REVERSE_CODE);
    }
}

bool ManualKeys_IsReverseActive(void)
{
    return s_reverse_active;
}
