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


void _adc_init(ADC_TypeDef *ADC){
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

void _init_adc(
    struct AdcCh  *adc_ch,
    AdcName          name,
    GPIO_TypeDef    *port,
    uint8_t           pin,
    ADC_TypeDef      *adc,
    uint8_t            ch
){

    adc_ch->name = name;
    adc_ch->port = port;
    adc_ch->pin  =  pin;
    adc_ch->adc  =  adc;
    adc_ch->ch   =   ch;

    pin_adc_init(port, pin);
    if(!(adc_ch->adc->CR & ADC_CR_ADEN))
        _adc_init(adc_ch->adc);

    /* set sample time to 12.5 cycles (156ns @ 80MHz) */
    /* total conversion time = sample time + 12.5 cycles (~300ns) */
    if(ch <= 9)
        adc_ch->adc->SMPR1 |= (2 << (3 * ch));
    else
        adc_ch->adc->SMPR2 |= (2 << (3 * (ch-10)));
}

void adc_init(void){

    rcc_adc_enable();
    for(uint8_t i=0; i < 3; i++)
        vp_adc[i] = &_vp_adc[i];

    for(uint8_t i=0; i < 2; i++)
        ip_adc[i] = &_ip_adc[i];

    vbat_adc = &_vbat_adc;
    throt_adc = &_throt_adc;
    vn_adc = &_vn_adc;

    /* set ADC clock source to HCLK */
    ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

    _init_adc(vp_adc[0], ADC_VA, VA_PORT, VA_PIN, VA_ADC, VA_ADCIN);
    _init_adc(vp_adc[1], ADC_VB, VB_PORT, VB_PIN, VB_ADC, VB_ADCIN);
    _init_adc(vp_adc[2], ADC_VC, VC_PORT, VC_PIN, VC_ADC, VC_ADCIN);

    _init_adc(ip_adc[0], ADC_IA, IA_PORT, IA_PIN, IA_ADC, IA_ADCIN);
    _init_adc(ip_adc[1], ADC_IB, IB_PORT, IB_PIN, IB_ADC, IB_ADCIN);

    _init_adc(vbat_adc, ADC_VBAT, VBAT_PORT, VBAT_PIN, VBAT_ADC, VBAT_ADCIN);
    _init_adc(vn_adc, ADC_VN, VN_PORT, VN_PIN, VN_ADC, VN_ADCIN);
    _init_adc(throt_adc, ADC_THROT, THROT_PORT, THROT_PIN, THROT_ADC, THROT_ADCIN);
    

}
