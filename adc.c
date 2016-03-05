#include "adc.h"
#include "pin.h"
#include "rcc.h"

/* XXX This should be broken up with each ADC going with it's own phase.
 * Not entirely sure where to put the measurement ones. Throttle probably
 * could get it's own file, since there is filtering and interrupts that go
 * along with it. Current probably too, if current control is implemented.
 * The motor neutral point is important for sensorless operation, which won't
 * be possible when using sinusoidal control.
 */


static void _adc_init(ADC_TypeDef *ADC){
    /* disable deep-power-down */
    ADC->CR &= ~ADC_CR_DEEPPWD;
    /* turn on ADC voltage regulator */
    ADC->CR |= ADC_CR_ADVREGEN;
    /* wait 10us for it to start up (FIXME: this is clock dependent!)*/
    uint8_t wait_time = 10 * (SystemCoreClock/1000000);
    while(wait_time != 0)
        wait_time--;

    /* start calibration */
    ADC->CR |= ADC_CR_ADCAL;
    /* wait for calibration to complete */
    while(ADC->CR & ADC_CR_ADCAL);

    /* enable ADC and wait for it to be ready */
    while(!(ADC->CR & ADC_CR_ADEN))
        ADC->CR |= ADC_CR_ADEN;
}

static void _init_adc(
    AdcName          name,
    GPIO_TypeDef    *port,
    uint8_t           pin,
    ADC_TypeDef      *adc_n,
    uint8_t            ch
){

    struct AdcCh *a = &_adc[name];
    a->name = name;
    a->port = port;
    a->pin  =  pin;
    a->adc  =  adc_n;
    a->ch   =   ch;

    pin_adc_init(port, pin);
    if(!(a->adc->CR & ADC_CR_ADEN))
        _adc_init(a->adc);

    /* set sample time to 12.5 cycles (156ns @ 80MHz) */
    /* total conversion time = sample time + 12.5 cycles (~300ns) */
    if(ch <= 9)
        a->adc->SMPR1 |= (2 << (3 * ch));
    else
        a->adc->SMPR2 |= (2 << (3 * (ch-10)));

    adc[name] = a;
}

void adc_init(void){

    rcc_adc_enable();

    /* set ADC clock source to HCLK */
    ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

    _init_adc(ADC_VA, VA_PORT, VA_PIN, VA_ADC, VA_ADCIN);
    _init_adc(ADC_VB, VB_PORT, VB_PIN, VB_ADC, VB_ADCIN);
    _init_adc(ADC_VC, VC_PORT, VC_PIN, VC_ADC, VC_ADCIN);

    _init_adc(ADC_IA, IA_PORT, IA_PIN, IA_ADC, IA_ADCIN);
    _init_adc(ADC_IB, IB_PORT, IB_PIN, IB_ADC, IB_ADCIN);

    _init_adc(ADC_VBAT, VBAT_PORT, VBAT_PIN, VBAT_ADC, VBAT_ADCIN);
    _init_adc(ADC_VN, VN_PORT, VN_PIN, VN_ADC, VN_ADCIN);
    _init_adc(ADC_THROT, THROT_PORT, THROT_PIN, THROT_ADC, THROT_ADCIN);

}
