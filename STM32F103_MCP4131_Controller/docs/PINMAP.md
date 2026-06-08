# Pin Map

| Pin | Function | Mode | Pull | Active | Note |
|---|---|---|---|---|---|
| PA5 | SPI1_SCK | AF Push-Pull | None | N/A | MCP4131 clock |
| PA7 | SPI1_MOSI | AF Push-Pull | None | N/A | MCP4131 data |
| PA6 | U1_CS | Output | None | Low | MCP U1 |
| PA4 | U2_CS | Output | None | Low | MCP U2 |
| PA3 | U3_CS | Output | None | Low | MCP U3 |
| PA2 | U4_CS | Output | None | Low | MCP U4 |
| PB0 | U5_CS | Output | None | Low | MCP U5 |
| PA9 | USART1_TX | AF Push-Pull | None | N/A | RS485 TX |
| PA10 | USART1_RX | Input | Pull-up | N/A | RS485 RX |
| PB1 | RS485_DE_RE | Output | None | High = TX | MAX3485 |
| PA15 | Manual Reverse | Input | Pull-up | Low | U5 REVERSE |
| PB3 | Manual Reverse | Input | Pull-up | Low | U5 REVERSE |
| PB12 | Address Bit3 | Input | Pull-up | Low = 0 | DIP ON = GND |
| PB13 | Address Bit2 | Input | Pull-up | Low = 0 | DIP ON = GND |
| PB14 | Address Bit1 | Input | Pull-up | Low = 0 | DIP ON = GND |
| PB15 | Address Bit0 | Input | Pull-up | Low = 0 | DIP ON = GND |

## PA15/PB3 JTAG Note

PA15 and PB3 are JTAG-related pins. Disable JTAG but keep SWD:

```c
__HAL_RCC_AFIO_CLK_ENABLE();
__HAL_AFIO_REMAP_SWJ_NOJTAG();
```
