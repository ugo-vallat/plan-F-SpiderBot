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

#define GPIO_PTR(n)   ((volatile gpio_t *)GPIO_BASE(n))

#define GPIOA   GPIO_PTR(0)
#define GPIOB   GPIO_PTR(1)
#define GPIOC   GPIO_PTR(2)
#define GPIOD   GPIO_PTR(3)
#define GPIOE   GPIO_PTR(4)
#define GPIOF   GPIO_PTR(5)
#define GPIOG   GPIO_PTR(6)
#define GPIOH   GPIO_PTR(7)
#define GPIOI   GPIO_PTR(8)
#define GPIOJ   GPIO_PTR(9)
#define GPIOK   GPIO_PTR(10)



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
