#ifndef DATATYPES_H
#define DATATYPES_H

typedef enum{
    PULLUP,
    PULLDOWN,
    NOPULL

} PullDir;

typedef enum {
	FAULT_CODE_NONE = 0,
	FAULT_CODE_OVER_VOLTAGE,
	FAULT_CODE_UNDER_VOLTAGE,
	FAULT_CODE_OVER_CURRENT,
	FAULT_CODE_OVER_TEMP_MOTOR
} FaultCode;

typedef enum{
    DEBUG_LED,
    STATUS_A_LED,
    STATUS_B_LED
} LedName;

typedef enum{
    LED_OFF,
    LED_ON,
    LED_BLINK
} LedMode;

typedef enum {
    /*Aoff, B off*/
    NONE=0,
    /*A solid, B off*/
    A,
    /*A solid, B solid*/
    A_B,
    /*Aoff, B solid*/
    B,
    /*A blink, B off*/
    ABLINK,
    /*Aoff, B blink*/
    BBLINK,
    /*A solid, B blink*/
    A_BBLINK,
    /*A blink, B solid*/
    ABLINK_B,
    /*A blink, B blink*/
    ABLINK_BBLINK,
    /* A blink, B blink out of phase */
    ABLINK_BBLINK_OOP
} StatusLedState;

typedef enum {
    INPUT,
    OUTPUT,
    ADC,
    AF
} PinMode;

typedef enum{
    AP,
    BP,
    CP
} PhaseName;

typedef enum{
    NORMAL,
    INVERTED
} FetPolarity;

//typedef enum{
    //HIGH,
    //LOW,
    //FLOATING
//} phase_status;

//typedef enum{
    //VA,
    //VB,
    //VC
//} phase_voltage;

//typedef enum{
    //IA,
    //IB,
    //IC
//} phase_current;

typedef enum{
    AH,
    AL,
    BH,
    BL,
    CH,
    CL
} FetName;

typedef enum{
    COMP, /* Complementary, high and low on. duty ratio applies to high side */
    HIGH, /* High side on, low side disabled */
    LOW,  /* Low side on, high side disabled */
    OFF   /* Both sides are off */
} PhaseMode;

typedef enum{
    ADC_VA,
    ADC_VB,
    ADC_VC,
    ADC_IA,
    ADC_IB,
    ADC_THROT,
    ADC_VN,
    ADC_VBAT
} AdcName;

#endif
