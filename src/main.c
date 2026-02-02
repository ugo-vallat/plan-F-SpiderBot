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
#include <control.h>
#include <battery.h>

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
    init_module_control();
    init_module_battery();
}

void main_loop(void) {
    while(true) {
        PRINTL("main loop\n");
        SWITCH_B_LED();
        bluetooth_receive_cmd();
        ctrl_update_robot();
        sm_move(1000000);
        get_battery_percentage();
    }
}

int main(void) {
    init();

    main_loop();

    return 0;
}