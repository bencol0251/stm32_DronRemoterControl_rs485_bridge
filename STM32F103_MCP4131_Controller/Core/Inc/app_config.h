#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>

#define APP_DEVICE_COUNT                 5u
#define APP_RS485_BAUDRATE               115200u
#define APP_PACKET_TIMEOUT_MS            3000u
#define APP_MANUAL_KEY_DEBOUNCE_MS       20u
#define APP_UART_RX_BUFFER_SIZE          9u
#define APP_UART_TX_TIMEOUT_MS           10u
#define APP_SPI_TIMEOUT_MS               10u

#define APP_PROTOCOL_REQUEST_LEN         9u
#define APP_PROTOCOL_RESPONSE_LEN        6u
#define APP_PROTOCOL_REQUEST_HEADER      0xAAu
#define APP_PROTOCOL_RESPONSE_HEADER     0x55u

#define MCP4131_INITIAL_U1               128u
#define MCP4131_INITIAL_U2               128u
#define MCP4131_INITIAL_U3               128u
#define MCP4131_INITIAL_U4               128u
#define MCP4131_INITIAL_U5               255u
#define MCP4131_U5_REVERSE_CODE          23u

#endif
