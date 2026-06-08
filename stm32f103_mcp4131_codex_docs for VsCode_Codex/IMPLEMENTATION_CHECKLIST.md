# Implementation Checklist

## Project

- [ ] HAL project generated.
- [ ] VS Code build configured.
- [ ] Startup file present.
- [ ] Linker script present.
- [ ] CMake or Makefile present.

## Timer

- [ ] TIM2 1 ms tick configured.
- [ ] Timer service implemented.
- [ ] No HAL_Delay in application logic.
- [ ] Debounce uses timer.
- [ ] Timeout uses timer.
- [ ] Simulation uses timer model.

## GPIO

- [ ] MCP CS pins output high.
- [ ] PB1 RS485 direction output low.
- [ ] PB12-PB15 input pull-up.
- [ ] PA15/PB3 input pull-up.
- [ ] JTAG disabled, SWD enabled.

## Firmware

- [ ] Protocol parser implemented.
- [ ] Address filtering implemented.
- [ ] SPI1 MCP driver implemented.
- [ ] Immediate five-device update implemented.
- [ ] Manual reverse implemented.
- [ ] Timeout reset implemented.

## Simulation

- [ ] Logic simulation generated.
- [ ] Visual simulation generated.
- [ ] Logic simulation tests pass.
- [ ] Visual simulation shows all required states.
