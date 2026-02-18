
#include "log.h"
#include "stm32f4/gpio.h"
#include "stm32f4/io.h"
#include "stm32f4/rcc.h"
#include <stm32f4/tim.h>
#include <servomotor.h>
#include <log.h>
#include <time.h>
#include <led.h>

#define SM_TIMER_1_PSC  (SM_TIMER_1_CLK / (65536 * 20))
#define SM_TIMER_2_PSC  (SM_TIMER_2_CLK / (65536 * 20))

#define SM_TIMER_1_ARR (SM_TIMER_1_CLK / (SM_TIMER_1_PSC * 50))
#define SM_TIMER_2_ARR (SM_TIMER_2_CLK / (SM_TIMER_2_PSC * 50))


#define SM_TIMER_1_DUTY_MIN    (SM_TIMER_1_CLK / (SM_TIMER_1_PSC * 1000))   // 1ms
#define SM_TIMER_1_DUTY_RANGE  (SM_TIMER_1_CLK / (SM_TIMER_1_PSC * 1000))   // 1ms

#define SM_TIMER_2_DUTY_MIN    (SM_TIMER_2_CLK / (SM_TIMER_2_PSC * 1000))   // 1ms
#define SM_TIMER_2_DUTY_RANGE  (SM_TIMER_2_CLK / (SM_TIMER_2_PSC * 1000))   // 1ms

#define SM_MAX_ANGLE    SM_MOVE_DIVIDER

#define CONVERT_ANGLE_TIMER_1(angle) (SM_TIMER_1_DUTY_MIN + SM_TIMER_1_DUTY_RANGE/8 + ((SM_TIMER_1_DUTY_RANGE*6/8 * (angle) / SM_MAX_ANGLE))) 
#define CONVERT_ANGLE_TIMER_2(angle) (SM_TIMER_2_DUTY_MIN + ((SM_TIMER_2_DUTY_RANGE * (angle) / SM_MAX_ANGLE))) 

#define NB_SERVOMOTOR       8

/* Move Z axis from SM_MAX_ANGLE to 0 */
#define Z_AXIS_MOVE_RIGHT(sm_id)    ((g_state.ref + g_state.shift[sm_id]) % SM_MAX_ANGLE)

/* Move Z axis from 0 to SM_MAX_ANGLE */
#define Z_AXIS_MOVE_LEFT(sm_id)     (((SM_MAX_ANGLE<<1) - g_state.ref - g_state.shift[sm_id]) % SM_MAX_ANGLE)

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
    bool invert;
} sm_config_t;


/**
 * @brief Configuration of each servomotor
 * 
 */
const sm_config_t SM_CONFIG[NB_SERVOMOTOR] = {
    [SM_FRZ] = {SM_FRZ_PIN, SM_FRZ_GPIO, SM_FRZ_AF, SM_FRZ_CCR, SM_FRZ_INVERT},  // TIM3_CH3
    [SM_RRZ] = {SM_RRZ_PIN, SM_RRZ_GPIO, SM_RRZ_AF, SM_RRZ_CCR, SM_RRZ_INVERT},  // TIM3_CH4
    [SM_RLZ] = {SM_RLZ_PIN, SM_RLZ_GPIO, SM_RLZ_AF, SM_RLZ_CCR, SM_RLZ_INVERT},  // TIM3_CH1
    [SM_FLZ] = {SM_FLZ_PIN, SM_FLZ_GPIO, SM_FLZ_AF, SM_FLZ_CCR, SM_FLZ_INVERT},  // TIM3_CH2
    [SM_FRX] = {SM_FRX_PIN, SM_FRX_GPIO, SM_FRX_AF, SM_FRX_CCR, SM_FRX_INVERT},  // TIM4_CH1
    [SM_RRX] = {SM_RRX_PIN, SM_RRX_GPIO, SM_RRX_AF, SM_RRX_CCR, SM_RRX_INVERT},  // TIM4_CH2
    [SM_RLX] = {SM_RLX_PIN, SM_RLX_GPIO, SM_RLX_AF, SM_RLX_CCR, SM_RLX_INVERT},  // TIM4_CH3
    [SM_FLX] = {SM_FLX_PIN, SM_FLX_GPIO, SM_FLX_AF, SM_FLX_CCR, SM_FLX_INVERT}   // TIM4_CH4
};


