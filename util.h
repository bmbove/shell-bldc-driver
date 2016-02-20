#ifndef UTIL_H
#define UTIL_H
#include "stm32l4xx.h"

#define _BV(__PIN__)    ((uint32_t)1 << (__PIN__))

void delay_ms(uint16_t ms);
#endif
