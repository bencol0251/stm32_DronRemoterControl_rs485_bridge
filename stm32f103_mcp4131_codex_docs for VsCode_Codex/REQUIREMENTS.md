# Firmware Requirements

## MCU

Target: STM32F103C8T6

## Clock

Target system clock: 72 MHz.

## RS485

| Item | Value |
|---|---|
| Peripheral | USART1 |
| Baudrate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Driver | MAX3485 |
| Direction pin | PB1 |

PB1 low = receive.  
PB1 high = transmit.

## SPI

| Item | Value |
|---|---|
| Peripheral | SPI1 |
| Type | Dedicated STM32 hardware SPI |
| Frequency | 1 MHz target |
| Mode | SPI Mode 0 |
| CPOL | 0 |
| CPHA | 0 |
| Data size | 8 bit |

## MCP4131 Chip Select Pins

| Device | CS Pin |
|---|---|
| U1 | PA6 |
| U2 | PA4 |
| U3 | PA3 |
| U4 | PA2 |
| U5 | PB0 |

CS is active low.

## DIP Switch Address

| Pin | Address Bit |
|---|---|
| PB12 | Bit 3 |
| PB13 | Bit 2 |
| PB14 | Bit 1 |
| PB15 | Bit 0 |

All DIP pins are input pull-up.  
DIP ON connects pin to GND.  
ON = 0, OFF = 1.

## Manual Keys

| Pin | Function |
|---|---|
| PA15 | Manual U5 REVERSE |
| PB3 | Manual U5 REVERSE |

Both pins are input pull-up.  
Connected to GND = active.  
Manual key result: U5 code = 23.

## Timeout

No valid packet for 3 seconds shall reset all MCP4131 devices to initial values.

## Timer Rule

Every time operation must be based on hardware timer events or timer counters.
