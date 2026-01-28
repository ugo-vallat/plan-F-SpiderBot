
#include <led.h>
#include <stm32f4/rcc.h>
#include <log.h>


void init_module_led(void) {
    PRINTL("[%s] ... ", __func__);
    /* Board leds */
    enable_gpio(GPIOD);
    INIT_LED(GPIOD, LED_G_PIN);
    INIT_LED(GPIOD, LED_O_PIN);
    INIT_LED(GPIOD, LED_R_PIN);
    INIT_LED(GPIOD, LED_B_PIN);
    PRINTL("OK\n");
}