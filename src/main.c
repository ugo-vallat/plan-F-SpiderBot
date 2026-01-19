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
    init_log();
    init_time();
    init_bluetooth();
    init_servomotor();

}

void main_loop(void) {
    while(true) {
    }
}

int main(void) {
    init();

    main_loop();

    return 0;
}