#include <stm32f4/adc.h>
#include <stm32f4/dac.h>
#include <stm32f4/exti.h>
#include <stm32f4/gpio.h>
#include <stm32f4/nvic.h>
#include <stm32f4/rcc.h>
#include <stm32f4/syscfg.h>
#include <stm32f4/tim.h>

#include <tinyprintf.h>
#include <time.h>
#include <log.h>
#include <bluetooth.h>
#include <led.h>
#include <servomotor.h>

/**
 * @brief Call init function of each module
 * 
 */
void init(void) {
    init_module_log();
    init_module_time();
    init_module_led();
    init_module_bluetooth();
    init_module_servomotor();
}

void main_loop(void) {
    while(true) {
        PRINTL("main loop\n");
        SWITCH_B_LED();
        sm_set_next_move(SM_FORWARD);
        sm_move(10000000);
    }
}

int main(void) {
    init();

    main_loop();

    return 0;
}