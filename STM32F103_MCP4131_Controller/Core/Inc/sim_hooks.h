#ifndef SIM_HOOKS_H
#define SIM_HOOKS_H

#include <stdint.h>

void SimHooks_OnSpiWrite(uint8_t device, uint8_t code);
void SimHooks_OnValidPacket(uint8_t command);
void SimHooks_OnTimeoutReset(void);

#endif
