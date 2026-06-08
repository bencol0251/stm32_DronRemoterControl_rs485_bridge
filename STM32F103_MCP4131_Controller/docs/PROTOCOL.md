# RS485 Protocol

## Request Frame

| Byte | Name | Description |
|---|---|---|
| 0 | Header | 0xAA |
| 1 | Address | Target device address |
| 2 | Command | Command code |
| 3 | Key1 | U1 key |
| 4 | Key2 | U2 key |
| 5 | Key3 | U3 key |
| 6 | Key4 | U4 key |
| 7 | Key5 | U5 key |
| 8 | Checksum | Sum bytes 0-7 modulo 256 |

## Response Frame

| Byte | Name | Description |
|---|---|---|
| 0 | Header | 0x55 |
| 1 | Address | Device address |
| 2 | Command | Echo command |
| 3 | Status | Result |
| 4 | Count | Updated MCP count |
| 5 | Checksum | Sum bytes 0-4 modulo 256 |

## Commands

| Code | Name |
|---|---|
| 0x10 | SET_FIVE_KEYS |
| 0x11 | STOP_ALL_RESET |
| 0x20 | GET_STATUS |

## Status Codes

| Code | Name |
|---|---|
| 0x00 | OK |
| 0x01 | CHECKSUM_ERROR |
| 0x02 | ADDRESS_ERROR |
| 0x03 | COMMAND_ERROR |
| 0x04 | BUSY |
| 0x05 | INTERNAL_ERROR |
