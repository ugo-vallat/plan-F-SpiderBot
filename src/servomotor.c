
#include "log.h"
#include "stm32f4/gpio.h"
#include "stm32f4/io.h"
#include <stm32f4/tim.h>
#include <servomotor.h>
#include <log.h>
#include <time.h>
#include <led.h>



#define SM_PSC      26
#define SM_ARR      (APB1_CLK / (SM_PSC * 50))

#define NB_SERVOMOTOR       8

#define SM_DUTY_MIN    (APB1_CLK / (SM_PSC * 1000))   // 1ms
#define SM_DUTY_RANGE  (APB1_CLK / (SM_PSC * 1000))   // 1ms
#define SM_MAX_ANGLE    SM_MOVE_DIVIDER

#define CONVERT_ANGLE(angle) (SM_DUTY_MIN + ((SM_DUTY_RANGE* angle / SM_MAX_ANGLE))) 

typedef struct {
    sm_move_t move;
    sm_move_t prev;
    int ref;
    int swhitch_ref;
    int shift[4];
    unsigned int angles [8];
} sm_state_t;

typedef struct {
    unsigned char pin;
    volatile gpio_t *gpio;
    unsigned char af;
    volatile uint32_t *ccr;
} sm_config_t;


const sm_config_t SM_CONFIG[NB_SERVOMOTOR] = {
    [SM_FRS] = {SM_FRS_PIN, SM_FRS_GPIO, SM_FRS_AF, SM_FRS_CCR},  // TIM3_CH3
    [SM_RRS] = {SM_RRS_PIN, SM_RRS_GPIO, SM_RRS_AF, SM_RRS_CCR},  // TIM3_CH4
    [SM_RLS] = {SM_RLS_PIN, SM_RLS_GPIO, SM_RLS_AF, SM_RLS_CCR},  // TIM3_CH1
    [SM_FLS] = {SM_FLS_PIN, SM_FLS_GPIO, SM_FLS_AF, SM_FLS_CCR},  // TIM3_CH2
    [SM_FRE] = {SM_FRE_PIN, SM_FRE_GPIO, SM_FRE_AF, SM_FRE_CCR},  // TIM4_CH1
    [SM_RRE] = {SM_RRE_PIN, SM_RRE_GPIO, SM_RRE_AF, SM_RRE_CCR},  // TIM4_CH2
    [SM_RLE] = {SM_RLE_PIN, SM_RLE_GPIO, SM_RLE_AF, SM_RLE_CCR},  // TIM4_CH3
    [SM_FLE] = {SM_FLE_PIN, SM_FLE_GPIO, SM_FLE_AF, SM_FLE_CCR}   // TIM4_CH4
};


sm_state_t g_state = {
    SM_STOP,
    SM_STOP,
    0, 
    0,
    {0, SM_MAX_ANGLE/4, SM_MAX_ANGLE/2, SM_MAX_ANGLE*3/4},
    {0, 0, 0, 0, 0, 0, 0, 0}
};


void sm_set_motor(unsigned int m, unsigned int angle);


void init_sm_tim(volatile timx_t *tim) {
    DISABLE_IRQS;

    /* _____Config timer_____ */
	tim->CR1 = 0;               // Disable timer
	tim->PSC = SM_PSC - 1;         // setup prescalor
	tim->ARR = SM_ARR;           // setup period
	tim->EGR = TIM_UG;          // reset counter
	tim->SR = 0;                // reset status

    /* _____config channel _____ */
    tim->CCR1 = 0; // Define Compare value
    tim->CCR2 = 0;
    tim->CCR3 = 0;
    tim->CCR4 = 0;
    
    tim->CCMR1 = TIM_CC1S_OUT | TIM_OC1M_PWM1 | TIM_OC1PE;  // Channel 1 in output PWM1 mode and use preload register
    tim->CCMR1 |= TIM_CC2S_OUT | TIM_OC2M_PWM1 | TIM_OC2PE;
    tim->CCMR2 = TIM_CC3S_OUT | TIM_OC3M_PWM1 | TIM_OC3PE;
    tim->CCMR2 |= TIM_CC4S_OUT | TIM_OC4M_PWM1 | TIM_OC4PE;

    tim->CCER = TIM_CC1E | TIM_CC2E | TIM_CC3E | TIM_CC4E;  // Enable channels
    tim->EGR = TIM_UG;          // Generate Update Event

    ENABLE_IRQS;
}

