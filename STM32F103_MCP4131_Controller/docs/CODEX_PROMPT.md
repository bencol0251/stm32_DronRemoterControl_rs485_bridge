# Prompt to Paste into Codex

```text
Read every Markdown file in this folder.

Generate a complete STM32F103C8T6 HAL firmware project that can be built in VS Code.

Use STM32 HAL format.

Configure USART1 RS485, SPI1 MCP4131 at 1 MHz, MCP chip-select pins, PB12-PB15 active-low pull-up DIP address inputs, and PA15/PB3 active-low pull-up manual reverse keys.

Disable JTAG but keep SWD enabled.

All timing must use hardware timers. Do not use HAL_Delay or software delay loops.

Generate timer_service.c/h and use it for debounce, timeout, RS485 turnaround timing, and simulation timing.

Generate logic simulation under Simulation/logic.

Generate visual simulation under Simulation/visual.

Provide build instructions for firmware, logic simulation, and visual simulation.
```
