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
    RCC->AHB1ENR |= RCC_GPIOCEN;
    RCC->AHB1ENR |= RCC_GPIODEN;
    INIT_LED(GPIOC, 0);
    INIT_LED(GPIOD, 14);
    // (*(volatile uint32_t *)((0x40020000 + (3) * 0x400) + 0x00)) = REP_BITS((*(volatile uint32_t *)((0x40020000 + (3) * 0x400) + 0x00)), 14*2, 2, GPIO_MODER_OUT);
    // (*(volatile uint32_t *)((0x40020000 + (3) * 0x400) + 0x04)) &= ~(1<<14);
}

void main_loop(void) {
    while(true) {
        PRINTL("main loop\n");
        sm_move(10000000, 1);
        SWITCH_LED(GPIOC, 0);
        SWITCH_LED(GPIOD, 14);
        // (*(volatile uint32_t *)((0x40020000 + (3) * 0x400) + 0x14)) ^= (1 << 14);
    }
}

int main(void) {
    init();

    main_loop();

    return 0;
}