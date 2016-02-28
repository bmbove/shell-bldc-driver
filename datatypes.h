#ifndef DATATYPES_H
#define DATATYPES_H

typedef enum {
	FAULT_CODE_NONE = 0,
	FAULT_CODE_OVER_VOLTAGE,
	FAULT_CODE_UNDER_VOLTAGE,
	FAULT_CODE_OVER_CURRENT,
	FAULT_CODE_OVER_TEMP_MOTOR
} fault_code;

typedef enum{
    DEBUG_LED,
    STATUS_A_LED,
    STATUS_B_LED
} led_name;

typedef enum{
    LED_OFF,
    LED_ON,
    LED_BLINK
} led_mode;

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
} status_led_state;

typedef enum {
    INPUT,
    OUTPUT,
    ADC,
    AF
} pin_mode;

typedef enum{
    A_PHASE = 0,
    B_PHASE = 1,
    C_PHASE = 2
} motor_phase_name;

typedef enum{
    HIGH,
    LOW,
    FLOATING
} phase_status;

typedef enum{
    VA,
    VB,
    VC
} phase_voltage;

typedef enum{
    IA,
    IB,
    IC
} phase_current;

typedef enum{
    AH,
    AL,
    BH,
    BL,
    CH,
    CL
} fet_name;

#endif
