#ifndef HALL_H
#define HALL_H
#include "stm32l4xx.h"

#define H100              0x04
#define H110              0x06
#define H010              0x02
#define H011              0x03
#define H001              0x01
#define H101              0x05

#define HALL_A_PORT     GPIOC
#define HALL_B_PORT     GPIOC
#define HALL_C_PORT     GPIOC

#define HALL_A_PIN      13
#define HALL_B_PIN      14
#define HALL_C_PIN      15

#define HALL_MASK       ((1 << HALL_A_PIN) | (1 << HALL_B_PIN) | (1 << HALL_C_PIN))

uint32_t volatile hall_state;
void hall_init(void);
uint32_t hall_get_state(void);
void enable_hall_interrupt(void);
void disable_hall_interrupt(void);


#endif
