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


struct MotorStep{
    struct Phase*       high;
    struct Phase*        low;
    struct Phase*        off;
    uint8_t             hall;
    struct MotorStep*   prev;
    struct MotorStep*   next;
} _com_step[6];


struct MotorStep *com_step[6];
float duty_g;

void _init_com_tim(void);
void motor_init(void);

void motor_next_step(void);
void activate_step(struct MotorStep* step);
#endif
