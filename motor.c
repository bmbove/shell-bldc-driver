#include "adc.h"
#include "commutate.h"
#include "hall.h"
#include "motor.h"
#include "phase.h"
#include "fet.h"
#include "stm32l4xx.h"
#include "util.h"

static void _init_com_tim(void);
static void _init_pwr_adc(void);
static void adc_callback();
static void commutate_callback();

/* Conversion complete interrupt for EMF and I measurements */
void ADC3_IRQHandler(){

    if(PWR_ADC->ISR & ADC_ISR_EOC){
        /* disable interrupt and clear flag */
        PWR_ADC->ISR &= ~ADC_ISR_EOC;
        PWR_ADC->IER &= ~ADC_IER_EOC;
        /* do stuff */
        adc_callback();
        /*re-enable interrupt */
        PWR_ADC->IER |= ADC_IER_EOC;
    }
}

/* Do stuff in center of PWM cycle */
void TIM1_UP_TIM16_IRQHandler(){
    if(TIM1->SR & TIM_SR_UIF){
        TIM1->DIER &= ~TIM_DIER_UIE;
        /* Do stuff at bottom (0) */
        if(!(TIM1->CR1 & TIM_CR1_DIR)){
            PWR_ADC->CR |= ADC_CR_ADSTART;
        }
        /* Do stuff at top (ARR) */
        else if(TIM1->CR1 & TIM_CR1_DIR){
            /* adjust duty cycle */
        }

        TIM1->SR = 0;
        TIM1->DIER |= TIM_DIER_UIE;
    }
}

/* commutation timer */
void TIM2_IRQHandler(){

	if(TIM2->SR & TIM_SR_UIF){ 
        /*disable interrupt*/
        TIM2->DIER &= ~TIM_DIER_UIE;
        /*clear interrupt flags*/
        TIM2->SR = 0;
        /* commutate if needed */
        commutate_callback();
        /* enable interrupt */
        TIM2->DIER |= TIM_DIER_UIE;
	}
}

static void adc_callback(){

}

static void commutate_callback(){

    /* trapezoidal hall step */
    uint8_t hall = hall_get_state();
    com_hall_step(hall);
}


static void _init_pwr_adc(void){
    /* 2 conversions in sequence */
    PWR_ADC->SQR1 |= _BV(1);
    /* set seq #1 */
    PWR_ADC->SQR1 |= IA_ADCIN << 6;
    /* set seq #2 */
    PWR_ADC->SQR1 |= IB_ADCIN << 12;
    /* register interrupt */
    NVIC_EnableIRQ(ADC3_IRQn);
    /* enable end of conversion interrupt */
    PWR_ADC->IER |= ADC_IER_EOC;
}

void motor_enable(){
    FET_TIM->BDTR |= TIM_BDTR_MOE;
}

void motor_disable(){

    phase_set_state(phase[APHASE], LOW, 100.0);
    phase_set_state(phase[BPHASE], LOW, 100.0);
    phase_set_state(phase[CPHASE], LOW, 100.0);
    phase_update_state();
}

void motor_latched_shutdown(){
    motor_disable();
    FET_TIM->BDTR &= ~TIM_BDTR_MOE;
}

void motor_update_duty(float duty){
    phase_set_duty(current_step->high, duty);
    phase_set_duty(current_step->low, 100.0);
    phase_update_state();
}

void motor_init(void){

    phase_init();
    _init_pwr_adc();
    _init_com_tim();
}

static void _init_com_tim(void){

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
