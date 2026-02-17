#include <led.h>
#include <stm32f4/rcc.h>
#include <log.h>
#include <time.h>
#include <bluetooth.h>

/* --- Bluetooth LED Configuration --- */
#define BT_LED_BLINK_DURATION_US 20000 /*!< Activity flicker duration: 20ms */

/* --- State Variables --- */
static time_t bt_led_deadline = 0;
static bool   bt_led_is_blinking = false;


/**
 * @brief Initializes the LED module.
 * Enables the clock for GPIOD and configures the 4 standard board LEDs 
 * (Green, Orange, Red, Blue) as outputs.
 */
void init_module_led(void) {
    PRINTL("[%s] ... ", __func__);
    
    /* Enable clock for GPIOD where the board LEDs are connected */
    enable_gpio(GPIOD);
    
    /* Initialize individual LED pins */
    INIT_LED(GPIOD, LED_G_PIN);
    INIT_LED(GPIOD, LED_O_PIN);
    INIT_LED(GPIOD, LED_R_PIN);
    INIT_LED(GPIOD, LED_B_PIN);
    
    PRINTL("OK\n");
}


/**
 * @brief Triggers a brief flicker on the Bluetooth status LED (Orange LED).
 *
 * This function creates a "negative pulse": if the Bluetooth is connected, 
 * the LED is normally ON. This function turns it OFF and sets a deadline 
 * to turn it back ON, creating a quick blink to indicate data activity.
 */
void bluetooth_trigger_activity(void) {
    // Do nothing if Bluetooth is not currently connected
    if (!bluetooth_is_connected()) {
        return; 
    }
    
    // Turn OFF the LED to start the flicker effect
    LED_O_OFF();
    
    // Set the deadline for when the LED should turn back ON
    bt_led_deadline = get_time() + BT_LED_BLINK_DURATION_US;
    bt_led_is_blinking = true;
}


/**
 * @brief Non-blocking background task to maintain the Bluetooth LED state.
 * * Must be called continuously in the main loop. It handles the following logic:
 * - Not connected -> LED OFF.
 * - Connected & Idle -> LED ON solid.
 * - Connected & Active -> LED flickers OFF for 20ms, then returns to solid ON.
 */
void bluetooth_process_led_task(void) {
    if (bluetooth_is_connected()) {
        
        if (bt_led_is_blinking) {
            // Check if the 20ms flicker duration has elapsed
            if (deadline_is_reached(bt_led_deadline)) {
                LED_O_ON(); // End of flicker, restore solid state
                bt_led_is_blinking = false;
            }
        } else {
            // Keep the LED on while connected and idle
            LED_O_ON();
        }
        
    } else {
        // Ensure LED is turned off if the connection drops
        LED_O_OFF(); 
        bt_led_is_blinking = false;
    }
}