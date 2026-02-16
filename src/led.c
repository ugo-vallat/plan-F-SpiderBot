
#include <led.h>
#include <stm32f4/rcc.h>
#include <log.h>
#include <time.h>

#define BT_LED_BLINK_DURATION_US 20000
static time_t bt_led_deadline = 0;
static bool bt_led_is_blinking = false;

void init_module_led(void) {
    PRINTL("[%s] ... ", __func__);
    /* Board leds */
    enable_gpio(GPIOD);
    INIT_LED(GPIOD, LED_G_PIN);
    INIT_LED(GPIOD, LED_O_PIN);
    INIT_LED(GPIOD, LED_R_PIN);
    INIT_LED(GPIOD, LED_B_PIN);
    PRINTL("OK\n");
}
/**
 * @brief Déclenche le scintillement de la LED
 */
void bluetooth_trigger_activity(void) {
    if (!bluetooth_is_connected()) return; 
    
    LED_O_OFF();
    bt_led_deadline = get_time() + BT_LED_BLINK_DURATION_US;
    bt_led_is_blinking = true;
}

/**
 * @brief Tâche de fond pour maintenir l'état de la LED
 */
void bluetooth_process_led_task(void) {
    if (bluetooth_is_connected()) {
        if (bt_led_is_blinking) {
            if (deadline_is_reached(bt_led_deadline)) {
                LED_O_ON(); 
                bt_led_is_blinking = false;
            }
        } else {
            LED_O_ON();
        }
    } else {
        LED_O_OFF(); 
        bt_led_is_blinking = false;
    }
}