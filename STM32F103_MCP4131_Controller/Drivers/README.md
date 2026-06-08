# Drivers

This is an STM32 HAL-layer project. The application code is in `Core/`, and the ST HAL/CMSIS sources are taken from the STM32CubeF1 package named by the `STM32CUBE_F1_PATH` CMake variable.

The HAL driver files are not rewritten locally. CMake compiles the official ST files from:

Expected package contents:

```text
Drivers/CMSIS/
Drivers/STM32F1xx_HAL_Driver/
```
