#include "adc.h"
#include "hall.h"
#include "motor.h"
#include "phase.h"
#include "stm32l4xx.h"
#include "util.h"

/* globals, across all control schemes: 
 * read new set point from throttle
 * duty ratio 'pid' scheme: don't allow (duty * Vbat) to be more than 1V from
 * back-EMF read off of floating phase
 */

uint8_t volatile adc_count;
uint32_t volatile EMF;
uint32_t volatile IA;
uint32_t volatile IB;
struct MotorStep *current_step;

/* Conversion complete interrupt for EMF and I measurements */
void ADC3_IRQHandler(){

    if(PWR_ADC->ISR & ADC_ISR_EOC){
        /* disable interrupt and clear flag */
        PWR_ADC->IER &= ~ADC_IER_EOC;
        PWR_ADC->ISR &= ~ADC_ISR_EOC;
        switch(adc_count){
                case 0:
                    EMF = PWR_ADC->DR;
                    adc_count = 1;
                    break;
                case 1:
                    IA = PWR_ADC->DR;
                    adc_count = 2;
                    break;
                case 2:
                    IB = PWR_ADC->DR;
                    adc_count = 0;
                    break;

        }
        /*re-enable interrupt */
        PWR_ADC->IER |= ADC_IER_EOC;
    }
}

/* start ADC conversion sequence- should ping in middle of PWM cycle */
void TIM1_UP_IRQHandler(){
    PWR_ADC->CR |= ADC_CR_ADSTART;
}

/* check hall sensors */
void TIM2_IRQHandler(){

	if(TIM2->SR & TIM_SR_UIF){ 
        /*disable interrupt*/
        TIM2->DIER &= ~TIM_DIER_UIE;
        /*clear interrupt flags*/
        TIM2->SR = 0;
        /* check hall state */
#ifdef CLOSED_LOOP
        uint32_t new_state = hall_get_state();   
        if(hall_state != new_state){
            /* if new state, commutate */
            hall_state = new_state;
            motor_hall_step();
        }
#else
        motor_next_step();
#endif
        TIM2->DIER |= TIM_DIER_UIE;
	}
}


void _init_pwr_adc(void){
    /* 3 conversions in sequence */
    PWR_ADC->SQR1 |= _BV(2);
    /* set seq #2 */
    PWR_ADC->SQR1 |= IA_ADCIN << 12;
    /* set seq #3 */
    PWR_ADC->SQR1 |= IB_ADCIN << 18;
    /* register interrupt */
    NVIC_EnableIRQ(ADC3_IRQn);
    /* enable end of conversion interrupt */
    PWR_ADC->IER |= ADC_IER_EOC;
    adc_count = 0;
}

void motor_next_step(){
    struct MotorStep *new_step = current_step->next;
    activate_step(new_step);
    current_step = new_step;
}

void activate_step(struct MotorStep* step){

    phase_set_state(step->off, OFF, 0.0);
    phase_set_state(step->high, COMP, duty_g);
    phase_set_state(step->low, LOW, 100.0);
    while(PWR_ADC->CR & ADC_CR_ADSTART)
        PWR_ADC->CR |= ADC_CR_ADSTP;
    PWR_ADC->SQR1 &= ~ADC_SQR1_SQ1;
    PWR_ADC->SQR1 |= step->off->adc->ch << 6;
    phase_update_state();
}


void _init_step(
    uint8_t     step,
    PhaseName   h_name,
    PhaseName   l_name,
    PhaseName   f_name,
    uint8_t     hall,
    uint8_t     prev,
    uint8_t     next
){

    struct MotorStep * s = &_com_step[step];
    s->high = phase[h_name];
    s->low = phase[l_name];
    s->off = phase[f_name];
    s->hall = hall;
    s->prev = &_com_step[prev];
    s->next = &_com_step[next];

    com_step[step] = s;
}


void motor_init(void){

    phase_init();
    /*         step, high, low, float, hall, prev, next*/
    _init_step(0, AP, BP, CP, H100, 5, 1);
    _init_step(1, AP, CP, BP, H110, 0, 2);
    _init_step(2, BP, CP, AP, H010, 1, 3);
    _init_step(3, BP, AP, CP, H011, 2, 4);
    _init_step(4, CP, AP, BP, H001, 3, 5);
    _init_step(5, CP, BP, AP, H101, 4, 0);

    current_step = &_com_step[0];

    _init_com_tim();
}

void _init_com_tim(void){

    /* enable TIM2 peripheral */
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    /* reset control registers */
    TIM2->CR1 = 0;
    TIM2->CR2 = 0;

    /* set prescaler */
    TIM2->PSC = (uint32_t)(SystemCoreClock/COM_TIM_CLK) - 1;

    /* set output period */
    TIM2->ARR = (uint32_t)(COM_TIM_CLK/COM_CLK);

    /* only overflow generates update interrupt */
    TIM2->CR1 |= TIM_CR1_URS;

    /* register interrupt */
    NVIC_EnableIRQ(TIM2_IRQn);

    /* enable timer */
    TIM2->CR1 |= TIM_CR1_CEN;

    /* enable overflow interrupt */
    TIM2->DIER |= TIM_DIER_UIE;
}
