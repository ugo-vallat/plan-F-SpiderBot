#include <stm32f4/adc.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <tinyprintf.h>
#include <log.h>
#include <battery.h>

#define ADC_CHANNEL_1    1
#define RCC_AHB1ENR_GPIOAEN  (1 << 0)   
#define RCC_APB2ENR_ADC1EN   (1 << 8)

/* --- FONCTION D'INITIALISATION --- */
void init_module_battery(void) {
    PRINTL("[%s] ... ", __func__);

    // Activer les horloges
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  

    //Configurer PA1
    GPIOA->MODER |= (3 << (1 * 2)); 

    //Activer l'ADC
    ADC1->CR1 = 0; 
    ADC1->CR2 = 0;
    ADC1->CR2 |= ADC_ADON; 
    
    for(volatile int i=0; i<2000; i++);

    PRINTL("OK\n");
}


/* ---FONCTION DE MESURE --- */
int get_battery_percentage(void) {
    ADC1->SQR3 = ADC_CHANNEL_1;
    ADC1->CR2 |= ADC_SWSTART;
    int timeout = 10000;
    while (!(ADC1->SR & ADC_EOC)) {
        if (--timeout == 0) {
            PRINTL("ERREUR: ADC Timeout\n");
            return -1; 
        }
    }

    // Lire la valeur
    uint32_t adc_val = ADC1->DR;
    
    PRINTL("ADC Raw: %ld\n", adc_val);

    // Calcul du pourcentage
    // Calibrage :
    // - 8.4V (100%) ~= 3300 ADC
    // - 6.0V (0%)   ~= 2350 ADC
    // - Plage       = 950 points ADC
    
    int32_t val_min = 2350;
    int32_t plage   = 950;
    
    // Formule : (Valeur_Lue - Min) * 100 / Plage
    int32_t pourcentage = ((int32_t)adc_val - val_min) * 100 / plage;

    // Bornage (Clamping) pour rester entre 0 et 100
    if (pourcentage > 100) pourcentage = 100;
    if (pourcentage < 0)   pourcentage = 0;

    PRINTL("battery: %d%%\n", (int)pourcentage);

    return (int)pourcentage;
}