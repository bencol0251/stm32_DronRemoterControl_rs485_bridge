# Firmware Architecture

## Required Modules

```text
Core/Inc/app.h
Core/Inc/app_config.h
Core/Inc/pinmap.h
Core/Inc/protocol.h
Core/Inc/rs485.h
Core/Inc/mcp4131.h
Core/Inc/dip_switch.h
Core/Inc/manual_keys.h
Core/Inc/timer_service.h
Core/Inc/sim_hooks.h

Core/Src/app.c
Core/Src/protocol.c
Core/Src/rs485.c
Core/Src/mcp4131.c
Core/Src/dip_switch.c
Core/Src/manual_keys.c
Core/Src/timer_service.c
Core/Src/sim_hooks.c
```

## Main Loop

```c
while (1)
{
    App_Task();
}
```

## Application Flow

1. RS485 task receives packet.
2. Protocol validates packet.
3. DIP switch address is compared.
4. Command is processed.
5. MCP4131 updates immediately.
6. Response is transmitted.
7. Timer service checks timeout.
8. Manual key service checks PA15/PB3 debounce.

## Immediate Update Function

```c
void App_UpdateFiveKeys(uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4, uint8_t k5)
{
    MCP4131_WriteCode(MCP_U1, KeyMap_U1(k1));
    MCP4131_WriteCode(MCP_U2, KeyMap_U2(k2));
    MCP4131_WriteCode(MCP_U3, KeyMap_U3(k3));
    MCP4131_WriteCode(MCP_U4, KeyMap_U4(k4));
    MCP4131_WriteCode(MCP_U5, KeyMap_U5(k5));
}
```
