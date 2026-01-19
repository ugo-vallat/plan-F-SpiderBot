#ifndef __STM32F4_TIM_H__
#define __STM32F4_TIM_H__

#include "io.h"
#include <stdint.h>

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR;
} timx_t;



// bases
#define TIM2_BASE	0x40000000
#define TIM3_BASE	0x40000400
#define TIM4_BASE	0x40000800
#define TIM5_BASE	0x40000C00
#define TIM6_BASE	0x40001000
#define TIM7_BASE	0x40001400
#define TIM12_BASE	0x40001800
#define TIM13_BASE	0x40001C00
#define TIM14_BASE	0x40002000
#define TIM1_BASE	0x40010000
#define TIM8_BASE	0x40010400
#define TIM9_BASE	0x40014000
#define TIM10_BASE	0x40014400
#define TIM11_BASE	0x40014800

#ifdef __INIT__
#define INIT_TIM(n) volatile timx_t *TIM##n = (volatile timx_t *)TIM##n##_BASE;
#else
#define INIT_TIM(n) extern volatile timx_t *TIM##n;
#endif


INIT_TIM(1)
INIT_TIM(2)
INIT_TIM(3)
INIT_TIM(4)
INIT_TIM(5)
INIT_TIM(6)
INIT_TIM(7)
INIT_TIM(8)
INIT_TIM(9)
INIT_TIM(10)
INIT_TIM(11)
INIT_TIM(12)
INIT_TIM(13)
INIT_TIM(14)

// TIM_CR1
#define TIM_CKD_CKINT	(0b00 << 8)
#define TIM_CKD_CKINT2	(0b01 << 8)
#define TIM_CKD_CKINT4	(0b10 << 8)
#define TIM_ARPE		(1 << 7)
#define TIM_CMS_MODE0	(0b00 << 5)
#define TIM_CMS_MODE1	(0b01 << 5)
#define TIM_CMS_MODE2	(0b10 << 5)
#define TIM_CMS_MODE3	(0b11 << 5)
#define TIM_DIR_DOWN	(1 << 4)
#define TIM_OPM			(1 << 3)
#define TIM_URS			(1 << 2)
#define TIM_UDIS		(1 << 1)
#define TIM_CEN			(1 << 0)

// TIM_CR2
#define TIM_TI1S				(1 << 7)
#define TIM_MMS_RESET			(0b000 << 4)
#define TIM_MMS_ENABLE			(0b001 << 4)
#define TIM_MMS_UPDATE			(0b010 << 4)
#define TIM_MMS_COMPARE_PULSE	(0b011 << 4)
#define TIM_MMS_COMPARE1		(0b100 << 4)
#define TIM_MMS_COMPARE2		(0b101 << 4)
#define TIM_MMS_COMPARE3		(0b110 << 4)
#define TIM_MMS_COMPARE4		(0b111 << 4)
#define TIM_CCDSS				(1 << 3)

// TIM_SMCR
#define TIM_ETP			(1 << 15)
#define TIM_ECE			(1 << 14)
#define TIM_ETPS_OFF	(0b00 << 12)
#define TIM_ETPS_ON2	(0b01 << 12)
#define TIM_ETPS_ON4	(0b10 << 12)
#define TIM_ETPS_ON8	(0b11 << 12)
#define TIM_ETF_CKINT1	(0b0000 << 8)
#define TIM_ETF_CKINT2	(0b0001 << 8)
#define TIM_ETF_CKINT4	(0b0010 << 8)
#define TIM_ETF_CKINT8	(0b0011 << 8)
#define TIM_ETF_DTS2_6	(0b0100 << 8)
#define TIM_ETF_DTS2_8	(0b0101 << 8)
#define TIM_ETF_DTS4_6	(0b0110 << 8)
#define TIM_ETF_DTS4_8	(0b0111 << 8)
#define TIM_ETF_DTS8_6	(0b1000 << 8)
#define TIM_ETF_DTS8_8	(0b1001 << 8)
#define TIM_ETF_DTS16_5	(0b1010 << 8)
#define TIM_ETF_DTS16_6	(0b1011 << 8)
#define TIM_ETF_DTS16_8	(0b1100 << 8)
#define TIM_ETF_DTS32_5	(0b1101 << 8)
#define TIM_ETF_DTS32_6	(0b1110 << 8)
#define TIM_ETF_DTS32_8	(0b1111 << 8)
#define TIM_MSM			(1 << 7)
#define TIM_TS_ITR0		(0b000 << 4)
#define TIM_TS_ITR1		(0b001 << 4)
#define TIM_TS_ITR2		(0b010 << 4)
#define TIM_TS_ITR3		(0b011 << 4)
#define TIM_TS_TI1F_ED	(0b100 << 4)
#define TIM_TS_TI1FP1	(0b101 << 4)
#define TIM_TS_TI2FP2	(0b110 << 4)
#define TIM_TS_ETRF		(0b111 << 4)
#define TIM_SMS_OFF		(0b000 << 0)
#define TIM_SMS_ENC1	(0b001 << 0)
#define TIM_SMS_ENC2	(0b010 << 0)
#define TIM_SMS_ENC3	(0b011 << 0)
#define TIM_SMS_RESET	(0b100 << 0)
#define TIM_SMS_GATED	(0b101 << 0)
#define TIM_SMS_TRIGGER	(0b110 << 0)
#define TIM_SMS_EXT		(0b111 << 0)

// TIM_DIER
#define TIM_TDE		(1 << 14)
#define TIM_CC4DE	(1 << 13)
#define TIM_CC3DE	(1 << 12)
#define TIM_CC2DE	(1 << 11)
#define TIM_CC1DE	(1 << 10)
#define TIM_UDE		(1 << 9)
#define TIM_TIE		(1 << 6)
#define TIM_CC4IE	(1 << 4)
#define TIM_CC3IE	(1 << 3)
#define TIM_CC2IE	(1 << 2)
#define TIM_CC1IE	(1 << 1)
#define TIM_UIE		(1 << 0)

