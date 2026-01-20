/**
 * Torpy - grid-follower wheeling and box delivering bot.
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
#ifndef STM32F4_DAC_H
#define STM32F4_DAC_H

#include "io.h"
#include <stdint.h>

typedef struct{
    volatile uint32_t CR;       //0x00
    volatile uint32_t SWTRIGR;  //0x04
    volatile uint32_t DHR12R1;  //0x08
    volatile uint32_t DHR12L1;  //0x0c
    volatile uint32_t DHR8R1;   //0x10
    volatile uint32_t DHR12R2;  //0x14
    volatile uint32_t DHR12L2;  //0x18
    volatile uint32_t DHR8R2;   //0x1C
    volatile uint32_t DHR12RD;  //0x20
    volatile uint32_t DHR12LD;  //0x24
    volatile uint32_t DHR8RD;   //0x28
    volatile uint32_t DOR1;     //0x2C
    volatile uint32_t DOR2;     //0x30
    volatile uint32_t SR;       //0x34

} dac_t;

#define DAC_BASE	0x40007400

#define DAC ((volatile dac_t*) DAC_BASE)

#define DAC_NOWAVE		0b00
#define DAC_NOISE		0b01
#define DAC_TRIANGLE	0b10
#define DAC_TSEL_TIM6	0b000
#define DAC_TSEL_TIM8	0b001
#define DAC_TSEL_TIM7	0b010
#define DAC_TSEL_TIM5	0b011
#define DAC_TTSEL_IM2	0b100
#define DAC_TSEL_TIM4	0b101
#define DAC_TSEL_EXTI9	0b110
#define DAC_TSEL_SWTRIG	0b111
#define DAC_TEN2		(1 << 18)
#define DAC_BOFF2		(1 << 17)
#define DAC_EN2			(1 << 16)
#define DAC_TEN1		(1 << 2)
#define DAC_BOFF1		(1 << 1)
#define DAC_EN1			(1 << 0)

#define DAC_SWTRIG2		(1 << 1)
#define DAC_SWTRIG1		(1 << 0)

#define DAC_DMAUDR2		(1 << 29)

#endif	// STM32F4_GPIO_H
