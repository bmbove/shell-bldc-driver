#include "stm32l4xx.h"
#include "datatypes.h"
#include "pin.h"
#include "rcc.h"
#include "util.h"

void _set_mode(GPIO_TypeDef *port, uint8_t pin, pin_mode mode){

	switch(mode){
		case INPUT:
			port->MODER &= ~_BV(2 * pin);
			port->MODER &= ~_BV((2 * pin) + 1);
			break;
		case OUTPUT:
			port->MODER |= _BV(2 * pin);
			port->MODER &= ~_BV((2 * pin) + 1);
			break;
		case AF:
			port->MODER &= ~_BV(2 * pin);
			port->MODER |= _BV((2 * pin) + 1);
			break;
		case ADC:
			port->MODER |= _BV(2 * pin);
			port->MODER |= _BV((2 * pin) + 1);
			break;
		default:
			port->MODER &= ~_BV(2 * pin);
			port->MODER &= ~_BV((2 * pin) + 1);
			break;
	}

}

void _set_pull(GPIO_TypeDef *port, uint8_t pin, uint8_t pull){

	switch(pull){
		case PULLDOWN:
            port->PUPDR &= ~_BV(2 * pin);
            port->PUPDR |= _BV((2 * pin) + 1);
            break;
        case PULLUP:
            port->PUPDR |= _BV(2 * pin);
            port->PUPDR &= ~_BV((2 * pin) + 1);
            break;
        case NOPULL:
            port->PUPDR &= ~_BV(2 * pin);
            port->PUPDR &= ~_BV((2 * pin) + 1);
            break;
        default:
            port->PUPDR &= ~_BV(2 * pin);
            port->PUPDR &= ~_BV((2 * pin) + 1);
            break;
	}
}

void _pin_init(GPIO_TypeDef *port, uint8_t pin, pin_mode mode, uint8_t pull){
	/* enable peripheral */
	rcc_gpio_enable(port);
	/* set mode */
	_set_mode(port, pin, mode);
	/* set pin direction */
	_set_pull(port, pin, pull);
	/* set port speed to fast.. for some reason */
	port->OSPEEDR |= _BV((2 * pin) + 1);
}

void pin_out_init(GPIO_TypeDef *port, uint8_t pin, uint8_t pull){
	_pin_init(port, pin, OUTPUT, pull);
}

void pin_in_init(GPIO_TypeDef *port, uint8_t pin, uint8_t pull){
	_pin_init(port, pin, INPUT, pull);
}

void pin_af_init(GPIO_TypeDef *port, uint8_t pin, uint8_t af, uint8_t pull){
	_pin_init(port, pin, AF, pull);

	if(pin < 8)
		port->AFR[0] |= (uint32_t)(af << (4 * pin));
	if(pin > 8)
		port->AFR[1] |= (uint32_t)(af << (4 * (pin - 8)));
}

void pin_adc_init(GPIO_TypeDef *port, uint8_t pin){
	_pin_init(port, pin, ADC, NOPULL);
	port->ASCR |= _BV(pin);
}
