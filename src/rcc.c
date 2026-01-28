#include "log.h"
#include <stm32f4/rcc.h>

void enable_gpio(volatile gpio_t * gpio) {
    if (GPIOA == gpio) {
        RCC->AHB1ENR |= RCC_GPIOAEN;
        return;
    }
    if (GPIOB == gpio) {
        RCC->AHB1ENR |= RCC_GPIOBEN;
        return;
    }
    if (GPIOC == gpio) {
        RCC->AHB1ENR |= RCC_GPIOCEN;
        return;
    }
    if (GPIOD == gpio) {
        RCC->AHB1ENR |= RCC_GPIODEN;
        return;
    }
    if (GPIOE == gpio) {
        RCC->AHB1ENR |= RCC_GPIOEEN;
        return;
    }
    if (GPIOF == gpio) {
        RCC->AHB1ENR |= RCC_GPIOFEN;
        return;
    }
    if (GPIOG == gpio) {
        RCC->AHB1ENR |= RCC_GPIOGEN;
        return;
    }
    if (GPIOH == gpio) {
        RCC->AHB1ENR |= RCC_GPIOHEN;
        return;
    }
    if (GPIOI == gpio) {
        RCC->AHB1ENR |= RCC_GPIOIEN;
        return;
    }

    WARNL("Undefined GPIO");
    return;
}



void enable_timx(volatile timx_t * tim) {
    if (TIM2 == tim) {
        RCC->APB1ENR |= RCC_TIM2EN;
        return;
    }
    if (TIM3 == tim) {
        RCC->APB1ENR |= RCC_TIM3EN;
        return;
    }
    if (TIM4 == tim) {
        RCC->APB1ENR |= RCC_TIM4EN;
        return;
    }
    if (TIM5 == tim) {
        RCC->APB1ENR |= RCC_TIM5EN;
        return;
    }
    if (TIM6 == tim) {
        RCC->APB1ENR |= RCC_TIM6EN;
        return;
    }
    if (TIM7 == tim) {
        RCC->APB1ENR |= RCC_TIM7EN;
        return;
    }
    if (TIM12 == tim) {
        RCC->APB1ENR |= RCC_TIM12EN;
        return;
    }
    if (TIM13 == tim) {
        RCC->APB1ENR |= RCC_TIM13EN;
        return;
    }
    if (TIM14 == tim) {
        RCC->APB1ENR |= RCC_TIM14EN;
        return;
    }
    if (TIM1 == tim) {
        RCC->APB2ENR |= RCC_TIM1EN;
        return; 
    }
    if (TIM8 == tim) {
        RCC->APB2ENR |= RCC_TIM8EN;
        return; 
    }
    if (TIM9 == tim) {
        RCC->APB2ENR |= RCC_TIM9EN;
        return; 
    }
    if (TIM10 == tim) {
        RCC->APB2ENR |= RCC_TIM10EN;
        return; 
    }
    if (TIM11 == tim) {
        RCC->APB2ENR |= RCC_TIM11EN;
        return; 
    }
    WARNL("Undefined TIMx");
    return;
}

void enable_adcx(volatile adcx_t * adc) {
    if(ADC1 == adc) {
        RCC->APB2ENR |= RCC_ADC1EN;
        return;
    }
    if(ADC2 == adc) {
        RCC->APB2ENR |= RCC_ADC2EN;
        return;
    }
    if(ADC3 == adc) {
        RCC->APB2ENR |= RCC_ADC3EN;
        return;
    }
    WARNL("Undefined ADCx");
    return;
}







