#include "fet.h"
#include "rcc.c"
#include "stm32l4xx.h"


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

    /* enable high channels */
    /*FET_TIM->CCER |= TIM_CCER_CC1E;*/
    /*FET_TIM->CCER |= TIM_CCER_CC2E;*/
    /*FET_TIM->CCER |= TIM_CCER_CC3E;*/

    /* enable low (complementary) channels */
    /*FET_TIM->CCER |= TIM_CCER_CC1NE;*/
    /*FET_TIM->CCER |= TIM_CCER_CC2NE;*/
    /*FET_TIM->CCER |= TIM_CCER_CC3NE;*/

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

    /* set COM bit for everything to take effect */
    FET_TIM->EGR |= TIM_EGR_COMG;

    /* enable output */
	/*FET_TIM->BDTR |= TIM_BDTR_MOE;*/

    /* enable timer */
    FET_TIME->CR1 |= TIM_CR1_CEN;

}


void _init_fet(
    struct _fet*    fet,
    fet_name        name,
    GPIO_TypeDef*   port,
    uint8_t         pin,
    TIM_TypeDef*    tim,
    uint8_t         oc,
    uint8_t         inv
){

    fet->name = name;
    fet->port = port;
    fet->pin  = pin;
    fet->tim  = tim;
    fet->oc   = oc;
    fet->inv  = inv;

    fet->phase   = 0;
    fet->enabled = 0;

    /* enable gpio AF (AF 1 for TIM1 pins) */
    pin_af_init(fet->port, fet->pin, FET_AF, PULLDOWN);
}

void init_fets(void){
    /* not sure on the order of this stuff. It makes sense to initialize
     * the pins first (as pulldowns) I suppose, which is then reinforced by
     * initializing the timer and disabling all of the outputs (setting them
     * to their idle state, which should be 'off')
     */

    _init_fet(
    _fet_TIM_init();

}