// TIM_SR
#define TIM_CC4OF	(1 << 12)
#define TIM_CC3OF	(1 << 11)
#define TIM_CC2OF	(1 << 10)
#define TIM_CC1OF	(1 << 9)
#define TIM_TIF		(1 << 6)
#define TIM_CC4IF	(1 << 4)
#define TIM_CC3IF	(1 << 3)
#define TIM_CC2IF	(1 << 2)
#define TIM_CC1IF	(1 << 1)
#define TIM_UIF		(1 << 0)

// TIM_EGR
#define TIM_CC4G	(1 << 4)
#define TIM_CC3G	(1 << 3)
#define TIM_CC2G	(1 << 2)
#define TIM_CC1G	(1 << 1)
#define TIM_UG		(1 << 0)

// CCMR1
#define TIM_OC2CE		(1 << 15)
#define TIM_OC2M_FROZEN	(0b000 << 12)
#define TIM_OC2M_SET	(0b001 << 12)
#define TIM_OC2M_CLR	(0b010 << 12)
#define TIM_OC2M_TOGGLE	(0b011 << 12)
#define TIM_OC2M_PWM1	(0b110 << 12)
#define TIM_OC2M_PWM2	(0b111 << 12)
#define TIM_OC2PE		(1 << 11)
#define TIM_OC2FE		(1 << 10)
#define TIM_CCS2S_OUT	(0b00 << 8)
#define TIM_CCS2S_IN2	(0b01 << 8)
#define TIM_CCS2S_IN1	(0b10 << 8)
#define TIM_CCS2S_TRC	(0b11 << 8)
#define TIM_OC1CE		(1 << 7)
#define TIM_OC1M_FROZEN	(0b000 << 4)
#define TIM_OC1M_SET	(0b001 << 4)
#define TIM_OC1M_CLR	(0b010 << 4)
#define TIM_OC1M_TOGGLE	(0b011 << 4)
#define TIM_OC1M_PWM1	(0b110 << 4)
#define TIM_OC1M_PWM2	(0b111 << 4)
#define TIM_OC1PE		(1 << 3)
#define TIM_OC1FE		(1 << 2)
#define TIM_CCS1S_OUT	(0b00 << 0)
#define TIM_CCS1S_IN1	(0b01 << 8)
#define TIM_CCS1S_IN2	(0b10 << 8)
#define TIM_CCS1S_TRC	(0b11 << 8)

// CCMR2
#define TIM_OC4CE		(1 << 15)
#define TIM_OC4M_FROZEN	(0b000 << 12)
#define TIM_OC4M_SET	(0b001 << 12)
#define TIM_OC4M_CLR	(0b010 << 12)
#define TIM_OC4M_TOGGLE	(0b011 << 12)
#define TIM_OC4M_PWM1	(0b110 << 12)
#define TIM_OC4M_PWM2	(0b111 << 12)
#define TIM_OC4PE		(1 << 11)
#define TIM_OC4FE		(1 << 10)
#define TIM_CCS4S_OUT	(0b00 << 8)
#define TIM_CCS4S_IN3	(0b01 << 8)
#define TIM_CCS4S_IN4	(0b10 << 8)
#define TIM_CCS4S_TRC	(0b11 << 8)
#define TIM_OC3CE		(1 << 7)
#define TIM_OC3M_FROZEN	(0b000 << 4)
#define TIM_OC3M_SET	(0b001 << 4)
#define TIM_OC3M_CLR	(0b010 << 4)
#define TIM_OC3M_TOGGLE	(0b011 << 4)
#define TIM_OC3M_PWM1	(0b110 << 4)
#define TIM_OC3M_PWM2	(0b111 << 4)
#define TIM_OC3PE		(1 << 3)
#define TIM_OC3RE		(1 << 2)
#define TIM_CCS3S_OUT	(0b00 << 0)
#define TIM_CCS3S_IN3	(0b01 << 8)
#define TIM_CCS3S_IN4	(0b10 << 8)
#define TIM_CCS3S_TRC	(0b11 << 8)

// CCER
#define TIM_CC4NP		(1 << 15)
#define TIM_CC4P		(1 << 13)
#define TIM_CC4E		(1 << 12)
#define TIM_CC3NP		(1 << 11)
#define TIM_CC3P		(1 << 9)
#define TIM_CC3E		(1 << 8)
#define TIM_CC2NP		(1 << 7)
#define TIM_CC2P		(1 << 5)
#define TIM_CC2E		(1 << 4)
#define TIM_CC1NP		(1 << 3)
#define TIM_CC1P		(1 << 1)
#define TIM_CC1E		(1 << 0)


// interrupts
#define TIM1_BRK_IRQ	24
#define TIM1_UP_IRQ		25
#define TIM1_TRG_COM_IRQ	26
#define TIM1_CC_IRQ		27
#define TIM2_IRQ		28
#define TIM3_IRQ		29
#define TIM4_IRQ		30
#define TIM5_IRQ		50
#define TIM6_IRQ		54
#define TIM7_IRQ		55
#define TIM8_BRK_IRQ	43
#define TIM8_UO_IRQ		44
#define TIM8_TRG_COM_IRQ	45
#define TIM8_CC_IRQ		46
#define TIM9_IRQ		24
#define TIM10_IRQ		25
#define TIM11_IRQ		26
#define TIM12_IRQ		43
#define TIM13_IRQ		44
#define TIM14_IRQ		45

#endif	// __STM32F4_TIM_H__
