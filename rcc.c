#include "stm32l4xx.h"
#include "system_stm32l4xx.h"
#include "rcc.h"

void SysTick_Handler(void){
    uw_tick++;
}

void init_tick(void){
    uw_tick = 0;
    uint32_t priority_group, priority_encode, tick_priority;
    priority_group = 0x00;
    priority_encode = 0x00;
    tick_priority= (uint32_t)0x0F;
    priority_group = NVIC_GetPriorityGrouping();
    priority_encode = NVIC_EncodePriority(priority_group, tick_priority, 0);
    NVIC_SetPriority(SysTick_IRQn, priority_encode);
}

void system_clock_config(void){
    /* oscillator setup */

    /* MSI set to 4MHz, used as system clock after reset. First need to
     * update FLASH latency (default is 0) so FLASH works. */


    // enable msirange for range select
    RCC->CR |= RCC_CR_MSIRGSEL;
    // set MSI clock to range 6
    RCC->CR |= RCC_CR_MSIRANGE_6;
    // wait for MSI to be ready
    while(!(RCC->CR & RCC_CR_MSIRDY));

    /*Disable the main PLL*/
    RCC->CR &= ~RCC_CR_PLLON;

    /*wait for PLL to be disabled */
    while(RCC->CR & RCC_CR_PLLRDY);

     /*set MSI as PLL source, set multiplication and division factors */
    __RCC_PLL_CONFIG(RCC_PLLCFGR_PLLSRC_MSI, 1, 40, 7, 4, 2);

    /* Enable the main PLL*/
    RCC->CR |= RCC_CR_PLLON;

    /* Enable PLL System Clock output */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    /*wait for PLL to be ready */
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /*Set wait states to 4*/
    FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
    /*Check to make sure it worked*/
    while(!(FLASH->ACR & FLASH_ACR_LATENCY_4WS));
    /* end oscillator setup */


    /* set clock */
    /*-----------*/

    // wait for PLL to be ready
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /*set PLL as system clock */
    RCC->CFGR |= 0x03;

    /* wait for sys clock source change flag */
    while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));

    /*set AHB prescaler (HCLK config) (not divided) */
    RCC->CFGR &= ~(1 << 7);

    /*configure PCLK1 APB1 prescaler (not divided) */
    RCC->CFGR &= ~(1 << 10);

    /*configure PCLK2 APB2 prescaler (not divided) */
    RCC->CFGR &= ~(0 << 13);

    /*at this point, sysclockfreq should = 80MHz*/
}


void rcc_tim_enable(TIM_TypeDef* TIM){

    if(TIM == TIM1)
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    if(TIM == TIM3)
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
    if(TIM == TIM4)
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
}


void rcc_gpio_enable(GPIO_TypeDef* GPIO){

    uint32_t en = 0;

    if(GPIO == GPIOA) 
        en = RCC_AHB2ENR_GPIOAEN;
    if(GPIO == GPIOB) 
        en = RCC_AHB2ENR_GPIOBEN;
    if(GPIO == GPIOC) 
        en = RCC_AHB2ENR_GPIOCEN;
    if(GPIO == GPIOD) 
        en = RCC_AHB2ENR_GPIODEN;
    if(GPIO == GPIOE) 
        en = RCC_AHB2ENR_GPIOEEN;
    if(GPIO == GPIOF) 
        en = RCC_AHB2ENR_GPIOFEN;
    if(GPIO == GPIOG) 
        en = RCC_AHB2ENR_GPIOGEN;
    if(GPIO == GPIOH) 
        en = RCC_AHB2ENR_GPIOHEN;

    RCC->AHB2ENR |= en;

}
