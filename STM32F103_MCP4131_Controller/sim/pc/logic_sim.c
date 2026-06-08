#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app_config.h"
#include "key_mapping.h"
#include "protocol.h"
#include "timer_service.h"

typedef struct
{
    bool pb12;
    bool pb13;
    bool pb14;
    bool pb15;
    bool pa15_low;
    bool pb3_low;
    bool last_pa15;
    bool last_pb3;
    uint32_t pa15_changed_ms;
    uint32_t pb3_changed_ms;
    uint32_t last_valid_packet_ms;
    uint8_t mcp[APP_DEVICE_COUNT];
    uint8_t spi_order[APP_DEVICE_COUNT];
    uint8_t spi_count;
    bool timeout_reset;
} SimState_t;

static SimState_t sim;

static void Sim_Log(const char *msg)
{
    printf("[%04lu ms] %s\n", (unsigned long)TimerService_Millis(), msg);
}

static void Sim_WriteMcp(uint8_t dev, uint8_t code)
{
    sim.mcp[dev] = code;
    if (sim.spi_count < APP_DEVICE_COUNT)
    {
        sim.spi_order[sim.spi_count++] = dev;
    }
    printf("[%04lu ms] SPI U%u code=%u\n",
           (unsigned long)TimerService_Millis(),
           (unsigned)(dev + 1u),
           (unsigned)code);
}

static void Sim_ResetAll(void)
{
    Sim_WriteMcp(0u, MCP4131_INITIAL_U1);
    Sim_WriteMcp(1u, MCP4131_INITIAL_U2);
    Sim_WriteMcp(2u, MCP4131_INITIAL_U3);
    Sim_WriteMcp(3u, MCP4131_INITIAL_U4);
    Sim_WriteMcp(4u, MCP4131_INITIAL_U5);
}

static void Sim_Init(void)
{
    memset(&sim, 0, sizeof(sim));
    TimerService_Init();
    sim.pb12 = true;
    sim.pb13 = true;
    sim.pb14 = true;
    sim.pb15 = true;
    Sim_ResetAll();
    sim.spi_count = 0u;
    TimerService_Restart(&sim.last_valid_packet_ms);
}

void SimTimer_AdvanceMs(uint32_t ms)
{
    for (uint32_t i = 0u; i < ms; i++)
    {
        TimerService_On1msTick();

        if (!sim.timeout_reset &&
            TimerService_Expired(sim.last_valid_packet_ms, APP_PACKET_TIMEOUT_MS))
        {
            Sim_Log("TIMEOUT RESET");
            Sim_ResetAll();
            sim.timeout_reset = true;
        }

        bool pa15_changed = sim.pa15_low != sim.last_pa15;
        bool pb3_changed = sim.pb3_low != sim.last_pb3;
        if (pa15_changed)
        {
            sim.last_pa15 = sim.pa15_low;
            sim.pa15_changed_ms = TimerService_Millis();
        }
        if (pb3_changed)
        {
            sim.last_pb3 = sim.pb3_low;
            sim.pb3_changed_ms = TimerService_Millis();
        }

        if ((sim.last_pa15 && TimerService_Expired(sim.pa15_changed_ms, APP_MANUAL_KEY_DEBOUNCE_MS)) ||
            (sim.last_pb3 && TimerService_Expired(sim.pb3_changed_ms, APP_MANUAL_KEY_DEBOUNCE_MS)))
        {
            Sim_WriteMcp(4u, MCP4131_U5_REVERSE_CODE);
        }
    }
}

void Sim_SetDipAddressPins(bool pb12, bool pb13, bool pb14, bool pb15)
{
    sim.pb12 = pb12;
    sim.pb13 = pb13;
    sim.pb14 = pb14;
    sim.pb15 = pb15;
}

void Sim_SetManualKeyPA15(bool low_active)
{
    sim.pa15_low = low_active;
}

void Sim_SetManualKeyPB3(bool low_active)
{
    sim.pb3_low = low_active;
}

static uint8_t Sim_GetDipAddress(void)
{
    return (uint8_t)(((sim.pb12 ? 1u : 0u) << 3) |
                     ((sim.pb13 ? 1u : 0u) << 2) |
                     ((sim.pb14 ? 1u : 0u) << 1) |
                     (sim.pb15 ? 1u : 0u));
}

