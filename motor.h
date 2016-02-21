#ifndef MOTOR_H
#define MOTOR_H

#include "datatypes.h"

struct _phase{
    phase_status         status;
    struct *fet          high;
    struct *fet          low;
    uint16_t             duty;
} _phase_a, _phase_b, _phase_c;

struct _phase*  motor_phase[3];

#endif
