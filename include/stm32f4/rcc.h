/**
 *	Torpy - grid-follower wheeling and box delivering bot.
 *	Copyright (C) 2021  Université de Toulouse <casse@irit.fr>
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */
#ifndef STM32F4_RCC_H
#define STM32F4_RCC_H

#include "io.h"
#include <stdint.h>

typedef struct {
    volatile uint32_t CR;           // 0x00
    volatile uint32_t PLLCFGR;      // 0x04
    volatile uint32_t CFGR;         // 0x08
    volatile uint32_t CIR;          // 0x0C
    volatile uint32_t AHB1RSTR;     // 0x10
    volatile uint32_t AHB2RSTR;     // 0x14
    volatile uint32_t AHB3RSTR;     // 0x18
    volatile uint32_t RESERVED0[1];
    volatile uint32_t APB1RSTR;     // 0x20
    volatile uint32_t APB2RSTR;     // 0x24
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;      // 0x30
    volatile uint32_t AHB2ENR;      // 0x34
    volatile uint32_t AHB3ENR;      // 0x38
    volatile uint32_t RESERVED2[1];
    volatile uint32_t APB1ENR;      // 0x40
    volatile uint32_t APB2ENR;      // 0x44
    volatile uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;    // 0x50
    volatile uint32_t AHB2LPENR;    // 0x54
    volatile uint32_t AHB3LPENR;    // 0x58
    volatile uint32_t RESERVED4[1];
    volatile uint32_t APB1LPENR;    // 0x60
    volatile uint32_t APB2LPENR;    // 0x64
    volatile uint32_t RESERVED5[2];
    volatile uint32_t BDCR;         // 0x70
    volatile uint32_t CSR;          // 0x74
    volatile uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;        // 0x80
    volatile uint32_t PLLISCFGR;    // 0x84
    volatile uint32_t PLLSAICFGR;   // 0x88
    volatile uint32_t DCKCFGR;      // 0x8C
} rcc_t;


#define RCC_BASE		0x40023800
#ifdef __INIT__
volatile rcc_t *RCC = (volatile rcc_t*) RCC_BASE;
#else
extern volatile rcc_t *RCC;
#endif


#define RCC_HSI				0b00
#define RCC_HSE				0b01
#define RCC_PLL				0b10

#define RCC_HPRE_NODIV		0b0000
#define RCC_PPRE_DIV2		0b100
#define RCC_PPRE_DIV3		0b101
#define RCC_PPRE_DIV4		0b110
#define RCC_PLLP2			0b00
#define RCC_PLLP4			0b01
#define RCC_PLLP6			0b10
#define RCC_PLLP8			0b11

#define RCC_REG(o)		_IOREG(RCC_BASE, o)
#define RCC_CR_HSION		(1 << 0)
#define RCC_CR_HSIRDY		(1 << 1)
#define RCC_CR_HSEON		(1 << 16)
#define RCC_CR_HSERDY		(1 << 17)
#define RCC_CR_PLLON		(1 << 24)
#define RCC_CR_PLLRDY		(1 << 25)
#define RCC_PLLCFGR_M_SET(x, v)		_SET(x, 5, 0, v)
#define RCC_PLLCFGR_N_SET(x, v)		_SET(x, 14, 6, v)
#define RCC_PLLCFGR_P_SET(x, v)		_SET(x, 17, 16, v)
#define RCC_PLLCFGR_SRC_HSI			0
#define RCC_PLLCFGR_SRC_HSE			(1 << 22)
#define RCC_PLLCFGR_Q_SET(x, v)		_SET(x, 27, 24, v)
#define RCC_CFGR_SW_SET(x, v)		_SET(x, 1, 0, v)
#define RCC_CFGR_SWS_GET(x)			_GET(x, 3, 2)
#define RCC_CFGGR_HPRE_SET(x, v)	_SET(x, 7, 4, v)
#define RCC_CFGGR_PPRE1_SET(x, v)	_SET(x, 12, 10, v)
#define RCC_CFGGR_PPRE2_SET(x, v)	_SET(x, 15, 13, v)


// APB1
#define RCC_TIM2EN		(1 << 0)
#define RCC_TIM3EN		(1 << 1)
#define RCC_TIM4EN		(1 << 2)
#define RCC_TIM5EN		(1 << 3)
#define RCC_TIM6EN		(1 << 4)
#define RCC_TIM7EN		(1 << 5)
#define RCC_TIM12EN		(1 << 6)
#define RCC_TIM13EN		(1 << 7)
#define RCC_TIM14EN		(1 << 8)
#define RCC_DACEN		(1 << 29)

// APB2
#define RCC_TIM1EN		(1 << 0)
#define RCC_TIM8EN		(1 << 1)
#define RCC_ADC1EN		(1 << 8)
#define RCC_ADC2EN		(1 << 9)
#define RCC_ADC3EN		(1 << 10)
#define RCC_TIM9EN		(1 << 16)
#define RCC_TIM10EN		(1 << 17)
#define RCC_TIM11EN		(1 << 18)

// AHB1
#define RCC_GPIOAEN		(1 << 0)
#define RCC_GPIOBEN		(1 << 1)
#define RCC_GPIOCEN		(1 << 2)
#define RCC_GPIODEN		(1 << 3)
#define RCC_GPIOEEN		(1 << 4)
#define RCC_GPIOFEN		(1 << 5)
#define RCC_GPIOGEN		(1 << 6)
#define RCC_GPIOHEN		(1 << 7)
#define RCC_GPIOIEN		(1 << 8)

#endif	// STM32F4_RCC_H
