#include "protocol.h"

uint8_t Protocol_Checksum(const uint8_t *data, uint8_t len)
{
    uint8_t sum = 0u;

    for (uint8_t i = 0u; i < len; i++)
    {
        sum = (uint8_t)(sum + data[i]);
    }

    return sum;
}

bool Protocol_IsSupportedCommand(uint8_t command)
{
    return (command == PROTOCOL_CMD_SET_FIVE_KEYS) ||
           (command == PROTOCOL_CMD_STOP_ALL_RESET) ||
           (command == PROTOCOL_CMD_GET_STATUS);
}

Protocol_Status_t Protocol_ParseRequest(const uint8_t *frame,
                                         uint8_t len,
                                         uint8_t local_address,
                                         Protocol_Request_t *request)
{
    if ((frame == 0) || (request == 0) || (len != APP_PROTOCOL_REQUEST_LEN) ||
        (frame[0] != APP_PROTOCOL_REQUEST_HEADER))
    {
        return PROTOCOL_STATUS_COMMAND_ERROR;
    }

    if (Protocol_Checksum(frame, APP_PROTOCOL_REQUEST_LEN - 1u) != frame[8])
    {
        return PROTOCOL_STATUS_CHECKSUM_ERROR;
    }

    if (frame[1] != local_address)
    {
        return PROTOCOL_STATUS_ADDRESS_ERROR;
    }

    if (!Protocol_IsSupportedCommand(frame[2]))
    {
        return PROTOCOL_STATUS_COMMAND_ERROR;
    }

    request->address = frame[1];
    request->command = frame[2];
    for (uint8_t i = 0u; i < APP_DEVICE_COUNT; i++)
    {
        request->key[i] = frame[3u + i];
    }

    return PROTOCOL_STATUS_OK;
}

void Protocol_BuildResponse(const Protocol_Response_t *response, uint8_t *frame)
{
    frame[0] = APP_PROTOCOL_RESPONSE_HEADER;
    frame[1] = response->address;
    frame[2] = response->command;
    frame[3] = (uint8_t)response->status;
    frame[4] = response->updated_count;
    frame[5] = Protocol_Checksum(frame, APP_PROTOCOL_RESPONSE_LEN - 1u);
}
