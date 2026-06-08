# DIP Switch Address Design

## Pins

| Pin | Address Bit |
|---|---|
| PB12 | Bit 3 |
| PB13 | Bit 2 |
| PB14 | Bit 1 |
| PB15 | Bit 0 |

## GPIO

```text
Mode: GPIO_MODE_INPUT
Pull: GPIO_PULLUP
```

## Logic

| DIP State | Pin Level | Bit |
|---|---|---|
| OFF | High | 1 |
| ON | Low/GND | 0 |

## Function

```c
uint8_t DipSwitch_GetAddress(void)
{
    uint8_t b3 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET ? 1 : 0;
    uint8_t b2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET ? 1 : 0;
    uint8_t b1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET ? 1 : 0;
    uint8_t b0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET ? 1 : 0;

    return (b3 << 3) | (b2 << 2) | (b1 << 1) | b0;
}
```
