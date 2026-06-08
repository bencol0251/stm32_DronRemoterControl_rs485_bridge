#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>
#include "app_config.h"

typedef enum
{
    PROTOCOL_CMD_SET_FIVE_KEYS = 0x10u,
    PROTOCOL_CMD_STOP_ALL_RESET = 0x11u,
    PROTOCOL_CMD_GET_STATUS = 0x20u
} Protocol_Command_t;

typedef enum
{
    PROTOCOL_STATUS_OK = 0x00u,
    PROTOCOL_STATUS_CHECKSUM_ERROR = 0x01u,
    PROTOCOL_STATUS_ADDRESS_ERROR = 0x02u,
    PROTOCOL_STATUS_COMMAND_ERROR = 0x03u,
    PROTOCOL_STATUS_BUSY = 0x04u,
    PROTOCOL_STATUS_INTERNAL_ERROR = 0x05u
} Protocol_Status_t;

typedef struct
{
    uint8_t address;
    uint8_t command;
    uint8_t key[APP_DEVICE_COUNT];
} Protocol_Request_t;

typedef struct
{
    uint8_t address;
    uint8_t command;
    Protocol_Status_t status;
    uint8_t updated_count;
} Protocol_Response_t;

uint8_t Protocol_Checksum(const uint8_t *data, uint8_t len);
bool Protocol_IsSupportedCommand(uint8_t command);
Protocol_Status_t Protocol_ParseRequest(const uint8_t *frame,
                                         uint8_t len,
                                         uint8_t local_address,
                                         Protocol_Request_t *request);
void Protocol_BuildResponse(const Protocol_Response_t *response, uint8_t *frame);

#endif
