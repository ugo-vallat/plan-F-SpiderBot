#ifndef __STM32F4_GPIO_H__
#define __STM32F4_GPIO_H__

#include "io.h"


typedef struct {
    volatile uint32_t REG;
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} gpio_t;

#define GPIO_INST(n)	((n) >> 4)		
#define GPIO_NUM(n)		((n) & 0b00001111)
#define GPIO_HNUM(n)	((n) & 0b00000111)
#define GPIO_HOFF(n)	((GPIO_NUM(n) >> 3) * 4)

#define GPIO_START		0x40020000
#define GPIO_SIZE		0x400
#define GPIO_BASE(n)	(GPIO_START + (n) * GPIO_SIZE)

#ifdef __INIT__
#define INIT_GPIO(n, l) volatile gpio_t *GPIO##l = (volatile gpio_t *)GPIO_BASE(n);
#else
#define INIT_GPIO(n, l) extern volatile gpio_t *GPIO##l;
#endif

INIT_GPIO(0,A)
INIT_GPIO(1,B)
INIT_GPIO(2,C)
INIT_GPIO(3,D)
INIT_GPIO(4,E)
INIT_GPIO(5,F)
INIT_GPIO(6,G)
INIT_GPIO(7,H)
INIT_GPIO(8,I)
INIT_GPIO(9,J)
INIT_GPIO(10,K)

#define GPIO_MODER_IN	0b00
#define GPIO_MODER_OUT	0b01
#define GPIO_MODER_ALT	0b10
#define GPIO_MODER_ANA	0b11

#define GPIO_OSPEEDR_LO	0b00
#define GPIO_OSPEEDR_ME	0b01
#define GPIO_OSPEEDR_HI	0b10
#define GPIO_OSPEEDR_VH	0b11

#define GPIO_PUPDR_NO	0b00
#define GPIO_PUPDR_PU	0b01
#define GPIO_PUPDR_PD	0b10


#endif	// __STM32F4_GPIO_H__
