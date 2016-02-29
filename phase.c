#include "adc.h"
#include "fet.h"
#include "phase.h"
#include "datatypes.h"

void _init_phase(PhaseName name, FetName high, FetName low, AdcName adc){
    struct Phase* p = &_phase[name];
    p->high = fet[high];
    p->low = fet[low];
    p->adc = vp_adc[adc];
    phase_set_state(p, OFF, 0);
    phase[name] = p;
}

void _phase_set_mode(struct Phase* phase, PhaseMode mode){

    phase->mode = mode;

    switch(mode){
        case COMP:
            fet_enable(phase->high);
            fet_enable(phase->low);
            fet_set_polarity(phase->low,  NORMAL);
            fet_set_polarity(phase->high, NORMAL);
            break;
        case HIGH:
            fet_enable(phase->high);
            fet_disable(phase->low);
            fet_set_polarity(phase->high, NORMAL);
            break;
        case LOW:
            fet_disable(phase->high);
            fet_enable(phase->low);
            fet_set_polarity(phase->low,  INVERTED);
            break;
        case OFF:
        default:
            fet_disable(phase->high);
            fet_disable(phase->low);
            phase->mode = OFF;
            break;
    }

}

void phase_update_state(){
    fet_update();
}

void _phase_set_duty(struct Phase* phase, float duty){

    if (duty < 0.0)
        duty = 0.0;
    if (duty > DUTY_MAX)
        duty = DUTY_MAX;
    fet_set_duty(phase->high, duty);
    phase->duty = duty; 
}

void phase_init(void){

    fet_init();
    adc_init();
    /* XXX get rid of the adc numbers, replace with enum */
    /*          phase, high fet, low fet, adc */
    _init_phase(AP, AH, AL, 0);
    _init_phase(BP, BH, BL, 1);
    _init_phase(CP, CH, CL, 2);
    fet_update();
}

void phase_set_state(struct Phase* phase, PhaseMode mode, float duty){
    _phase_set_mode(phase, mode);
    _phase_set_duty(phase, duty);
}
