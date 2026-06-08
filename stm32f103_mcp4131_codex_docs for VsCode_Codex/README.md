# STM32F103C8T6 MCP4131 HAL + Timer + Simulation Codex Package

Use these Markdown files as Codex instructions for generating a complete VS Code buildable STM32F103C8T6 HAL firmware project.

## Main Requirements

- STM32F103C8T6
- STM32 HAL format
- VS Code buildable project
- USART1 RS485, 115200 8N1
- MAX3485 DE/RE on PB1
- SPI1 hardware SPI for MCP4131
- SPI target frequency: 1 MHz
- Immediate U1 → U2 → U3 → U4 → U5 MCP4131 update after valid packet reception
- PB12/PB13/PB14/PB15 DIP-switch address input
- DIP inputs must use internal pull-up
- DIP switch ON connects pin to GND, so ON = 0
- PA15/PB3 active-low manual keys
- PA15/PB3 must use internal pull-up
- PA15/PB3 function is same as U5 REVERSE
- All time operations must use hardware timers
- Logic simulation required
- Visual simulation required
