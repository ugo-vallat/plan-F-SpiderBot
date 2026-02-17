/**
 * @file battery.c
 * @brief Implementation of the battery monitoring module.
 * * Handles ADC1 configuration on PA1 to read battery voltage,
 * calculates the remaining percentage using integer arithmetic,
 * and triggers a low-battery LED warning.
 */

#include <stm32f4/adc.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <tinyprintf.h>
#include <log.h>
#include <battery.h>
#include <led.h>

/* --- ADC Configuration Defines --- */
#define ADC_CHANNEL_1           1
#define RCC_AHB1ENR_GPIOAEN     (1 << 0)   
#define RCC_APB2ENR_ADC1EN      (1 << 8)
#define ADC_TIMEOUT_CYCLES      10000

/* --- Battery Calibration Defines --- */
// Based on a voltage divider connected to a 2-cell Li-Ion (6.0V - 8.4V)
#define BATTERY_ADC_MAX_VAL     3300  /*!< ADC value corresponding to 100% (approx 8.4V) */
#define BATTERY_ADC_MIN_VAL     2350  /*!< ADC value corresponding to 0% (approx 6.0V) */
#define BATTERY_ADC_RANGE       (BATTERY_ADC_MAX_VAL - BATTERY_ADC_MIN_VAL) /*!< Range = 950 */

#define BATTERY_LOW_THRESHOLD   10    /*!< Percentage below which the red LED turns on */


/**
 * @brief Initializes the battery module (GPIO and ADC1).
 */
void init_module_battery(void) {
    PRINTL("[%s] ... ", __func__);

    // Enable Clocks for GPIOA and ADC1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  

    // Configure PA1 in Analog mode (MODER = 11)
    GPIOA->MODER |= (3 << (1 * 2)); 

    // Configure and enable ADC1
    ADC1->CR1 = 0; 
    ADC1->CR2 = 0;
    ADC1->CR2 |= ADC_ADON; // Power up the ADC
    
    // Short delay to allow ADC voltage to stabilize after power-on (tSTAB)
    for(volatile int i = 0; i < 2000; i++);

    PRINTL("OK\n");
}


/**
 * @brief Reads the ADC and calculates the battery percentage.
 * @return The battery level (0-100), or -1 if the ADC conversion times out.
 */
int get_battery_percentage(void) {
    // Select ADC Channel 1 for the first conversion in the regular sequence
    ADC1->SQR3 = ADC_CHANNEL_1;
    
    // Start the conversion
    ADC1->CR2 |= ADC_SWSTART;
    
    // Wait for End Of Conversion (EOC) flag with a timeout mechanism
    int timeout = ADC_TIMEOUT_CYCLES;
    while (!(ADC1->SR & ADC_EOC)) {
        if (--timeout == 0) {
            PRINTL("ERROR: ADC Conversion Timeout\n");
            return -1; 
        }
    }

    // 4. Read the raw ADC value
    uint32_t adc_val = ADC1->DR;
    PRINTL("ADC Raw: %ld\n", adc_val);

    // Calculate percentage using integer arithmetic
    // Formula: (Current_Value - Min_Value) * 100 / Range
    int32_t percentage = ((int32_t)adc_val - BATTERY_ADC_MIN_VAL) * 100 / BATTERY_ADC_RANGE;

    // Clamp the value between 0% and 100%
    if (percentage > 100) percentage = 100;
    if (percentage < 0)   percentage = 0;

    PRINTL("Battery: %d%%\n", (int)percentage);

    // Check low battery threshold and update LED status
    if (percentage < BATTERY_LOW_THRESHOLD) {
        LED_R_ON();  
    } else {
        LED_R_OFF();
    }

    return (int)percentage;
}