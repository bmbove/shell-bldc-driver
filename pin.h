#ifndef PIN_H
#define PIN_H

#include "datatypes.h"

void _set_mode(GPIO_TypeDef *port, uint8_t pin, PinMode mode);
void _set_pull(GPIO_TypeDef *port, uint8_t pin, PullDir pull);
void _pin_init(GPIO_TypeDef *port, uint8_t pin, PinMode mode, PullDir pull);

void pin_out_init(GPIO_TypeDef *port, uint8_t pin, PullDir pull);
void pin_in_init(GPIO_TypeDef *port, uint8_t pin, PullDir pull);
void pin_af_init(GPIO_TypeDef *port, uint8_t pin, uint8_t af, PullDir pull);
void pin_adc_init(GPIO_TypeDef *port, uint8_t pin);

#endif
