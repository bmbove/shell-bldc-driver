#ifndef RCC_H
#define RCC_H

#define __RCC_PLL_CONFIG(__PLLSOURCE__, __PLLM__, __PLLN__, __PLLP__, __PLLQ__,__PLLR__ )      \
                   (RCC->PLLCFGR = (((__PLLM__) - 1) << 4U) | ((__PLLN__) << 8U) | (((__PLLP__)      >> 4U ) << 17U) | \
                    (__PLLSOURCE__) | ((((__PLLQ__) >> 1U) - 1) << 21U) | ((((__PLLR__) >> 1U)      - 1) << 25U))

#define TICK_PRIORITY   ((uint32_t)0x0F)

uint32_t volatile uw_tick;
uint32_t sysclockfreq;
void init_tick(void);
void system_clock_config(void);

void rcc_tim_enable(TIM_TypeDef* TIM);
void rcc_gpio_enable(GPIO_TypeDef* GPIO);
void rcc_adc_enable();

#endif
