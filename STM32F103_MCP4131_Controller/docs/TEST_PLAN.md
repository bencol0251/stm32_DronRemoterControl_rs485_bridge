# Test Plan

## Unit Tests

| ID | Test | Expected |
|---|---|---|
| UT-001 | Checksum | Correct modulo-256 result |
| UT-002 | DIP decode | All 16 addresses correct |
| UT-003 | Key mapping | Correct MCP codes |
| UT-004 | PA15 debounce | Active after 20 ms |
| UT-005 | PB3 debounce | Active after 20 ms |
| UT-006 | Timer timeout | Reset at 3000 ms |
| UT-007 | Packet parser | Valid packet accepted |

## Integration Tests

| ID | Test | Expected |
|---|---|---|
| IT-001 | Valid packet | U1-U5 update |
| IT-002 | Wrong address | No update |
| IT-003 | Invalid checksum | Rejected |
| IT-004 | STOP_ALL_RESET | Initial values restored |
| IT-005 | PA15 low | U5 code 23 |
| IT-006 | PB3 low | U5 code 23 |
| IT-007 | No packet 3 seconds | Timeout reset |

## Hardware Verification

Use a logic analyzer:

- SPI1 SCK near 1 MHz.
- CS sequence is U1 → U2 → U3 → U4 → U5.
- No 20 ms gap between updates.
- Total update time less than 100 ms.
