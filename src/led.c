
#include <led.h>
#include <stm32f4/rcc.h>


void init_module_led(void) {
    /* Board leds */
    RCC->AHB1ENR |= RCC_GPIODEN;
    INIT_LED(GPIOD, LED_G_PIN);
    INIT_LED(GPIOD, LED_O_PIN);
    INIT_LED(GPIOD, LED_R_PIN);
    INIT_LED(GPIOD, LED_B_PIN);


}