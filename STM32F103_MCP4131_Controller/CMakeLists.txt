cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
endif()
if(NOT CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
endif()
if(NOT CMAKE_OBJCOPY)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
endif()

project(STM32F103_MCP4131_Controller C ASM)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(STM32CUBE_F1_PATH "$ENV{STM32CUBE_F1_PATH}" CACHE PATH "Path to STM32CubeF1 package root")
if(NOT STM32CUBE_F1_PATH)
    message(FATAL_ERROR "Set STM32CUBE_F1_PATH to the STM32CubeF1 package root, for example C:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6")
endif()

set(HAL_DRIVER_DIR "${STM32CUBE_F1_PATH}/Drivers/STM32F1xx_HAL_Driver")
set(CMSIS_DIR "${STM32CUBE_F1_PATH}/Drivers/CMSIS")

add_executable(firmware.elf
    startup_stm32f103xb.s
    Core/Src/main.c
    Core/Src/stm32f1xx_it.c
    Core/Src/stm32f1xx_hal_msp.c
    Core/Src/app.c
    Core/Src/protocol.c
    Core/Src/rs485.c
    Core/Src/mcp4131.c
    Core/Src/dip_switch.c
    Core/Src/manual_keys.c
    Core/Src/timer_service.c
    Core/Src/key_mapping.c
    Core/Src/sim_hooks.c
    "${CMSIS_DIR}/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_cortex.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_gpio.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_dma.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_rcc.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_rcc_ex.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_flash.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_flash_ex.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_uart.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_spi.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_tim.c"
    "${HAL_DRIVER_DIR}/Src/stm32f1xx_hal_tim_ex.c"
)

target_include_directories(firmware.elf PRIVATE
    Core/Inc
    "${HAL_DRIVER_DIR}/Inc"
    "${HAL_DRIVER_DIR}/Inc/Legacy"
    "${CMSIS_DIR}/Device/ST/STM32F1xx/Include"
    "${CMSIS_DIR}/Include"
)

target_compile_definitions(firmware.elf PRIVATE
    STM32F103xB
    USE_HAL_DRIVER
)

target_compile_options(firmware.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_link_options(firmware.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/firmware.map
    --specs=nano.specs
    --specs=nosys.specs
)

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware.elf> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware.elf> firmware.bin
    BYPRODUCTS firmware.hex firmware.bin
)