void Sim_SendPacket(const uint8_t *packet, uint8_t len)
{
    Protocol_Request_t request;
    uint8_t local_address = Sim_GetDipAddress();
    Protocol_Status_t status = Protocol_ParseRequest(packet, len, local_address, &request);

    if (status != PROTOCOL_STATUS_OK)
    {
        printf("[%04lu ms] RX rejected status=%u\n",
               (unsigned long)TimerService_Millis(),
               (unsigned)status);
        return;
    }

    Sim_Log("RX valid packet");
    sim.spi_count = 0u;

    if (request.command == PROTOCOL_CMD_SET_FIVE_KEYS)
    {
        Sim_WriteMcp(0u, KeyMap_U1(request.key[0]));
        Sim_WriteMcp(1u, KeyMap_U2(request.key[1]));
        Sim_WriteMcp(2u, KeyMap_U3(request.key[2]));
        Sim_WriteMcp(3u, KeyMap_U4(request.key[3]));
        Sim_WriteMcp(4u, KeyMap_U5(request.key[4]));
    }
    else if (request.command == PROTOCOL_CMD_STOP_ALL_RESET)
    {
        Sim_ResetAll();
    }

    TimerService_Restart(&sim.last_valid_packet_ms);
    sim.timeout_reset = false;
}

static void MakePacket(uint8_t address, uint8_t command, const uint8_t keys[APP_DEVICE_COUNT], uint8_t packet[APP_PROTOCOL_REQUEST_LEN])
{
    packet[0] = APP_PROTOCOL_REQUEST_HEADER;
    packet[1] = address;
    packet[2] = command;
    for (uint8_t i = 0u; i < APP_DEVICE_COUNT; i++)
    {
        packet[3u + i] = keys[i];
    }
    packet[8] = Protocol_Checksum(packet, 8u);
}

static void Test_Checksum(void)
{
    uint8_t bytes[] = { 0xAAu, 0x03u, 0x10u, 1u, 3u, 5u, 7u, 33u };
    assert(Protocol_Checksum(bytes, sizeof(bytes)) == 0xEEu);
}

static void Test_DipAllAddresses(void)
{
    for (uint8_t address = 0u; address < 16u; address++)
    {
        Sim_SetDipAddressPins((address & 8u) != 0u,
                              (address & 4u) != 0u,
                              (address & 2u) != 0u,
                              (address & 1u) != 0u);
        assert(Sim_GetDipAddress() == address);
    }
}

static void Test_ValidPacketUpdates(void)
{
    Sim_Init();
    Sim_SetDipAddressPins(false, false, true, true);
    uint8_t keys[APP_DEVICE_COUNT] = { 1u, 3u, 5u, 7u, 33u };
    uint8_t packet[APP_PROTOCOL_REQUEST_LEN];
    MakePacket(3u, PROTOCOL_CMD_SET_FIVE_KEYS, keys, packet);
    Sim_SendPacket(packet, sizeof(packet));

    assert(sim.mcp[0] == 51u);
    assert(sim.mcp[1] == 51u);
    assert(sim.mcp[2] == 51u);
    assert(sim.mcp[3] == 51u);
    assert(sim.mcp[4] == 23u);
    for (uint8_t i = 0u; i < APP_DEVICE_COUNT; i++)
    {
        assert(sim.spi_order[i] == i);
    }
}

static void Test_WrongAddressAndChecksumRejected(void)
{
    Sim_Init();
    uint8_t keys[APP_DEVICE_COUNT] = { 1u, 3u, 5u, 7u, 33u };
    uint8_t packet[APP_PROTOCOL_REQUEST_LEN];
    MakePacket(2u, PROTOCOL_CMD_SET_FIVE_KEYS, keys, packet);
    Sim_SendPacket(packet, sizeof(packet));
    assert(sim.mcp[0] == MCP4131_INITIAL_U1);

    MakePacket(15u, PROTOCOL_CMD_SET_FIVE_KEYS, keys, packet);
    packet[8] ^= 0x55u;
    Sim_SendPacket(packet, sizeof(packet));
    assert(sim.mcp[4] == MCP4131_INITIAL_U5);
}

static void Test_ManualKeys(void)
{
    Sim_Init();
    Sim_SetManualKeyPA15(true);
    SimTimer_AdvanceMs(20u);
    assert(sim.mcp[4] == MCP4131_U5_REVERSE_CODE);

    Sim_Init();
    Sim_SetManualKeyPB3(true);
    SimTimer_AdvanceMs(20u);
    assert(sim.mcp[4] == MCP4131_U5_REVERSE_CODE);
}

static void Test_TimeoutReset(void)
{
    Sim_Init();
    uint8_t keys[APP_DEVICE_COUNT] = { 1u, 3u, 5u, 7u, 33u };
    uint8_t packet[APP_PROTOCOL_REQUEST_LEN];
    MakePacket(15u, PROTOCOL_CMD_SET_FIVE_KEYS, keys, packet);
    Sim_SendPacket(packet, sizeof(packet));
    SimTimer_AdvanceMs(3000u);
    assert(sim.timeout_reset);
    assert(sim.mcp[0] == MCP4131_INITIAL_U1);
    assert(sim.mcp[4] == MCP4131_INITIAL_U5);
}

int main(void)
{
    Sim_Init();
    Test_Checksum();
    Test_DipAllAddresses();
    Test_ValidPacketUpdates();
    Test_WrongAddressAndChecksumRejected();
    Test_ManualKeys();
    Test_TimeoutReset();
    puts("logic_sim: all tests passed");
    return 0;
}
