# Logic Simulation

## Purpose

Provide a host-side simulation that tests firmware logic without STM32 hardware.

## Location

```text
Simulation/logic/
```

## Required Simulated Items

- Timer tick source
- RS485 packet input
- Protocol parser
- DIP switch PB12-PB15
- Manual keys PA15/PB3
- MCP4131 virtual state
- Timeout reset

## Required API

```c
void SimTimer_AdvanceMs(uint32_t ms);
void Sim_SetDipAddressPins(bool pb12, bool pb13, bool pb14, bool pb15);
void Sim_SetManualKeyPA15(bool low_active);
void Sim_SetManualKeyPB3(bool low_active);
void Sim_SendPacket(const uint8_t *packet, uint8_t len);
```

## Required Tests

- Valid packet updates U1-U5.
- Wrong address ignored.
- Invalid checksum rejected.
- PA15 low sets U5 code to 23.
- PB3 low sets U5 code to 23.
- 3000 ms timeout resets all MCP4131s.
- All 16 DIP addresses decode correctly.

## Output Log Example

```text
[0000 ms] DIP address = 3
[0010 ms] RX valid SET_FIVE_KEYS
[0011 ms] SPI U1 code=51
[0011 ms] SPI U2 code=51
[0011 ms] SPI U3 code=51
[0011 ms] SPI U4 code=51
[0011 ms] SPI U5 code=23
[3012 ms] TIMEOUT RESET
```
