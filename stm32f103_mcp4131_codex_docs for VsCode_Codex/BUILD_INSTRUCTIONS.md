# Build Instructions

## Required Tools

- VS Code
- ARM GNU Toolchain
- STM32CubeMX or STM32CubeIDE
- CMake or Make
- STM32CubeProgrammer
- ST-LINK

## CMake Build

```bash
cmake -S . -B build
cmake --build build
```

## Make Build

```bash
make -j
```

## Flash

```bash
STM32_Programmer_CLI -c port=SWD -w build/firmware.elf -v -rst
```

## Logic Simulation

```bash
cd Simulation/logic
cmake -S . -B build
cmake --build build
./build/logic_sim
```

## Visual Simulation

If HTML:

```bash
cd Simulation/visual
python3 -m http.server 8080
```

Open:

```text
http://localhost:8080
```