const int SM_SHIFT[SM_MAX_MOVE][4] = {
    [SM_STOP]           = {0, 0, 0, 0},
    [SM_FORWARD]        = {0, SM_MAX_ANGLE*3/4, SM_MAX_ANGLE/4, SM_MAX_ANGLE/2},
    [SM_REVERSE]        = {SM_MAX_ANGLE/4, SM_MAX_ANGLE*3/4, 0, SM_MAX_ANGLE/2},
    [SM_ROTATE_LEFT]    = {0, SM_MAX_ANGLE*3/4, SM_MAX_ANGLE/4, SM_MAX_ANGLE/2},
    [SM_ROTATE_RIGHT]   = {0, SM_MAX_ANGLE*3/4, SM_MAX_ANGLE/4, SM_MAX_ANGLE/2},
    [SM_INIT_Z_AXIS]    = {0, 0, 0, 0},
    [SM_INIT_X_AXIS]    = {0, 0, 0, 0},
    [SM_START]          = {0, 0, 0, 0}
};



/**
 * @brief Global state of servomotor module
 * 
 */
sm_state_t g_state = {
    .move = SM_START,    // Current movement
    .prev = SM_STOP,    // Previous movement
    .ref = 0,          // reference angle
    .swhitch_ref = 0,  // reference angle during move switch
    .shift = {SM_SHIFT[SM_START][0], SM_SHIFT[SM_START][1], SM_SHIFT[SM_START][2], SM_SHIFT[SM_START][3]}, // shift of each servomotor with the reference angle
    .angles = {0, SM_MAX_ANGLE, 0, SM_MAX_ANGLE, 0, 0, 0, 0}    // Current destination angle
};

void init_sm_tim(volatile timx_t *tim, unsigned int arr, unsigned int psc);
void init_sm_gpio(volatile gpio_t * gpio, int pin, int af);

void sm_set_motor(sm_id m, unsigned int angle);
unsigned int sm_get_z_axis_pos(sm_id sm);
unsigned int sm_get_x_axis_pos(sm_id smx_id, sm_id smz_id);
void sm_switch_move(void);
void sm_next_state(void);



/**
 * @brief Initialize timer
 * 
 * @param tim Timer to initialize
 * @param arr ARR for timer
 * @param psc PSC for timer
 *
 * @note Doesn't start the timer
 */
void init_sm_tim(volatile timx_t *tim, unsigned int arr, unsigned int psc) {
    DISABLE_IRQS;

    /* _____Config timer_____ */
	tim->CR1 = 0;               // Disable timer
	tim->PSC = psc - 1;         // setup prescalor
	tim->ARR = arr;           // setup period
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

/**
 * @brief Initilize GPIO
 * 
 * @param gpio GPIO to initialize
 * @param pin Pin to initialize
 * @param af Alternate function used
 */
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

    /* enable modules */
    enable_gpio(SM_FRZ_GPIO);
    enable_gpio(SM_RRZ_GPIO);
    enable_gpio(SM_RLZ_GPIO);
    enable_gpio(SM_FLZ_GPIO);
    enable_gpio(SM_FRX_GPIO);
    enable_gpio(SM_RRX_GPIO);
    enable_gpio(SM_RLX_GPIO);
    enable_gpio(SM_FLX_GPIO);
    enable_timx(SM_TIMER_1);
    enable_timx(SM_TIMER_2);

    /* config timers */
    init_sm_tim(SM_TIMER_1, SM_TIMER_1_ARR, SM_TIMER_1_PSC);
    init_sm_tim(SM_TIMER_2, SM_TIMER_2_ARR, SM_TIMER_2_PSC);
    sm_set_motor((unsigned int)SM_FRZ, g_state.angles[SM_FRZ] + g_state.shift[SM_FRZ]);
    sm_set_motor((unsigned int)SM_RRZ, g_state.angles[SM_RRZ] + g_state.shift[SM_RRZ]);
    sm_set_motor((unsigned int)SM_RLZ, g_state.angles[SM_RLZ] + g_state.shift[SM_RLZ]);
    sm_set_motor((unsigned int)SM_FLZ, g_state.angles[SM_FLZ] + g_state.shift[SM_FLZ]);
    sm_set_motor((unsigned int)SM_FRX, g_state.angles[SM_FRX]);
    sm_set_motor((unsigned int)SM_RRX, g_state.angles[SM_RRX]);
    sm_set_motor((unsigned int)SM_RLX, g_state.angles[SM_RLX]);
    sm_set_motor((unsigned int)SM_FLX, g_state.angles[SM_FLX]);
    SM_TIMER_1->CR1 |= TIM_CEN;
    SM_TIMER_2->CR1 |= TIM_CEN;

    /* config gpio */
    for(int i = 0; i < NB_SERVOMOTOR; i++) {
        init_sm_gpio(SM_CONFIG[i].gpio, SM_CONFIG[i].pin, SM_CONFIG[i].af);
    }
    PRINTL("OK\n");
}

/**
 * @brief Move the servomotor to the given angle
 * 
 * @param m ID of the servomotor
 * @param angle Target angle
 */
