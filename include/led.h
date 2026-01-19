#ifndef __LED_H__
#define __LED_H__

#include <stm32f4/io.h>
#include <stm32f4/gpio.h>

#define INIT_LED(gpio, n)                                                       \
    do {                                                                        \
        (gpio)->MODER = REP_BITS((gpio)->MODER, (n)*2, 2, GPIO_MODER_OUT);      \
        (gpio)->OTYPER &= ~(1U << (n));                                         \
    } while (0)

#define TEST_INIT INIT_LED(TEST,1)

#define SWITCH_LED(gpio, n)             \
    do {                                \
        (gpio)->ODR ^= (1 << n);        \
    } while (0)                         \



#endif // __LED_H__