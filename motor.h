#ifndef MOTOR_H
#define MOTOR_H

#include "stm32l4xx.h"
#include "datatypes.h"
#include "fet.h"

#ifdef CLOSED_LOOP
/* 10us Hall Check          */
#define COM_TIM_CLK     20000000
#define COM_CLK         100000
#else
/* 10ms open loop commutate */
#define COM_TIM_CLK     2000000
#define COM_CLK         100
#endif

void motor_enable();
void motor_disable();
void motor_latched_shutdown();
void motor_update_duty(float duty);
void motor_init(void);

#endif
