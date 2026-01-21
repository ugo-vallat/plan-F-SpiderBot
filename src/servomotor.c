
#include "log.h"
#include "stm32f4/gpio.h"
#include "stm32f4/io.h"
#include <stm32f4/tim.h>
#include <servomotor.h>
#include <log.h>
#include <time.h>

#define SM_TIMER_1  TIM3
#define SM_TIMER_2  TIM4

#define SM_PSC      26
#define SM_ARR      (APB1_CLK / (SM_PSC * 50))

#define NB_SERVOMOTOR       8

#define SM_DUTY_MIN    (APB1_CLK / (SM_PSC * 1000))   // 1ms
#define SM_DUTY_RANGE  (APB1_CLK / (SM_PSC * 1000))   // 1ms
#define SM_MAX_ANGLE    180

#define CONVERT_ANGLE(angle) (SM_DUTY_MIN + ((SM_DUTY_RANGE* angle / SM_MAX_ANGLE))) 

typedef struct {
    unsigned char pin;
    volatile gpio_t *gpio;
    unsigned char af;
    volatile uint32_t *ccr;
} sm_config_t;

const sm_config_t SM_CONFIG[NB_SERVOMOTOR] = {
    {0, GPIOB, 0x2, &(SM_TIMER_1->CCR3)},  // TIM3_CH3
    {1, GPIOB, 0x2, &(SM_TIMER_1->CCR4)},  // TIM3_CH4
    {4, GPIOB, 0x2, &(SM_TIMER_1->CCR1)},  // TIM3_CH1
    {5, GPIOB, 0x2, &(SM_TIMER_1->CCR2)},  // TIM3_CH2
    {6, GPIOB, 0x2, &(SM_TIMER_2->CCR1)},  // TIM4_CH1
    {7, GPIOB, 0x2, &(SM_TIMER_2->CCR2)},  // TIM4_CH2
    {8, GPIOB, 0x2, &(SM_TIMER_2->CCR3)},  // TIM4_CH3
    {9, GPIOB, 0x2, &(SM_TIMER_2->CCR4)}   // TIM4_CH4
};





void init_sm_tim(volatile timx_t *tim, int ccr) {
    DISABLE_IRQS;

    /* _____Config timer_____ */
	tim->CR1 = 0;               // Disable timer
	tim->PSC = SM_PSC - 1;         // setup prescalor
	tim->ARR = SM_ARR;           // setup period
	tim->EGR = TIM_UG;          // reset counter
	tim->SR = 0;                // reset status

    /* _____config channel 1_____ */
    tim->CCR1 = ccr; // Define Compare value
    tim->CCR2 = ccr;
    tim->CCR3 = ccr;
    tim->CCR4 = ccr;
    
    tim->CCMR1 = TIM_CC1S_OUT | TIM_OC1M_PWM1 | TIM_OC1PE;  // Channel 1 in output PWM1 mode and use preload register
    tim->CCMR1 |= TIM_CC2S_OUT | TIM_OC2M_PWM1 | TIM_OC2PE;
    tim->CCMR2 = TIM_CC3S_OUT | TIM_OC3M_PWM1 | TIM_OC3PE;
    tim->CCMR2 |= TIM_CC4S_OUT | TIM_OC4M_PWM1 | TIM_OC4PE;

    tim->CCER = TIM_CC1E | TIM_CC2E | TIM_CC3E | TIM_CC4E;  // Enable channels
    tim->EGR = TIM_UG;          // Generate Update Event
    tim->CR1 |= TIM_CEN;

    ENABLE_IRQS;
}

void init_sm_gpio(volatile gpio_t * gpio, int pin, int af) {
    gpio->MODER = REP_BITS(gpio->MODER, 2*pin, 2, GPIO_MODER_ALT);  // Mode alternate function
    gpio->AFRL = REP_BITS(gpio->AFRL, (pin)*4, 4, af);           // Alternant function
    gpio->OTYPER = REP_BITS(gpio->OTYPER, pin, 1, 0);   
}

void init_servomotor(void) {
    PRINTL("[%s] ... ", __func__);
    RCC->APB1ENR |= RCC_TIM3EN | RCC_TIM4EN;    // TODO : Pas modulaire
    RCC->AHB1ENR |= RCC_GPIOBEN;                // TODO : Pas modulaire

    init_sm_tim(SM_TIMER_1, CONVERT_ANGLE(SM_MAX_ANGLE/2));
    init_sm_tim(SM_TIMER_2, CONVERT_ANGLE(SM_MAX_ANGLE/2));
    for(int i = 0; i < NB_SERVOMOTOR; i++) {
        init_sm_gpio(SM_CONFIG[i].gpio, SM_CONFIG[i].pin, SM_CONFIG[i].af);
    }
    PRINTL("OK\n");
}


void sm_set_motor(unsigned int m, unsigned int angle) {
    if(!ASSERTL(angle < SM_MAX_ANGLE,"invalid angle (%u)", angle)) {
        return;
    }
    if(!ASSERTL(m < NB_SERVOMOTOR,"invalid motor (%u)", m)) {
        return;
    }
    *(SM_CONFIG[m].ccr) = CONVERT_ANGLE(angle);
    
}


unsigned int angle = 0;

void sm_move(time_t t, int step) {
    time_t start_time = get_time();
    time_t end_time = start_time + t;
    time_t period = 50000;
    time_t next_update = start_time + period;
    
    while (get_time() < end_time) {
        if(get_time() > next_update) {
            next_update = next_update + period;
            for(unsigned int i = 0; i < NB_SERVOMOTOR; i++) {
                sm_set_motor(i, angle);
            }
            PRINTL("[%s] angle : %d\n", __func__, angle);
            angle = (angle + step) % SM_MAX_ANGLE;
        }
    }
}