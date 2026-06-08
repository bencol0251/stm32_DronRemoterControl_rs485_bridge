# STM32F103 MCP4131 RS485 Controller

Complete STM32F103C8T6 STM32 HAL firmware project for controlling five MCP4131 digital potentiometers from an RS485 packet stream.

The firmware uses the HAL layer throughout the board-facing code:

- `HAL_Init()`, `HAL_RCC_OscConfig()`, and `HAL_RCC_ClockConfig()` for startup and clock setup.
- `MX_GPIO_Init()`, `HAL_GPIO_Init()`, `HAL_GPIO_ReadPin()`, and `HAL_GPIO_WritePin()` for GPIO.
- `HAL_UART_Init()`, `HAL_UART_Receive_IT()`, `HAL_UART_Transmit()`, and `HAL_UART_RxCpltCallback()` for RS485 over USART1.
- `HAL_SPI_Init()` and `HAL_SPI_Transmit()` for MCP4131 writes.
- `HAL_TIM_Base_Init()`, `HAL_TIM_Base_Start_IT()`, and `HAL_TIM_PeriodElapsedCallback()` for the 1 ms timer service.
- `HAL_UART_MspInit()`, `HAL_SPI_MspInit()`, and `HAL_TIM_Base_MspInit()` own peripheral clocks, GPIO alternate-function setup, and NVIC configuration.

## Hardware Summary

| Item | Configuration |
|---|---|
| MCU | STM32F103C8T6 |
| Clock | HSE 8 MHz, PLL x9, SYSCLK 72 MHz |
| RS485 | USART1, 115200 8N1 |
| RS485 direction | PB1, low RX, high TX |
| SPI | SPI1 hardware SPI, mode 0 |
| SPI target | 1.125 MHz with PCLK2 72 MHz and prescaler 64 |
| MCP CS pins | U1 PA6, U2 PA4, U3 PA3, U4 PA2, U5 PB0 |
| DIP address | PB12 bit3, PB13 bit2, PB14 bit1, PB15 bit0 |
| Manual reverse | PA15 and PB3, active low |

PA15 and PB3 require JTAG disabled while SWD remains enabled. The firmware calls `__HAL_AFIO_REMAP_SWJ_NOJTAG()`.

## Protocol

Request frame:

```text
AA ADDR CMD KEY1 KEY2 KEY3 KEY4 KEY5 CHECKSUM
```

Response frame:

```text
55 ADDR CMD STATUS COUNT CHECKSUM
```

Checksum is the byte sum modulo 256 over every byte before the checksum.

Commands:

| Code | Name |
|---|---|
| 0x10 | SET_FIVE_KEYS |
| 0x11 | STOP_ALL_RESET |
| 0x20 | GET_STATUS |

## Timing

All application timing is based on the TIM2 1 ms interrupt and `timer_service.c`.

- Manual key debounce: 20 ms.
- Packet timeout: 3000 ms.
- Timeout behavior: reset all MCP4131 devices to initial values.
- MCP update order after a valid packet: U1, U2, U3, U4, U5.

## Firmware Build

Install ARM GNU Toolchain and STM32CubeF1. Set `STM32CUBE_F1_PATH` to the Cube package root. The project CMake file compiles the ST HAL driver `.c` files directly from that package.

PowerShell example:

```powershell
$env:STM32CUBE_F1_PATH="C:\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.6"
cmake -S . -B build -G "Ninja"
cmake --build build
```

Outputs:

```text
build/firmware.elf
build/firmware.hex
build/firmware.bin
```

Flash with STM32CubeProgrammer:

```powershell
STM32_Programmer_CLI -c port=SWD -w build/firmware.elf -v -rst
```

## Logic Simulation

```powershell
cd Simulation/logic
cmake -S . -B build
cmake --build build
.\build\Debug\logic_sim.exe
```

With single-config generators:

```powershell
.\build\logic_sim.exe
```

## Visual Simulation

Open `Simulation/visual/index.html` directly in a browser, or serve it locally:

```powershell
cd Simulation/visual
python -m http.server 8080
```

Then open `http://localhost:8080`.
