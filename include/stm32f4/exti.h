/**
 *	Embedded System Course - University of Toulouse 3
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
#ifndef STM32F4_EXTI_H
#define STM32F4_EXTI_H

#include "io.h"

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} exti_t;

// registers
#define EXTI_BASE		0x40013C00

#ifdef __INIT__
volatile exti_t *EXTI = (volatile exti_t*) EXTI_BASE;
#else
extern volatile exti_t *EXTI;
#endif


// interrupts
#define EXTI0_IRQ		6
#define EXTI1_IRQ		7
#define EXTI2_IRQ		8
#define EXTI3_IRQ		9
#define EXTI4_IRQ		10
#define EXTI9_5_IRQ		23
#define EXTI15_10_IRQ	40

#endif	// STM32F4_EXTI_H
