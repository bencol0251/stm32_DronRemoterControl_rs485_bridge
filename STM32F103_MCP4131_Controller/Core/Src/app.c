#include "app.h"
#include "app_config.h"
#include "dip_switch.h"
#include "key_mapping.h"
#include "manual_keys.h"
#include "mcp4131.h"
#include "protocol.h"
#include "rs485.h"
#include "sim_hooks.h"
#include "timer_service.h"

static uint32_t s_last_valid_packet_ms;
static bool s_timeout_reset_done;

void App_UpdateFiveKeys(uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4, uint8_t k5)
{
    (void)MCP4131_WriteCode(MCP_U1, KeyMap_U1(k1));
    (void)MCP4131_WriteCode(MCP_U2, KeyMap_U2(k2));
    (void)MCP4131_WriteCode(MCP_U3, KeyMap_U3(k3));
    (void)MCP4131_WriteCode(MCP_U4, KeyMap_U4(k4));
    (void)MCP4131_WriteCode(MCP_U5, KeyMap_U5(k5));
}

void App_Init(UART_HandleTypeDef *uart, SPI_HandleTypeDef *spi)
{
    TimerService_Init();
    DipSwitch_Init();
    MCP4131_Init(spi);
    ManualKeys_Init();
    RS485_Init(uart);
    TimerService_Restart(&s_last_valid_packet_ms);
    s_timeout_reset_done = false;
}

void App_Task(void)
{
    uint8_t frame[APP_PROTOCOL_REQUEST_LEN];
    Protocol_Request_t request;
    uint8_t response_frame[APP_PROTOCOL_RESPONSE_LEN];
    uint8_t local_address = DipSwitch_GetAddress();

    ManualKeys_Task();

    if (RS485_Task(frame))
    {
        Protocol_Status_t status = Protocol_ParseRequest(frame,
                                                          APP_PROTOCOL_REQUEST_LEN,
                                                          local_address,
                                                          &request);
        Protocol_Response_t response =
        {
            .address = frame[1],
            .command = frame[2],
            .status = status,
            .updated_count = 0u
        };

        if (status == PROTOCOL_STATUS_OK)
        {
            if (request.command == PROTOCOL_CMD_SET_FIVE_KEYS)
            {
                App_UpdateFiveKeys(request.key[0], request.key[1], request.key[2],
                                   request.key[3], request.key[4]);
                response.updated_count = MCP_COUNT;
            }
            else if (request.command == PROTOCOL_CMD_STOP_ALL_RESET)
            {
                (void)MCP4131_ResetAllInitial();
                response.updated_count = MCP_COUNT;
            }
            else
            {
                response.updated_count = MCP_COUNT;
            }

            TimerService_Restart(&s_last_valid_packet_ms);
            s_timeout_reset_done = false;
            SimHooks_OnValidPacket(request.command);
        }

        Protocol_BuildResponse(&response, response_frame);
        (void)RS485_Send(response_frame, APP_PROTOCOL_RESPONSE_LEN);
    }

    if (!s_timeout_reset_done &&
        TimerService_Expired(s_last_valid_packet_ms, APP_PACKET_TIMEOUT_MS))
    {
        (void)MCP4131_ResetAllInitial();
        s_timeout_reset_done = true;
        SimHooks_OnTimeoutReset();
    }
}
