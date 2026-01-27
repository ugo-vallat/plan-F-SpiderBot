#ifndef __LED_H__
#define __LED_H__

#include <stm32f4/io.h>
#include <stm32f4/gpio.h>

#define INIT_LED(gpio, n)                                                       \
    do {                                                                        \
        (gpio)->MODER = REP_BITS((gpio)->MODER, (n)*2, 2, GPIO_MODER_OUT);      \
        (gpio)->OTYPER &= ~(1U << (n));                                         \
    } while (0)

#define SWITCH_LED(gpio, n)             \
    do {                                \
        (gpio)->ODR ^= (1 << n);        \
    } while (0)                         \


#define LED_G_PIN   12
#define LED_O_PIN   13
#define LED_R_PIN   14
#define LED_B_PIN   15

#define SWITCH_G_LED() SWITCH_LED(GPIOD, LED_G_PIN)
#define SWITCH_O_LED() SWITCH_LED(GPIOD, LED_O_PIN)
#define SWITCH_R_LED() SWITCH_LED(GPIOD, LED_R_PIN)
#define SWITCH_B_LED() SWITCH_LED(GPIOD, LED_B_PIN)

void init_module_led(void);

#endif // __LED_H__