#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

void TimerService_Init(void);
void TimerService_On1msTick(void);
uint32_t TimerService_Millis(void);
bool TimerService_Expired(uint32_t start_ms, uint32_t timeout_ms);
void TimerService_Restart(uint32_t *timestamp_ms);

#endif
