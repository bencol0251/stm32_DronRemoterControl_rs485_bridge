# Test Coverage

The host logic simulation covers the required behavior:

- Checksum calculation.
- All 16 DIP address combinations.
- Valid SET_FIVE_KEYS packet updates U1 through U5.
- Wrong address rejection.
- Invalid checksum rejection.
- PA15 active-low manual reverse.
- PB3 active-low manual reverse.
- 3000 ms timeout reset.
- SPI update order U1, U2, U3, U4, U5.

Run from `Simulation/logic`.
