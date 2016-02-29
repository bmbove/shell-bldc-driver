#ifndef ADC_H
#define ADC_H

#include "stm32l4xx.h"
#include "datatypes.h"

/* use this ADC for voltage and current */
#define PWR_ADC         ADC3

/* ADC3 Fast Channels */
#define VA_PIN          0
#define VA_PORT         GPIOC
#define VA_ADC          ADC3
#define VA_ADCIN        1

#define VB_PIN          1
#define VB_PORT         GPIOC 
#define VB_ADC          ADC3
#define VB_ADCIN        2

#define VC_PIN          2
#define VC_PORT         GPIOC 
#define VC_ADC          ADC3
#define VC_ADCIN        3

#define IA_PIN          3
#define IA_PORT         GPIOC 
#define IA_ADC          ADC3
#define IA_ADCIN        4

#define IB_PIN          0
#define IB_PORT         GPIOA
#define IB_ADC          ADC3
#define IB_ADCIN        5

/* ADC2 Slow Channels */
#define THROT_PIN       4
#define THROT_PORT      GPIOA 
#define THROT_ADC       ADC2
#define THROT_ADCIN     9

#define VN_PIN          7
#define VN_PORT         GPIOA
#define VN_ADC          ADC2
#define VN_ADCIN        12

#define VBAT_PIN        6
#define VBAT_PORT       GPIOA
#define VBAT_ADC        ADC2
#define VBAT_ADCIN      11

struct AdcCh{
    AdcName          name;
    GPIO_TypeDef    *port;
    uint8_t           pin;
    ADC_TypeDef      *adc;
    uint8_t            ch;
} _vp_adc[3], _ip_adc[2], _vbat_adc, _throt_adc, _vn_adc;

/* phase voltages */
struct AdcCh *vp_adc[3];
/* phase currents */
struct AdcCh *ip_adc[2];
/* other stuff */
struct AdcCh *vbat_adc, *throt_adc, *vn_adc;

void _init_adc(
    struct AdcCh     *adc_ch,
    AdcName          name,
    GPIO_TypeDef    *port,
    uint8_t           pin,
    ADC_TypeDef      *adc,
    uint8_t            ch
);

void adc_init(void);

#endif
