# Project Description

## Overview

`STM32F103_MCP4131_Controller` is an STM32F103C8T6 firmware project for controlling five MCP4131 digital potentiometers from an RS485 command stream. It is intended for a remote-control bridge where an external master sends compact packets over a half-duplex RS485 bus, and the STM32 translates those key commands into SPI writes for five independent digital potentiometer channels.

The firmware is built on the STM32 HAL and keeps the application logic separated into small modules for protocol parsing, RS485 transport, MCP4131 control, DIP-switch addressing, manual input handling, timing, and simulation hooks. The main loop is simple: initialize the peripherals, start a 1 ms timer interrupt, initialize the application modules, then continuously run `App_Task()`.

## Main Purpose

The controller receives fixed-length RS485 packets, validates the packet header, destination address, command, and checksum, then updates the MCP4131 devices according to the decoded key values. It also replies to every complete packet with a fixed-length status response. If valid communication stops for more than 3 seconds, the firmware resets all digital potentiometers to their configured initial values.

The design supports multiple boards on the same RS485 bus. Each board reads a 4-bit DIP switch address and only accepts packets addressed to that local address.

## Target Hardware

The project targets an STM32F103C8T6 microcontroller running from an 8 MHz external crystal with the PLL configured for a 72 MHz system clock.

Hardware interfaces:

- MCU: STM32F103C8T6, Cortex-M3.
- System clock: HSE 8 MHz, PLL x9, 72 MHz SYSCLK.
- RS485 bus: USART1 at 115200 baud, 8 data bits, no parity, 1 stop bit.
- RS485 direction control: PB1, low for receive and high for transmit.
- SPI bus: SPI1 in master mode, mode 0, 8-bit transfers, software-controlled chip select.
- Digital potentiometers: five MCP4131 devices, named U1 through U5.
- Address selection: 4-bit DIP switch on PB12, PB13, PB14, and PB15.
- Manual reverse inputs: PA15 and PB3, active low.

PA15 and PB3 are normally related to JTAG pins on STM32F103 devices, so the firmware disables JTAG while keeping SWD enabled with `__HAL_AFIO_REMAP_SWJ_NOJTAG()`.

## MCP4131 Connections

The firmware drives all MCP4131 devices through SPI1 and uses separate GPIO chip-select outputs:

| Device | Chip select pin | Initial code |
|---|---:|---:|
| U1 | PA6 | 128 |
| U2 | PA4 | 128 |
| U3 | PA3 | 128 |
| U4 | PA2 | 128 |
| U5 | PB0 | 255 |

Each MCP4131 write sends two bytes over SPI: command byte `0x00` followed by the target wiper code. Chip select is pulled low before the SPI transfer and returned high afterward. When a write succeeds, the driver updates an internal cached copy of the current code for that device.

## RS485 Protocol

The request frame is 9 bytes:

```text
AA ADDR CMD KEY1 KEY2 KEY3 KEY4 KEY5 CHECKSUM
```

The response frame is 6 bytes:

```text
55 ADDR CMD STATUS COUNT CHECKSUM
```

The checksum is the unsigned 8-bit sum of all previous bytes in the frame. In other words, the checksum is calculated modulo 256 over every byte before the checksum field.

Supported commands:

| Command | Value | Description |
|---|---:|---|
| `SET_FIVE_KEYS` | `0x10` | Maps five key values to five MCP4131 wiper codes and writes U1 through U5. |
| `STOP_ALL_RESET` | `0x11` | Resets all five MCP4131 devices to their initial values. |
| `GET_STATUS` | `0x20` | Validates and acknowledges the packet without changing key-driven outputs. |

Response status values:

| Status | Value | Meaning |
|---|---:|---|
| `OK` | `0x00` | Packet accepted and command handled. |
| `CHECKSUM_ERROR` | `0x01` | Packet checksum did not match. |
| `ADDRESS_ERROR` | `0x02` | Packet was not addressed to this board. |
| `COMMAND_ERROR` | `0x03` | Header, length, or command was invalid. |
| `BUSY` | `0x04` | Reserved status for busy conditions. |
| `INTERNAL_ERROR` | `0x05` | Reserved status for internal failures. |

## Key Mapping

For `SET_FIVE_KEYS`, the five key bytes are mapped independently to MCP4131 codes.

U1:

| Key | MCP4131 code |
|---:|---:|
| 1 | 51 |
| 2 | 205 |
| Other | 128 |

U2:

| Key | MCP4131 code |
|---:|---:|
| 3 | 51 |
| 4 | 205 |
| Other | 128 |

U3:

| Key | MCP4131 code |
|---:|---:|
| 5 | 51 |
| 6 | 205 |
| Other | 128 |

U4:

| Key | MCP4131 code |
|---:|---:|
| 7 | 51 |
| 8 | 205 |
| Other | 128 |

U5:

| Key | MCP4131 code |
|---:|---:|
| 32 | 0 |
| 33 | 23 |
| 34 | 42 |
| 35 | 59 |
| 36 | 73 |
| 37 | 85 |
| 38 | 96 |
| 39 | 158 |
| 40 | 185 |
| 41 | 200 |
| 42 | 210 |
| 43 | 217 |
| Other | 255 |

## Runtime Behavior

At startup, the firmware initializes HAL, configures the 72 MHz clock tree, initializes GPIO, USART1, SPI1, and TIM2, then starts TIM2 in interrupt mode. TIM2 generates the 1 ms application timebase used by the debounce logic and packet timeout logic.

Application initialization performs these actions:

1. Resets the software millisecond timer.
2. Initializes DIP-switch address handling.
3. Initializes the MCP4131 driver and writes all initial values.
4. Initializes manual reverse key state.
5. Initializes RS485 receive mode and starts interrupt-driven byte reception.
6. Starts the valid-packet timeout timer.

During normal operation, `App_Task()`:

1. Polls and debounces the manual reverse inputs.
2. Checks whether a complete RS485 request frame has been received.
3. Parses and validates the request.
4. Executes the command when the request is valid.
5. Builds and transmits a response frame.
6. Resets all MCP4131 devices if no valid packet has been received for 3000 ms.

## Manual Reverse Inputs

PA15 and PB3 are active-low manual reverse inputs with a 20 ms debounce period. If either input becomes stably active, the firmware writes code `23` to U5. This manual action is independent of packet reception and lets the hardware force the U5 channel into the configured reverse value.

## Timeout Safety Behavior

The firmware tracks the time of the last valid RS485 packet. If no valid packet is received for `APP_PACKET_TIMEOUT_MS`, currently 3000 ms, all five MCP4131 devices are reset to their initial values. This reset happens once per timeout period and is armed again after the next valid packet.

## Source Layout

Important application files:

| File | Role |
|---|---|
| `Core/Src/main.c` | HAL startup, clock setup, peripheral initialization, main loop, and timer callback. |
| `Core/Src/app.c` | Top-level application orchestration and command handling. |
| `Core/Src/protocol.c` | Request validation, checksum calculation, command support checks, and response building. |
| `Core/Src/rs485.c` | Interrupt-driven byte reception, frame assembly, and half-duplex transmit control. |
| `Core/Src/mcp4131.c` | SPI writes, chip-select handling, initial reset, and cached wiper values. |
| `Core/Src/key_mapping.c` | Mapping from received key codes to MCP4131 wiper codes. |
| `Core/Src/manual_keys.c` | Active-low manual input debounce and manual U5 reverse action. |
| `Core/Src/dip_switch.c` | Runtime board address reading from the 4-bit DIP switch. |
| `Core/Src/timer_service.c` | 1 ms software timer and timeout helper functions. |
| `Core/Src/sim_hooks.c` | Hook points used by simulation builds or external test harnesses. |

Configuration and pin definitions are centralized in:

- `Core/Inc/app_config.h`
- `Core/Inc/pinmap.h`

## Build System

The project uses CMake with `arm-none-eabi-gcc`. The STM32 HAL and CMSIS sources are taken from an installed STM32CubeF1 package referenced by the `STM32CUBE_F1_PATH` environment variable.

Typical build commands:

```powershell
$env:STM32CUBE_F1_PATH="C:\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.6"
cmake -S . -B build -G "Ninja"
cmake --build build
```

Build outputs:

- `build/firmware.elf`
- `build/firmware.hex`
- `build/firmware.bin`
- `build/firmware.map`

The firmware can be flashed with STM32CubeProgrammer over SWD:

```powershell
STM32_Programmer_CLI -c port=SWD -w build/firmware.elf -v -rst
```

## Simulation Support

The repository includes simulation assets in the `Simulation` directory:

- `Simulation/logic` contains a CMake-based logic simulation entry point for exercising firmware behavior on a host machine.
- `Simulation/visual` contains a browser-based visual simulation with HTML, CSS, and JavaScript.

These simulations are useful for checking protocol behavior, key mapping, timeout handling, and high-level control flow before flashing real hardware.

## Design Characteristics

The firmware is intentionally small and deterministic. It uses fixed-size packets, fixed-size responses, static storage, and simple polling from the main loop. Interrupt work is kept minimal: USART1 receives one byte at a time and assembles frames, while TIM2 only increments the millisecond counter. Hardware-facing behavior is concentrated in HAL-based modules, which keeps the application logic readable and makes the protocol and mapping code easier to test or simulate.

