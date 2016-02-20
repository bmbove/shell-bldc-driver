#ifndef PIN_H
#define PIN_H

#include "datatypes.h"

#define PULLDOWN        0
#define PULLUP          1
#define NOPULL          2

#define OUTPUT          0
#define INPUT           1


void _set_mode(GPIO_TypeDef *port, uint8_t pin, pin_mode mode);
void _set_pull(GPIO_TypeDef *port, uint8_t pin, uint8_t pull);
void _pin_init(GPIO_TypeDef *port, uint8_t pin, pin_mode mode, uint8_t pull);

void pin_out_init(GPIO_TypeDef *port, uint8_t pin, uint8_t pull);
void pin_in_init(GPIO_TypeDef *port, uint8_t pin, uint8_t pull);
void pin_af_init(GPIO_TypeDef *port, uint8_t pin, uint8_t af, uint8_t pull);
void pin_adc_init(GPIO_TypeDef *port, uint8_t pin);

#endif
