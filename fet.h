#include "datatypes.h"

struct _fet{
    fet_name            name;
    GPIO_TypeDef*       port;
    uint8_t             pin;
    TIM_TypeDef*        tim;
    uint8_t             oc;
    /* inverse- negative side, ie CH3N */
    uint8_t             inv;
    /* current polarity, changes when phase is low */
    uint8_t             polarity;
    /* 
    uint8_t             enabled;
}
