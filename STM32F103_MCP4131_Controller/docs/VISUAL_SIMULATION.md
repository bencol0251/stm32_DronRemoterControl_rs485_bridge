# Visual Simulation

## Purpose

Create a visual simulation based on the mockup behavior.

## Location

```text
Simulation/visual/
```

## Recommended Type

HTML + JavaScript dashboard.

## Required UI Elements

- DIP switch PB12, PB13, PB14, PB15
- Decoded device address
- PC target address
- PC command selector
- Key1-Key5 selectors
- PA15 manual key button
- PB3 manual key button
- RS485 request frame display
- RS485 response frame display
- U1-U5 MCP4131 code display
- U1-U5 PBW equivalent display
- SPI update log
- Timer tick display
- Last valid packet age
- 3-second timeout reset indicator

## Required Visual Behavior

When a valid packet is sent:

1. Request frame appears.
2. Address is validated.
3. U1-U5 update immediately.
4. SPI log shows U1 → U2 → U3 → U4 → U5.
5. Timeout timer restarts.

When PA15 or PB3 is pressed:

1. Pin becomes LOW.
2. U5 changes to REVERSE.
3. U5 code becomes 23.

When no valid packet is received for 3 seconds:

1. Timeout warning appears.
2. U1-U5 return to initial values.
