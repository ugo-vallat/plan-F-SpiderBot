#include <time.h>
#include <log.h>
#include <stm32f4/io.h>
#include <stm32f4/nvic.h>
#include <stm32f4/tim.h>
#include <stm32f4/rcc.h>


#define TIME_PSC        1
#define TIME_ARR_1US    (APB1_CLK / TIME_PSC / 1000000) 

static time_t cur_time = 0;


void handler_time(void) {
    if ((TIM6_SR & TIM_UIF) == 0) {
        return;
    }
    TIM6_SR &= ~TIM_UIF;
    cur_time++;
}

void init_time(void) {
    printl("[%s] ... ", __func__);
    cur_time = 0;

    RCC_APB1ENR |= RCC_TIM6EN;

    DISABLE_IRQS;

    TIM6_CR1 = 0;               // Disable timer
	TIM6_PSC = TIME_PSC - 1;    // setup prescalor
	TIM6_ARR = TIME_ARR_1US;    // setup period
	TIM6_EGR = TIM_UG;          // reset counter
	TIM6_SR = 0;                // reset status
    TIM6_DIER = TIM_UIE;        // update interrupt enabled
    NVIC_ICER(TIM6_IRQ/32) = 1 <<(TIM6_IRQ%32);     // Interrupt Clear-Enable (desable interrupt)
    NVIC_IRQ(TIM6_IRQ) = (uint32_t)handler_time;    // Define handler
    NVIC_IPR(TIM6_IRQ) = 0;     // InterruptPriority
    NVIC_ICPR(TIM6_IRQ/32) = 1 << (TIM6_IRQ % 32);  // Interrupt clear pending
    NVIC_ISER(TIM6_IRQ/32) = 1 << (TIM6_IRQ % 32);  // Interrupt Set-Enable (enable interrupt)

    ENABLE_IRQS;

    TIM6_CR1 = TIM_CEN;
    printl("OK\n");
}

time_t get_time(void) {
    return cur_time;
}

bool deadline_is_reached(time_t deadline) {
    return deadline <= cur_time;
}