void sm_set_motor(sm_id m, unsigned int angle) {
    if(!ASSERTL(angle <= SM_MAX_ANGLE,"invalid angle : motor %d (%u)", m,  angle)) {
        return;
    }
    if(!ASSERTL(m < NB_SERVOMOTOR,"invalid motor (%u)", m)) {
        return;
    }
    switch (m) {
    case SM_FRZ:
    case SM_RRZ:
    case SM_RLZ:
    case SM_FLZ:
        *(SM_CONFIG[m].ccr) = (SM_CONFIG[m].invert) ? (CONVERT_ANGLE_TIMER_1((SM_MAX_ANGLE - angle))) : (CONVERT_ANGLE_TIMER_1(angle));
        break;
    case SM_RRX:
    case SM_FRX:
    case SM_RLX:
    case SM_FLX:
        *(SM_CONFIG[m].ccr) = (SM_CONFIG[m].invert) ? (CONVERT_ANGLE_TIMER_2(SM_MAX_ANGLE - angle)) : (CONVERT_ANGLE_TIMER_2(angle));
        break;
    }
}

/**
 * @brief Compute the angle at which the Z-axis servomotor should be
 * 
 * @param sm servomotor of the Z axis
 * @return angle
 */
unsigned int sm_get_z_axis_pos(sm_id sm) {
    if (SM_FRZ == sm || SM_RRZ == sm) {
        switch (g_state.move) {
            case SM_STOP:
                return g_state.angles[sm];
            case SM_FORWARD:
            case SM_ROTATE_LEFT:
                return Z_AXIS_MOVE_RIGHT(sm);
            case SM_REVERSE:
            case SM_ROTATE_RIGHT:
                return Z_AXIS_MOVE_LEFT(sm);
            case SM_INIT_Z_AXIS:
                if(g_state.ref == 0) {
                    return 0;
                }
                if(g_state.ref == SM_MAX_ANGLE/3) {
                    return SM_MAX_ANGLE/2;
                }
                if(g_state.ref == SM_MAX_ANGLE*2/3) {
                    return SM_MAX_ANGLE - 1;
                }
                return g_state.angles[sm];
            case SM_INIT_X_AXIS:
                return g_state.angles[sm];
            case SM_START:
                return g_state.angles[sm];
            default:
                WARNL("undefined move : %d", g_state.move);
                return g_state.angles[sm];
        }
    }
    if (SM_FLZ == sm || SM_RLZ == sm) {
        switch (g_state.move) {
            case SM_STOP:
                return g_state.angles[sm];
            case SM_FORWARD:
            case SM_ROTATE_RIGHT:
                return Z_AXIS_MOVE_LEFT(sm);
            case SM_REVERSE:
            case SM_ROTATE_LEFT:
                return Z_AXIS_MOVE_RIGHT(sm);
            case SM_INIT_Z_AXIS:
                if(g_state.ref == 0) {
                    return 0;
                }
                if(g_state.ref == SM_MAX_ANGLE/3) {
                    return SM_MAX_ANGLE/2;
                }
                if(g_state.ref == SM_MAX_ANGLE*2/3) {
                    return SM_MAX_ANGLE - 1;
                }
                return g_state.angles[sm];
            case SM_INIT_X_AXIS:
                return g_state.angles[sm];
            case SM_START:
                return g_state.angles[sm];
            default:
                WARNL("undefined move : %d", g_state.move);
                return g_state.angles[sm];
        }
    }
    WARNL("Not a Z axis servomotor : %d", g_state.move);
    return g_state.angles[sm];
}

/**
 * @brief Compute the angle at which the X-axis servomotor should be
 * 
 * @param smx_id Servomotor of the X axis
 * @param smz_id Servomotor of the Z axis on the same leg as smx_id
 * @return angle
 */
unsigned int sm_get_x_axis_pos(sm_id smx_id, sm_id smz_id) {
    switch(g_state.move) {
        case SM_STOP:
            return 0;
        case SM_FORWARD:
        case SM_REVERSE:
        case SM_ROTATE_LEFT:
        case SM_ROTATE_RIGHT:
            if ((g_state.ref + g_state.shift[smz_id]) == SM_MAX_ANGLE - 2) {
                return SM_MAX_ANGLE / 2;
            }

            if ((g_state.ref + g_state.shift[smz_id]) % SM_MAX_ANGLE == 8) {
                return 0;
            }
            return g_state.angles[smx_id];
        case SM_INIT_X_AXIS:
            if(g_state.ref == 0) {
                return 0;
            }
            if(g_state.ref == SM_MAX_ANGLE/3) {
                return SM_MAX_ANGLE/2;
            }
            if(g_state.ref == SM_MAX_ANGLE*2/3) {
                return SM_MAX_ANGLE - 1;
            }
            return g_state.angles[smx_id];
        case SM_INIT_Z_AXIS:
            return g_state.angles[smx_id];
        case SM_START:
            return 0;
        default:
            WARNL("Undefined move %d", g_state.move);
    }
    return g_state.angles[smx_id];
}

