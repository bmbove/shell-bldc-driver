#include "datatypes.h"
#include "led.h"
#include "pin.h"
#include "util.h"
#include "rcc.h"

void _led_tim_init(struct _led* led){

    if(!(led->tim->CR1 & TIM_CR1_CEN)){
        /* enable TIM peripheral */
        rcc_tim_enable(led->tim);
        /* reset control registers */
        led->tim->CR1 = 0;
        led->tim->CR2 = 0;
        /* set prescaler */
        led->tim->PSC = (uint32_t)(SystemCoreClock/LED_TIM_CLK) - 1;
        /* set output period */
        led->tim->ARR = (uint32_t)(LED_TIM_CLK/LED_CLK);
        /* enable timer */
        led->tim->CR1 |= TIM_CR1_CEN;
    }

    /* enable output compare */
    led->tim->CCER |= _BV((led->oc - 1) * 4);
    /* set to force inactive (off) */
    _led_modeset(led, LED_OFF);
    /*_led_modeset(led, LED_BLINK);*/

}

void _led_modeset(struct _led* led, led_mode mode){

    uint8_t oc_mode;

    switch(mode){
        case LED_OFF:
            /* force inactive, 0100 */
            oc_mode = 4;
            break;
        case LED_ON:
            /* force active, 0101 (backwards from datasheet?)*/
            oc_mode = 5;
            break;
        case LED_BLINK:
            /* toggle mode, 0011 */
            oc_mode = 3;
            break;
        default:
            oc_mode = 4;
            break;
    }

    if(led->oc < 3){
        led->tim->CCMR1 &= ~(7 << (4 + (led->oc-1) * 8));
        led->tim->CCMR1 |= oc_mode << (4 + (led->oc-1) * 8);
    }
    else if (led->oc >= 3){
        led->tim->CCMR2 &= ~(7 << (4 + (led->oc-3) * 8));
        led->tim->CCMR2 |= oc_mode << (4 + (led->oc-3) * 8);
    }

    led->mode = mode;
}

void _init_led(
    struct _led*     led,
    GPIO_TypeDef*    port,
    uint8_t          pin,
    TIM_TypeDef*     tim,
    uint8_t          oc,
    led_name         name
){
    led->port = port;
    led->pin = pin;
    led->led = name;
    led->tim = tim;
    led->oc = oc;
    /* attach to timer */
    pin_af_init(led->port, led->pin, 2, PULLUP);
    _led_tim_init(led);
}


void led_init(void){
    debug_led = &_debug_led;
    status_led[0] = &_status_led[0];
    status_led[1] = &_status_led[1];
    _init_led(
        debug_led,
        DEBUG_PORT,
        DEBUG_PIN,
        DEBUG_TIM,
        DEBUG_OC,
        DEBUG_LED
    );

    _init_led(
        status_led[0],
        STATUS_A_PORT,
        STATUS_A_PIN,
        STATUS_A_TIM,
        STATUS_A_OC,
        STATUS_A_LED
    );

    _init_led(
        status_led[1],
        STATUS_B_PORT,
        STATUS_B_PIN,
        STATUS_B_TIM,
        STATUS_B_OC,
        STATUS_B_LED
    );
}

void set_debug_led(led_mode mode){
    _led_modeset(debug_led, mode);
}

void toggle_debug_led(void){
    if(debug_led->mode == LED_ON)
        set_debug_led(LED_OFF);
    else
        set_debug_led(LED_ON);
}
