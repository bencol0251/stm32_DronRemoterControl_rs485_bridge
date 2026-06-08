# Timer Architecture

## Timer-Only Rule

All time operations must use hardware timers.

Forbidden in application logic:

```c
HAL_Delay();
for-loop delay;
while-loop delay;
software-only blocking timeout;
```

## Timer Allocation

| Timer | Purpose | Period |
|---|---|---|
| TIM2 | Main 1 ms scheduler tick | 1 ms |
| TIM3 | Manual key debounce tick | 1 ms |
| TIM4 | Communication timeout / simulation event tick | 1 ms |

Codex may implement all timing using TIM2 only if a clean timer service is provided.

## Required Timer Service

Create:

```text
Core/Inc/timer_service.h
Core/Src/timer_service.c
```

Required API:

```c
void TimerService_Init(void);
void TimerService_On1msTick(void);
uint32_t TimerService_Millis(void);
bool TimerService_Expired(uint32_t start_ms, uint32_t timeout_ms);
void TimerService_Restart(uint32_t *timestamp_ms);
```

## HAL Callback

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        TimerService_On1msTick();
    }
}
```

## Timeout

```c
if (TimerService_Expired(last_valid_packet_ms, 3000))
{
    MCP4131_ResetAllInitial();
}
```

## Debounce

Debounce PA15/PB3 using timer ticks with a 20 ms threshold.
