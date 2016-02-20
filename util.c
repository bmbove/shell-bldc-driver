#include "stm32l4xx.h"
#include "rcc.h"
#include "util.h"

void delay_ms(uint16_t ms){
    uint32_t now = uw_tick;
    uint32_t wait = (uint32_t)ms * 1000;
    while((uw_tick - now) < wait);
}
