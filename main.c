#include "stm32l4xx.h"
#include "datatypes.h"
#include "led.h"
#include "rcc.h"
#include "util.h"

int main(void){

    /* initialize system clock to 80MHz */
    uw_tick = 0;
    system_clock_config(); 
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000000);

    /* init functions, don't leave here */
    led_init();
    /***********************************/

    set_debug_led(LED_BLINK);

    while(1);
}
