#ifndef __SBUS_H
#define __SBUS_H
#include "stm32f4xx.h"

extern volatile uint16_t CH[17];
void Sbus_Init(void);
void Sbus_Data_Decoding(void);
void Sbus_update(void);
#endif
