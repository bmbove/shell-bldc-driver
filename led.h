#ifndef LED_H
#define LED_H
#include "stm32l4xx.h"

/* timer runs at 10kHz */
#define LED_TIM_CLK     10000
/* timer overflow period 4Hz */
#define LED_CLK         4

#define DEBUG_PIN               9
#define DEBUG_PORT              GPIOB
#define DEBUG_TIM               TIM4
#define DEBUG_OC                4

#define STATUS_A_PIN           0
#define STATUS_A_PORT          GPIOB
#define STATUS_A_TIM           TIM3
#define STATUS_A_OC            3

#define STATUS_B_PIN           1
#define STATUS_B_PORT          GPIOB
#define STATUS_B_TIM           TIM3
#define STATUS_B_OC            4

struct Led{
    GPIO_TypeDef*       port;
    uint8_t             pin;
    LedName             led;
    LedMode             mode;
    uint8_t             oc;
    TIM_TypeDef*        tim;
} _debug_led, _status_led[2];

struct Led *debug_led;
struct Led *status_led[2];

void _init_led(
    struct Led      *led,
    GPIO_TypeDef    *port,
    uint8_t          pin,
    TIM_TypeDef     *tim,
    uint8_t          oc,
    LedName          name
);

void _led_tim_init(struct Led* led);
void _led_modeset(struct Led* led, LedMode mode);
void led_init(void);
void led_set_debug(LedMode mode);
void led_toggle_debug(void);

#endif
