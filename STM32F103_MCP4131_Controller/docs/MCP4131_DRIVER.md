# MCP4131 Driver

## API

```c
typedef enum
{
    MCP_U1 = 0,
    MCP_U2,
    MCP_U3,
    MCP_U4,
    MCP_U5,
    MCP_COUNT
} MCP4131_Device_t;

void MCP4131_Init(void);
HAL_StatusTypeDef MCP4131_WriteCode(MCP4131_Device_t dev, uint8_t code);
void MCP4131_ResetAllInitial(void);
uint8_t MCP4131_GetCurrentCode(MCP4131_Device_t dev);
```

## SPI Transaction

```text
CS LOW
Send command byte
Send data byte
CS HIGH
```

## Timing

At 1 MHz:

- 16 bits = 16 us per MCP4131.
- Five devices = 80 us plus overhead.
- Practical expected update time: less than 1 ms.
- Required update time: less than 100 ms.

## No Scheduled Slot Delay

Do not use 20 ms slots. Updates are back-to-back.
