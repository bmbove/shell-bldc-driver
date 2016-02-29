#ifndef PHASE_H
#define PHASE_H

#include "datatypes.h"
#include "adc.h"
#include "fet.h"

/* The duty ratio definitely needs to be limited, mostly to prevent
 * runaway PID from burning stuff up. It's limited by hardware to 100%, 
 * obviously, but 100% duty cycle running a motor at 48V will be a mess. 
 * This might need to be adjusted for sinusoidal outputs.
 */
#define DUTY_MAX        30.0


struct Phase{
    struct Fet         *high;
    struct Fet          *low;
    struct AdcCh        *adc;
    PhaseMode           mode;
    float               duty;
} _phase[3];

struct Phase* phase[3];

void _init_phase(PhaseName name, FetName high, FetName low, AdcName adc);
void _phase_set_duty(struct Phase* phase, float duty);
void _phase_set_mode(struct Phase* phase, PhaseMode mode);

void phase_update_state(void);
void phase_set_state(struct Phase* phase, PhaseMode mode, float duty);
void phase_init(void);

#endif
