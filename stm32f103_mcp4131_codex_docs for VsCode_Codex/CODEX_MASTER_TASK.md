# Codex Master Task

Generate a complete STM32F103C8T6 HAL firmware project that can be built in VS Code.

## Project Structure

Create:

```text
STM32F103_MCP4131_Controller/
├── Core/
│   ├── Inc/
│   └── Src/
├── Drivers/
├── Simulation/
│   ├── logic/
│   └── visual/
├── Tests/
├── CMakeLists.txt or Makefile
├── startup_stm32f103xb.s
├── STM32F103C8Tx_FLASH.ld
└── README.md
```

## Firmware Rules

1. Use STM32 HAL.
2. Do not create a register-only project.
3. Make the project buildable in VS Code.
4. Use hardware timers for all time operations.
5. Do not use `HAL_Delay()` in application logic.
6. Do not use software delay loops.
7. Provide logic simulation.
8. Provide visual simulation.

## Required Firmware Behavior

- Read device address from PB12-PB15 DIP switch.
- Receive RS485 packet through USART1.
- Validate packet header, checksum, command, and address.
- If valid, immediately update five MCP4131 devices over SPI1.
- SPI1 shall be configured as hardware SPI near 1 MHz.
- Update order must be U1 → U2 → U3 → U4 → U5.
- PA15/PB3 grounded shall execute U5 REVERSE.
- If no valid packet is received for more than 3 seconds, restore all MCP4131s to initial values.