void init_sm_gpio(volatile gpio_t * gpio, int pin, int af) {
    gpio->MODER = REP_BITS(gpio->MODER, 2*pin, 2, GPIO_MODER_ALT);  // Mode alternate function
    if(pin < 8) {
        gpio->AFRL = REP_BITS(gpio->AFRL, (pin)*4, 4, af);           // Alternant function
    } else {
        gpio->AFRH = REP_BITS(gpio->AFRH, (pin-8)*4, 4, af);
    }
    gpio->OTYPER = REP_BITS(gpio->OTYPER, pin, 1, 0);   
}

void init_module_servomotor(void) {
    PRINTL("[%s] ... ", __func__);
    RCC->APB1ENR |= RCC_TIM3EN | RCC_TIM4EN;    // TODO : Pas modulaire
    RCC->AHB1ENR |= RCC_GPIOBEN | RCC_GPIODEN;  // TODO : Pas modulaire

    /* config timers */
    init_sm_tim(SM_TIMER_1);
    init_sm_tim(SM_TIMER_2);
    sm_set_motor((unsigned int)SM_FRS, g_state.angles[SM_FRS] + g_state.shift[SM_FRS]);
    sm_set_motor((unsigned int)SM_RRS, g_state.angles[SM_RRS] + g_state.shift[SM_RRS]);
    sm_set_motor((unsigned int)SM_RLS, g_state.angles[SM_RLS] + g_state.shift[SM_RLS]);
    sm_set_motor((unsigned int)SM_FLS, g_state.angles[SM_FLS] + g_state.shift[SM_FLS]);
    sm_set_motor((unsigned int)SM_FRE, g_state.angles[SM_FRE]);
    sm_set_motor((unsigned int)SM_RRE, g_state.angles[SM_RRE]);
    sm_set_motor((unsigned int)SM_RLE, g_state.angles[SM_RLE]);
    sm_set_motor((unsigned int)SM_FLE, g_state.angles[SM_FLE]);
    SM_TIMER_1->CR1 |= TIM_CEN;
    SM_TIMER_2->CR1 |= TIM_CEN;


    for(int i = 0; i < NB_SERVOMOTOR; i++) {
        init_sm_gpio(SM_CONFIG[i].gpio, SM_CONFIG[i].pin, SM_CONFIG[i].af);
    }
    PRINTL("OK\n");
}



void sm_set_motor(unsigned int m, unsigned int angle) {
    if(!ASSERTL(angle < SM_MAX_ANGLE,"invalid angle : motor %d (%u)", m,  angle)) {
        return;
    }
    if(!ASSERTL(m < NB_SERVOMOTOR,"invalid motor (%u)", m)) {
        return;
    }
    *(SM_CONFIG[m].ccr) = CONVERT_ANGLE(angle);
    
}
#define SHOULDER_MOVE_RIGHT(shoulder_id)    ((g_state.ref + g_state.shift[shoulder_id]) % SM_MAX_ANGLE)
#define SHOULDER_MOVE_LEFT(shoulder_id)     (((SM_MAX_ANGLE<<1) - g_state.ref - g_state.shift[shoulder_id]) % SM_MAX_ANGLE)


unsigned int sm_get_shoulder_pos(sm_id sm) {
    switch (g_state.move) {
        case SM_STOP:
            return g_state.angles[sm];
        case SM_FORWARD:
        case SM_ROTATE_LEFT:
            return SHOULDER_MOVE_RIGHT(sm);
        case SM_REVERSE:
        case SM_ROTATE_RIGH:
            return SHOULDER_MOVE_LEFT(sm);
        default:
            WARNL("undefined move : %d", g_state.move);
            return g_state.angles[sm];
    }
}


