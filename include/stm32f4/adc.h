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
    volatile uint32_t SR;       // 0x00
    volatile uint32_t CR1;      // 0x04
    volatile uint32_t CR2;      // 0x08
    volatile uint32_t SMPR1;    // 0x0C
    volatile uint32_t SMPR2;    // 0x10
    volatile uint32_t JOFR1;    // 0x14
    volatile uint32_t JOFR2;    // 0x18
    volatile uint32_t JOFR3;    // 0x1C
    volatile uint32_t JOFR4;    // 0x20
    volatile uint32_t HTR;      // 0x24
    volatile uint32_t LTR;      // 0x28
    volatile uint32_t SQR1;     // 0x2C
    volatile uint32_t SQR2;     // 0x30
    volatile uint32_t SQR3;     // 0x34
    volatile uint32_t JSQR;     // 0x38
    volatile uint32_t JDR1;     // 0x3C
    volatile uint32_t JDR2;     // 0x40
    volatile uint32_t JDR3;     // 0x44
    volatile uint32_t JDR;      // 0x48
    volatile uint32_t DR;       // 0x4C

} adcx_t ;

#define ADC1_BASE	0x40012000
#define ADC2_BASE	0x40012100
#define ADC3_BASE	0x40012200
#define ADCC_BASE	0x40012300

#define ADC_PTR(n)  ((volatile adcx_t *)ADC##n##_BASE)

#define ADC1    ADC_PTR(1)
#define ADC2    ADC_PTR(2)
#define ADC3    ADC_PTR(3)

#define ADC_JEOC	(1 << 2)
#define ADC_EOC		(1 << 1)

#define ADC_OVERIE	(1 << 26)
#define ADC_12b		0b00
#define ADC_10b		0b01
#define ADC_8b		0b10
#define ADC_6b		0b11
#define ADC_JDISCEN	(1 << 12)
#define ADC_DISCEN	(1 << 11)
#define ADC_SCAN	(1 << 8)
#define ADC_EOCIE	(1 << 5)

#define ADC_SWSTART	(1 << 30)
#define ADC_EXTEN_dis		(0b00 << 28)
#define ADC_EXTEN_rise		(0b01 << 28)
#define ADC_EXTEN_fall		(0b10 << 28)
#define ADC_EXTEN_both		(0b11 << 28)
#define ADC_EXTSEL_T1CC1	(0b0000 << 24)
#define ADC_EXTSEL_T1CC2	(0b0001 << 24)
#define ADC_EXTSEL_T1CC3	(0b0010 << 24)
#define ADC_EXTSEL_T2CC2	(0b0011 << 24)
#define ADC_EXTSEL_T2CC3	(0b0100 << 24)
#define ADC_EXTSEL_T2CC4	(0b0101 << 24)
#define ADC_EXTSEL_T2TRGO	(0b0110 << 24)
#define ADC_EXTSEL_T3CC1	(0b0111 << 24)
#define ADC_EXTSEL_T3TRGO	(0b1000 << 24)
#define ADC_EXTSEL_T4CC4	(0b1001 << 24)
#define ADC_EXTSEL_T5CC1	(0b1010 << 24)
#define ADC_EXTSEL_T5CC2	(0b1011 << 24)
#define ADC_EXTSEL_T5CC3	(0b1100 << 24)
#define ADC_EXTSEL_T8CC1	(0b1101 << 24)
#define ADC_EXTSEL_T8TRGO	(0b1110 << 24)
#define ADC_EXTSEL_EXTI11	(0b1111 << 24)
#define ADC_JWSTART (1 << 22)
#define ADC_EOCS	(1 << 10)
#define ADC_CONT	(1 << 1)
#define ADC_ADON	(1 << 0)

#define ADC_CSR		ADCC_REG(0x00)
#define ADC_CCR		ADCC_REG(0x04)
#define	ADC_DIV2	(0b00 << 16)
#define ADC_DIV4	(0b01 << 16)
#define ADC_DIV6	(0b10 << 16)
#define ADC_DIV8	(0b11 << 16)
#define ADC_CDR		ADCC_REG(0x08)

#define ADC_IRQ		18

#endif	// STM32F4_GPIO_H
