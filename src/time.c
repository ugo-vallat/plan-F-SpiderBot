#include <time.h>
#include <log.h>
#include <stm32f4/io.h>
#include <stm32f4/nvic.h>
#include <stm32f4/tim.h>
#include <stm32f4/rcc.h>

#define TIME_TIM TIM6
#define TIME_TIM_IRQ TIM6_IRQ

#define TIME_PSC        1
#define TIME_ARR_1US    (APB1_CLK / TIME_PSC / 1000000) 

static time_t cur_time = 0;


void handler_time(void) {
    if ((TIME_TIM->SR & TIM_UIF) == 0) {
        return;
    }
    TIME_TIM->SR &= ~TIM_UIF;
    cur_time++;
}

void init_time(void) {
    printl("[%s] ... ", __func__);
    cur_time = 0;

    RCC->APB1ENR |= RCC_TIM6EN;

    DISABLE_IRQS;

    TIME_TIM->CR1 = 0;               // Disable timer
	TIME_TIM->PSC = TIME_PSC - 1;    // setup prescalor
	TIME_TIM->ARR = TIME_ARR_1US;    // setup period
	TIME_TIM->EGR = TIM_UG;          // reset counter
	TIME_TIM->SR = 0;                // reset status
    TIME_TIM->DIER = TIM_UIE;        // update interrupt enabled
    NVIC_ICER(TIME_TIM_IRQ/32) = 1 <<(TIME_TIM_IRQ%32);     // Interrupt Clear-Enable (desable interrupt)
    NVIC_IRQ(TIME_TIM_IRQ) = (uint32_t)handler_time;    // Define handler
    NVIC_IPR(TIME_TIM_IRQ) = 0;     // InterruptPriority
    NVIC_ICPR(TIME_TIM_IRQ/32) = 1 << (TIME_TIM_IRQ % 32);  // Interrupt clear pending
    NVIC_ISER(TIME_TIM_IRQ/32) = 1 << (TIME_TIM_IRQ % 32);  // Interrupt Set-Enable (enable interrupt)

    ENABLE_IRQS;

    TIME_TIM->CR1 = TIM_CEN;
    printl("OK\n");
}

time_t get_time(void) {
    return cur_time;
}

bool deadline_is_reached(time_t deadline) {
    return deadline <= cur_time;
}