unsigned int sm_get_elbow(sm_id elbow_id, sm_id shoulder_id) {
    if ((g_state.ref + g_state.shift[shoulder_id]) == SM_MAX_ANGLE - 2) {
        return SM_MAX_ANGLE / 2;
    }

    if ((g_state.ref + g_state.shift[shoulder_id]) % SM_MAX_ANGLE == 2) {
        return 0;
    }

    return g_state.angles[elbow_id];
}

void sm_switch_move(void) {
    if (g_state.swhitch_ref > 0) {
        g_state.swhitch_ref--;
        return;
    }
    g_state.angles[SM_FRE] = 0;
    g_state.angles[SM_RRE] = 0;
    g_state.angles[SM_RLE] = 0;
    g_state.angles[SM_FLE] = 0;

    if (sm_get_shoulder_pos(SM_FRS) != g_state.angles[SM_FRS]) {
        PRINTL("[SWITCH] Set Front Right Leg\n");
        g_state.angles[SM_FRE] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_FRS] = sm_get_shoulder_pos(SM_FRS);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_shoulder_pos(SM_RRS) != g_state.angles[SM_RRS]) {
        PRINTL("[SWITCH] Set Rear Right Leg\n");
        g_state.angles[SM_RRE] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_RRS] = sm_get_shoulder_pos(SM_RRS);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_shoulder_pos(SM_RLS) != g_state.angles[SM_RLS]) {
        PRINTL("[SWITCH] Set Rear Left Leg\n");
        g_state.angles[SM_RLE] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_RLS] = sm_get_shoulder_pos(SM_RLS);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_shoulder_pos(SM_FLS) != g_state.angles[SM_FLS]) {
        PRINTL("[SWITCH] Set Front Left Leg\n");
        g_state.angles[SM_FLE] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_FLS] = sm_get_shoulder_pos(SM_FLS);
        g_state.swhitch_ref = 10;
        return;
    }

    g_state.prev = g_state.move;
}



void sm_next_state(void) {
    if (g_state.move != SM_STOP && g_state.move != g_state.prev) {
        sm_switch_move();
        return;
    }
    g_state.ref = (g_state.ref + 1) % SM_MAX_ANGLE;
    g_state.angles[SM_FRS] = sm_get_shoulder_pos(SM_FRS);
    g_state.angles[SM_RRS] = sm_get_shoulder_pos(SM_RRS);
    g_state.angles[SM_RLS] = sm_get_shoulder_pos(SM_RLS);
    g_state.angles[SM_FLS] = sm_get_shoulder_pos(SM_FLS);
    g_state.angles[SM_FRE] = sm_get_elbow(SM_FRE, SM_FRS);
    g_state.angles[SM_RRE] = sm_get_elbow(SM_RRE, SM_RRS);
    g_state.angles[SM_RLE] = sm_get_elbow(SM_RLE, SM_RLS);
    g_state.angles[SM_FLE] = sm_get_elbow(SM_FLE, SM_FLS);

}


void sm_set_next_move(sm_move_t m) {
    PRINTL("New move : %d\n", m);
    g_state.prev = g_state.move;
    g_state.move = m;
}

void sm_move(time_t t) {
    time_t start_time = get_time();
    time_t end_time = start_time + t;
    time_t period = SM_UPDATE_PERIOD;
    time_t next_update = start_time + period;
    
    while (get_time() < end_time) {

        if(get_time() > next_update) {
            SWITCH_G_LED();
            next_update = next_update + period;
            sm_next_state();
            sm_set_motor(SM_FRS, g_state.angles[SM_FRS]);
            sm_set_motor(SM_RRS, g_state.angles[SM_RRS]);
            sm_set_motor(SM_RLS, g_state.angles[SM_RLS]);
            sm_set_motor(SM_FLS, g_state.angles[SM_FLS]);
            sm_set_motor(SM_FRE, SM_MAX_ANGLE - 1 - g_state.angles[SM_FRE]);
            sm_set_motor(SM_RRE, SM_MAX_ANGLE - 1 - g_state.angles[SM_RRE]);
            sm_set_motor(SM_RLE, SM_MAX_ANGLE - 1 - g_state.angles[SM_RLE]);
            sm_set_motor(SM_FLE, SM_MAX_ANGLE - 1 - g_state.angles[SM_FLE]);

        }
    }
}