#include "commutate.h"
#include "datatypes.h"
#include "hall.h"
#include "phase.h"

static void init_step(
    StepName        name,
    PhaseName       high,
    PhaseName        low,
    PhaseName        off,
    uint8_t         hall,
    StepName        prev,
    StepName        next
);

void com_init(){

    /*    step, highside, lowside, floating, hallstate, prev, next */
    init_step(STEP0, APHASE, BPHASE, CPHASE, H100, STEP5, STEP1);
    init_step(STEP1, APHASE, CPHASE, BPHASE, H110, STEP0, STEP2);
    init_step(STEP2, BPHASE, CPHASE, APHASE, H010, STEP1, STEP3);
    init_step(STEP3, BPHASE, APHASE, CPHASE, H011, STEP2, STEP4);
    init_step(STEP4, CPHASE, APHASE, BPHASE, H001, STEP3, STEP5);
    init_step(STEP5, CPHASE, BPHASE, APHASE, H101, STEP4, STEP0);

    uint8_t hall_state = hall_get_state();
    com_hall_step(hall_state);
}

void com_hall_step(uint8_t hall){

    for(uint8_t i=0; i < 6; i++){
        if(hall == com_step[i]->hall){
            com_activate_step(com_step[i]);
        }
    }
}

void com_activate_step(struct ComStep *step){

    phase_set_mode(step->high, HIGH);
    phase_set_state(step->low, LOW, 100.0);
    phase_set_mode(step->off, OFF);

    phase_update_state();
}

static void init_step(
    StepName        name,
    PhaseName       high,
    PhaseName        low,
    PhaseName        off,
    uint8_t         hall,
    StepName        prev,
    StepName        next
){
    struct ComStep *step = &_com_step[name];
    step->high = phase[high];
    step->low  = phase[low];
    step->off  = phase[off];
    step->hall = hall;
    step->prev = &_com_step[prev];
    step->next = &_com_step[next];
}
