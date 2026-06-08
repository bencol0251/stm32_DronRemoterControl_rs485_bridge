# Manual Key Design

## Pins

| Pin | Function |
|---|---|
| PA15 | Manual U5 REVERSE |
| PB3 | Manual U5 REVERSE |

## Configuration

```text
GPIO_MODE_INPUT
GPIO_PULLUP
```

## Active State

LOW, connected to GND.

## Debounce

Use timer-driven 20 ms debounce.

Do not use `HAL_Delay()`.

## Behavior

If either key is debounced active:

```text
MCP4131 U5 = REVERSE
U5 code = 23
```
