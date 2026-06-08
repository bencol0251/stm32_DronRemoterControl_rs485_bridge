#include "rs485.h"
#include "app_config.h"
#include "pinmap.h"

static UART_HandleTypeDef *s_uart;
static uint8_t s_rx_byte;
static uint8_t s_frame[APP_PROTOCOL_REQUEST_LEN];
static volatile uint8_t s_index;
static volatile bool s_frame_ready;

void RS485_Init(UART_HandleTypeDef *uart)
{
    s_uart = uart;
    s_index = 0u;
    s_frame_ready = false;
    HAL_GPIO_WritePin(PINMAP_RS485_DE_RE_PORT, PINMAP_RS485_DE_RE_PIN, GPIO_PIN_RESET);
    RS485_StartReceive();
}

void RS485_StartReceive(void)
{
    if (s_uart != 0)
    {
        (void)HAL_UART_Receive_IT(s_uart, &s_rx_byte, 1u);
    }
}

void RS485_OnRxByte(uint8_t byte)
{
    if ((s_index == 0u) && (byte != APP_PROTOCOL_REQUEST_HEADER))
    {
        return;
    }

    s_frame[s_index++] = byte;

    if (s_index >= APP_PROTOCOL_REQUEST_LEN)
    {
        s_index = 0u;
        s_frame_ready = true;
    }
}

bool RS485_Task(uint8_t *frame)
{
    if (!s_frame_ready)
    {
        return false;
    }

    __disable_irq();
    for (uint8_t i = 0u; i < APP_PROTOCOL_REQUEST_LEN; i++)
    {
        frame[i] = s_frame[i];
    }
    s_frame_ready = false;
    __enable_irq();

    return true;
}

HAL_StatusTypeDef RS485_Send(const uint8_t *data, uint8_t len)
{
    if (s_uart == 0)
    {
        return HAL_ERROR;
    }

    HAL_GPIO_WritePin(PINMAP_RS485_DE_RE_PORT, PINMAP_RS485_DE_RE_PIN, GPIO_PIN_SET);
    HAL_StatusTypeDef status = HAL_UART_Transmit(s_uart, (uint8_t *)data, len, APP_UART_TX_TIMEOUT_MS);
    HAL_GPIO_WritePin(PINMAP_RS485_DE_RE_PORT, PINMAP_RS485_DE_RE_PIN, GPIO_PIN_RESET);

    return status;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == s_uart)
    {
        RS485_OnRxByte(s_rx_byte);
        RS485_StartReceive();
    }
}
