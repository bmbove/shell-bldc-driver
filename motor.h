#ifndef MOTOR_H
#define MOTOR_H

#include "stm32l4xx.h"
#include "datatypes.h"
#include "fet.h"

struct _phase{
    phase_status         status;
    struct fet            *high;
    struct fet            *low;
} _phase_a, _phase_b, _phase_c;

struct _phase*  motor_phase[3];

#endif
