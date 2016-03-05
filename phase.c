#include "adc.h"
#include "fet.h"
#include "phase.h"
#include "datatypes.h"

static void _init_phase(PhaseName name, FetName high, FetName low, AdcName adc_n){
    struct Phase* p = &_phase[name];
    p->high = fet[high];
    p->low = fet[low];
    p->adc = adc[adc_n];
    phase_set_state(p, OFF, 0);
    phase[name] = p;
}

void phase_set_mode(struct Phase* phase, PhaseMode mode){

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

void phase_set_duty(struct Phase* phase, float duty){

    if (duty < 0.0)
        duty = 0.0;
    if (duty > 100.0)
        duty = 100.0;
    fet_set_duty(phase->high, duty);
    phase->duty = duty; 
}

void phase_init(void){

    fet_init();
    adc_init();
    /*          phase, high fet, low fet, adc */
    _init_phase(APHASE, AH_FET, AL_FET, ADC_VA);
    _init_phase(BPHASE, BH_FET, BL_FET, ADC_VB);
    _init_phase(CPHASE, CH_FET, CL_FET, ADC_VC);
    fet_update();
}

void phase_set_state(struct Phase* phase, PhaseMode mode, float duty){
    phase_set_mode(phase, mode);
    phase_set_duty(phase, duty);
}
