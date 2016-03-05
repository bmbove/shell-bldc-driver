#include "datatypes.h"
#include "fet.h"
#include "pin.h"
#include "rcc.h"
#include "stm32l4xx.h"
#include "util.h"


void _fet_TIM_init(void){
    /* XXX: The ARPE bit in CR1 would be useful for adaptive frequency
     *      Use CKD bits in CR1 to dial in dead time
     */

    /* enable peripheral */
    rcc_tim_enable(FET_TIM);

    /* reset control registers */
    FET_TIM->CR1 = 0;
    FET_TIM->CR2 = 0;

    /* set prescaler */
    FET_TIM->PSC = (uint32_t)(SystemCoreClock/FET_TIM_FREQ) - 1;

    /* set output period */
    FET_TIM->ARR = (uint32_t)(FET_TIM_FREQ/FET_FREQ) - 1;

    /* center aligned mode 3 (interrupt on both up and down) */
    FET_TIM->CR1 |= TIM_CR1_CMS;

    /* preload enable (need to set COM bit to commit CCMRx register updates)*/
    FET_TIM->CR2 |= TIM_CR2_CCPC;

    /* output compare to phase-correct PWM */
    FET_TIM->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    FET_TIM->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    FET_TIM->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);

    /* preload enable for output compare registers (update on COM bit set)*/
    FET_TIM->CCMR1 |= TIM_CCMR1_OC1PE;
    FET_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
    FET_TIM->CCMR2 |= TIM_CCMR2_OC3PE;

    /* enable fast mode */
    FET_TIM->CCMR1 |= TIM_CCMR1_OC1FE;
    FET_TIM->CCMR1 |= TIM_CCMR1_OC2FE;
    FET_TIM->CCMR2 |= TIM_CCMR2_OC3FE;

    /* set high channels to idle (off)*/
    FET_TIM->CCER &= ~TIM_CCER_CC1E;
    FET_TIM->CCER &= ~TIM_CCER_CC2E;
    FET_TIM->CCER &= ~TIM_CCER_CC3E;

    /* set low (complementary) channels to idle (off)*/
    FET_TIM->CCER &= ~TIM_CCER_CC1NE;
    FET_TIM->CCER &= ~TIM_CCER_CC2NE;
    FET_TIM->CCER &= ~TIM_CCER_CC3NE;

    /* set idle states for high side - ensures off when not in use */
	FET_TIM->CR2 |= (TIM_CR2_OIS1 | TIM_CR2_OIS2 | TIM_CR2_OIS3);

    /* set idle states for low side */
	FET_TIM->CR2 &= ~(TIM_CR2_OIS1N | TIM_CR2_OIS2N | TIM_CR2_OIS3N);

    /* dead time insertion */
    /* TODO: Change this to an auto calculation from a constant in the
     * header file, ie #define  DEADTIME   50   (nanoseconds or whatever)
     */
    FET_TIM->BDTR |= TIM_BDTR_DTG_7;
    FET_TIM->BDTR |= TIM_BDTR_DTG_6;
    FET_TIM->BDTR |= TIM_BDTR_DTG_5;
    FET_TIM->BDTR |= (TIM_BDTR_DTG_2 | TIM_BDTR_DTG_1 | TIM_BDTR_DTG_0);

    fet_update();
    /* enable output */
	/*FET_TIM->BDTR |= TIM_BDTR_MOE;*/

    /* enable timer */
    FET_TIM->CR1 |= TIM_CR1_CEN;

}


void _init_fet(
    FetName         name,
    GPIO_TypeDef*   port,
    uint8_t         pin,
    uint8_t         oc,
    uint8_t         inv
){

    struct Fet *f = &_fet[name];
    f->name = name;
    f->port = port;
    f->pin  = pin;
    f->enabled = 0;
    f->oc   = oc;
    f->inv  = inv;
    /* enable gpio AF (AF 1 for TIM1 pins) */
    pin_af_init(f->port, f->pin, FET_AF, PULLDOWN);

    fet[name] = f;
}

void fet_init(void){
    /* not sure on the order of this stuff. It makes sense to initialize
     * the pins first (as pulldowns) I suppose, which is then reinforced by
     * initializing the timer and disabling all of the outputs (setting them
     * to their idle state, which should be 'off')
     */

    _init_fet(AH_FET, AH_PORT, AH_PIN, A_OC, 0); 
    _init_fet(AL_FET, AL_PORT, AL_PIN, A_OC, 0); 
    _init_fet(BH_FET, BH_PORT, BH_PIN, B_OC, 0); 
    _init_fet(BL_FET, BL_PORT, BL_PIN, B_OC, 0); 
    _init_fet(CH_FET, CH_PORT, CH_PIN, C_OC, 0); 
    _init_fet(CL_FET, CL_PORT, CL_PIN, C_OC, 0); 

    _fet_TIM_init();

}


void fet_update(void){
    /* set COM bit for everything to take effect */
    FET_TIM->EGR |= TIM_EGR_COMG;
}


void fet_disable(struct Fet* fet){
    uint8_t offset = 0;
    if (fet->inv == 1)
        offset = 2;
    FET_TIM->CCER &= ~_BV((4 * (fet->oc - 1) + offset));
}

void fet_enable(struct Fet* fet){
    uint8_t offset = 0;
    if (fet->inv == 1)
        offset = 2;

    FET_TIM->CCER |= _BV((4 * (fet->oc - 1) + offset));
}

void fet_set_duty(struct Fet* fet, float duty){

    /* constrain duty ratio to be from 0 to 100
    * These situations are obviously some kind of fault, so it might
    * be a good idea to raise some kind of flag and/or shut the motor
    * down to prevent damage to anything.
    */

    if (duty < 0.0)
        duty = 0.0;
    if (duty > 100.0)
        duty = 100.0;

    duty /= 100.0;

    switch (fet->oc){
        case A_OC:
            FET_TIM->CCR1 = (uint16_t)((FET_TIM->ARR + 0.0) * duty);
            break;
        case B_OC:
            FET_TIM->CCR2 = (uint16_t)((FET_TIM->ARR + 0.0) * duty);
            break;
        case C_OC:
            FET_TIM->CCR3 = (uint16_t)((FET_TIM->ARR + 0.0) * duty);
            break;
        default:
            FET_TIM->CCR1 = 0;
            FET_TIM->CCR2 = 0;
            FET_TIM->CCR3 = 0;
            break;
    }
}

void fet_set_polarity(struct Fet* fet, FetPolarity polarity){
    uint8_t offset = 1;

    if(fet->inv)
        offset = 3;

    if(polarity == INVERTED)
        FET_TIM->CCER |= _BV(4 * (fet->oc - 1) + offset);
    else 
        FET_TIM->CCER &= ~_BV(4 * (fet->oc - 1) + offset);
}
