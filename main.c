#include "stm32l4xx.h"
#include "datatypes.h"
#include "led.h"
#include "fet.h"
#include "motor.h"
#include "phase.h"
#include "rcc.h"
#include "util.h"

int main(void){

    /* initialize system clock to 80MHz */
    uw_tick = 0;
    system_clock_config(); 
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000000);

    /* init functions, don't leave here */
    /*led_init();*/
    /***********************************/

    /*led_set_debug(LED_BLINK);*/

    duty_g = 20.0;
    motor_init();

    while(1);

}
