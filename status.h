#ifndef STATUS_H
#define STATUS_H

#include "stm32l4xx.h"
#include "datatypes.h"

/* defines a status/fault code */
static struct _status_state{
    /* fault code */
    fault_code          fault;
    /* latched shutdown (cleared on reset or by PC) */
    uint8_t             latch;
    /* status LED configuration */
    status_led_state    led_state; 
} status_states[5];

volatile struct _status_state* global_status;

uint8_t set_status(struct _status_state state);
void _reset_status_leds(struct status_led_state state);
void _set_status_leds(struct status_led_state state);

#endif
