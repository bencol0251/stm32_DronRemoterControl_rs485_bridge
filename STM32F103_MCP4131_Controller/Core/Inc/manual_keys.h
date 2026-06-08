#ifndef MANUAL_KEYS_H
#define MANUAL_KEYS_H

#include <stdbool.h>

void ManualKeys_Init(void);
void ManualKeys_Task(void);
bool ManualKeys_IsReverseActive(void);

#endif
