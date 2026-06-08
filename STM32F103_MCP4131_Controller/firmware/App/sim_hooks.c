#include "sim_hooks.h"

__attribute__((weak)) void SimHooks_OnSpiWrite(uint8_t device, uint8_t code)
{
    (void)device;
    (void)code;
}

__attribute__((weak)) void SimHooks_OnValidPacket(uint8_t command)
{
    (void)command;
}

__attribute__((weak)) void SimHooks_OnTimeoutReset(void)
{
}