/**
 * @brief Move each leg to its position in the new movement
 * 
 */
void sm_switch_move(void) {
    if (g_state.swhitch_ref > 0) {
        if (g_state.swhitch_ref < 5) {
            g_state.angles[SM_FRX] = 0;
            g_state.angles[SM_RRX] = 0;
            g_state.angles[SM_RLX] = 0;
            g_state.angles[SM_FLX] = 0;
        }
        g_state.swhitch_ref--;
        return;
    }
    
    g_state.angles[SM_FRX] = 0;
    g_state.angles[SM_RRX] = 0;
    g_state.angles[SM_RLX] = 0;
    g_state.angles[SM_FLX] = 0;

    if (sm_get_z_axis_pos(SM_FRZ) != g_state.angles[SM_FRZ]) {
        PRINTL("[SWITCH] Set Front Right Leg\n");
        g_state.angles[SM_FRX] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_FRZ] = sm_get_z_axis_pos(SM_FRZ);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_z_axis_pos(SM_RRZ) != g_state.angles[SM_RRZ]) {
        PRINTL("[SWITCH] Set Rear Right Leg\n");
        g_state.angles[SM_RRX] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_RRZ] = sm_get_z_axis_pos(SM_RRZ);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_z_axis_pos(SM_RLZ) != g_state.angles[SM_RLZ]) {
        PRINTL("[SWITCH] Set Rear Left Leg\n");
        g_state.angles[SM_RLX] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_RLZ] = sm_get_z_axis_pos(SM_RLZ);
        g_state.swhitch_ref = 10;
        return;
    }
    if (sm_get_z_axis_pos(SM_FLZ) != g_state.angles[SM_FLZ]) {
        PRINTL("[SWITCH] Set Front Left Leg\n");
        g_state.angles[SM_FLX] = SM_MAX_ANGLE / 2;
        g_state.angles[SM_FLZ] = sm_get_z_axis_pos(SM_FLZ);
        g_state.swhitch_ref = 10;
        return;
    }

    g_state.prev = g_state.move;
}

/**
 * @brief Update the global state of the servomotor module
 * 
 */
void sm_next_state(void) {
    if (g_state.move != SM_STOP && g_state.move != g_state.prev) {
        sm_switch_move();
        return;
    }
    g_state.ref = (g_state.ref + 1) % SM_MAX_ANGLE;
    g_state.angles[SM_FRZ] = sm_get_z_axis_pos(SM_FRZ);
    g_state.angles[SM_RRZ] = sm_get_z_axis_pos(SM_RRZ);
    g_state.angles[SM_RLZ] = sm_get_z_axis_pos(SM_RLZ);
    g_state.angles[SM_FLZ] = sm_get_z_axis_pos(SM_FLZ);
    g_state.angles[SM_FRX] = sm_get_x_axis_pos(SM_FRX, SM_FRZ);
    g_state.angles[SM_RRX] = sm_get_x_axis_pos(SM_RRX, SM_RRZ);
    g_state.angles[SM_RLX] = sm_get_x_axis_pos(SM_RLX, SM_RLZ);
    g_state.angles[SM_FLX] = sm_get_x_axis_pos(SM_FLX, SM_FLZ);

}


void sm_set_next_move(sm_move_t m) {
    PRINTL("New move : %d\n", m);
    g_state.prev = g_state.move;
    g_state.move = m;
    g_state.shift[0] = SM_SHIFT[m][0];
    g_state.shift[1] = SM_SHIFT[m][1];
    g_state.shift[2] = SM_SHIFT[m][2];
    g_state.shift[3] = SM_SHIFT[m][3];
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
            sm_set_motor(SM_FRZ, g_state.angles[SM_FRZ]);
            sm_set_motor(SM_RRZ, g_state.angles[SM_RRZ]);
            sm_set_motor(SM_RLZ, g_state.angles[SM_RLZ]);
            sm_set_motor(SM_FLZ, g_state.angles[SM_FLZ]);
            sm_set_motor(SM_FRX, g_state.angles[SM_FRX]);
            sm_set_motor(SM_RRX, g_state.angles[SM_RRX]);   
            sm_set_motor(SM_RLX, g_state.angles[SM_RLX]);
            sm_set_motor(SM_FLX, g_state.angles[SM_FLX]);
        }
    }
}