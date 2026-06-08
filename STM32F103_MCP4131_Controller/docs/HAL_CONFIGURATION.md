# HAL Configuration

## Clock

Preferred:

```text
HSE = 8 MHz
PLL = x9
SYSCLK = 72 MHz
HCLK = 72 MHz
PCLK1 = 36 MHz
PCLK2 = 72 MHz
```

## USART1

```c
huart1.Instance = USART1;
huart1.Init.BaudRate = 115200;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits = UART_STOPBITS_1;
huart1.Init.Parity = UART_PARITY_NONE;
huart1.Init.Mode = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;
```

## SPI1

```c
hspi1.Instance = SPI1;
hspi1.Init.Mode = SPI_MODE_MASTER;
hspi1.Init.Direction = SPI_DIRECTION_2LINES;
hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi1.Init.NSS = SPI_NSS_SOFT;
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
```

If SPI1 clock is 72 MHz, prescaler 64 gives 1.125 MHz. This is the nearest common HAL divider to 1 MHz. If exact 1 MHz is mandatory, configure clock tree accordingly and document it.

## GPIO Defaults

- MCP4131 CS pins default HIGH.
- PB1 RS485 direction default LOW.
- DIP pins input pull-up.
- PA15/PB3 input pull-up.
