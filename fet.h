#ifndef FET_H
#define FET_H

#include "stm32l4xx.h"
#include "datatypes.h"

/* Pre-scaler of 1 gives ARR register gives almost 12-bit resolution
 * on timer/duty cycle at 10kHz PWM (20kHz counting) and 11-bit resolution at
 * 20kHz PWM 
 */
#define FET_TIM_FREQ    80000000
/* PWM frequency = fet_freq/2  (counts up then down) */
#define FET_FREQ        20000

#define FET_TIM         TIM1
#define FET_AF          2

#define AH_PIN          8
#define BH_PIN          9
#define CH_PIN          10
#define AL_PIN          13
#define BL_PIN          14
#define CL_PIN          15

#define AH_PORT         GPIOA
#define BH_PORT         GPIOA
#define CH_PORT         GPIOA
#define AL_PORT         GPIOB
#define BL_PORT         GPIOB
#define CL_PORT         GPIOB

#define A_OC            1
#define B_OC            2
#define C_OC            3


struct Fet{
    FetName             name;
    GPIO_TypeDef*       port;
    uint8_t             pin;
    uint8_t             enabled; 
    /* output compare # */
    uint8_t             oc;
    /* inverse- negative side, ie CH3N */
    uint8_t             inv;
} _fet[6];

struct Fet *fet[6];

void _fet_TIM_init(void);
void _init_fet(
    struct Fet     *fet,
    FetName         name,
    GPIO_TypeDef*   port,
    uint8_t         pin,
    uint8_t         oc,
    uint8_t         inv
);

void fet_init(void);
void fet_update(void);
void fet_disable(struct Fet* fet);
void fet_enable(struct Fet* fet);
void fet_set_duty(struct Fet* fet, float duty);
void fet_set_polarity(struct Fet* fet, FetPolarity polarity);
#endif
