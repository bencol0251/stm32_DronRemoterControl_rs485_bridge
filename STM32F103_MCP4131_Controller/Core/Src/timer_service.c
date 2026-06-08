#include "timer_service.h"

static volatile uint32_t s_millis;

void TimerService_Init(void)
{
    s_millis = 0u;
}

void TimerService_On1msTick(void)
{
    s_millis++;
}

uint32_t TimerService_Millis(void)
{
    return s_millis;
}

bool TimerService_Expired(uint32_t start_ms, uint32_t timeout_ms)
{
    return (uint32_t)(TimerService_Millis() - start_ms) >= timeout_ms;
}

void TimerService_Restart(uint32_t *timestamp_ms)
{
    if (timestamp_ms != 0)
    {
        *timestamp_ms = TimerService_Millis();
    }
}
