#include "defines.h"
#include <stdint.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/io.h>
#include <stm32f4/dma.h>    
#include <led.h>
#include <log.h>
#include <bluetooth.h>
#include <time.h>

// --- Definitions USART1 existantes ---
#define USART1_BASE     0x40011000
#define USART1_SR       _IOREG(USART1_BASE, 0x00)
#define USART1_DR       _IOREG(USART1_BASE, 0x04)
#define USART1_BRR      _IOREG(USART1_BASE, 0x08)
#define USART1_CR1      _IOREG(USART1_BASE, 0x0C)
#define USART1_CR3      _IOREG(USART1_BASE, 0x14) 

#define USART_TXE       (1 << 7)
#define USART_RXNE      (1 << 5)
#define USART_UE        (1 << 13)
#define USART_TE        (1 << 3)
#define USART_RE        (1 << 2)
#define USART_DMAR      (1 << 6) // Bit pour activer le mode DMA Receiver

// --- Definitions DMA TX (DMA2 Stream 7 Channel 4 = USART1_TX) ---
#define RX_BUFFER_SIZE  64
#define TX_BUFFER_SIZE  128
#define USART_DMAT      (1 << 7)

#define BT_STATE_PIN    8

// Tampon de réception et index de lecture
volatile uint8_t g_dma_rx_buffer[RX_BUFFER_SIZE];
static uint32_t g_read_index = 0;
static uint8_t g_dma_tx_buffer[TX_BUFFER_SIZE];

cmd_t g_bluetooth_last_cmd_received = CMD_STOP;


void init_module_bluetooth(void) {
    // Bit 0 = GPIOA, Bit 22 = DMA2
    RCC->AHB1ENR |= (1 << 0) | (1 << 22); 
    RCC->APB2ENR |= (1 << 4); // USART1

    // GPIO PA9/PA10 
    GPIOA->MODER &= ~((0x3 << 18) | (0x3 << 20)); 
    GPIOA->MODER |=  ((0x2 << 18) | (0x2 << 20)); 
    GPIOA->AFRH  &= ~((0xF << 4) | (0xF << 8)); 
    GPIOA->AFRH  |=  ((0x7 << 4) | (0x7 << 8)); 

    // GPIO PA8 pour indiquer l'état du Bluetooth
    GPIOA->MODER &= ~(3 << (BT_STATE_PIN * 2));
    GPIOA->PUPDR &= ~(3 << (BT_STATE_PIN * 2)); // Reset
    GPIOA->PUPDR |=  (2 << (BT_STATE_PIN * 2)); // Set Pull-down

    // ---------------- CONFIGURATION DMA RX (Stream 2) ----------------
    DMA2_Stream2->CR &= ~1; 
    while(DMA2_Stream2->CR & 1); 
    DMA2_Stream2->PAR = (uint32_t)&USART1_DR;
    DMA2_Stream2->M0AR = (uint32_t)g_dma_rx_buffer;
    DMA2_Stream2->NDTR = RX_BUFFER_SIZE;            
    // Channel 4, Prio High, Circular, MemInc
    DMA2_Stream2->CR = (4 << 25) | (2 << 16) | (1 << 8) | (1 << 10) | (1 << 6);
    DMA2_Stream2->CR &= ~(3 << 6); // Force DIR = 00
    DMA2_Stream2->CR |= 1; // Enable RX

    // ---------------- CONFIGURATION DMA TX (Stream 7) ----------------
    // Désactiver le stream avant config
    DMA2_Stream7->CR &= ~1;
    while(DMA2_Stream7->CR & 1);

    DMA2_Stream7->PAR = (uint32_t)&USART1_DR;       // Destination: UART DR
    DMA2_Stream7->M0AR = (uint32_t)g_dma_tx_buffer; // Source: Notre buffer TX
    // Config CR pour TX :
    // CHSEL[2:0] = 100 (Channel 4)
    // MINC       = 1   (Memory Increment)
    // DIR[1:0]   = 01  (Memory to Peripheral) !!! Important pour TX
    // TCIE       = 1   (Transfer Complete Interrupt - Optionnel, on ne l'utilise pas ici)
    DMA2_Stream7->CR = (4 << 25) | (1 << 10) | (1 << 6); 

    // 4. Configuration UART
    USART1_BRR = 0x222E; // 9600 Bauds
    
    // Activer DMAR (RX) et DMAT (TX)
    USART1_CR3 |= USART_DMAR | USART_DMAT;

    // Activer UART
    USART1_CR1 = USART_UE | USART_TE | USART_RE;
}

void send_char(char c) {
    while (!(USART1_SR & USART_TXE));
    USART1_DR = c;
    bluetooth_trigger_activity();
}
void bluetooth_print_dma_debug(void) {
    PRINTL("--- DMA DEBUG INFO ---\n");
    
    // Affiche la position courante du DMA (NDTR)
    uint32_t remaining = DMA2_Stream2->NDTR;
    PRINTL("DMA Data remaining to transfer (NDTR): %d\n", remaining);
    
    PRINTL("Buffer Content (Non-zero):\n");
    for (int i = 0; i < RX_BUFFER_SIZE; i++) {
        // On affiche seulement les cases qui ne sont pas vides
        if (g_dma_rx_buffer[i] != 0) {
            PRINTL("[%d]: 0x%02X (%d)\n", i, g_dma_rx_buffer[i], g_dma_rx_buffer[i]);
        }
    }
    PRINTL("----------------------\n");
    bluetooth_trigger_activity();
}
void bluetooth_receive_cmd(void) {
    // Calculer la position d'écriture actuelle du DMA.
    uint32_t write_index = RX_BUFFER_SIZE - DMA2_Stream2->NDTR;
    bool has_received_data = false;

    // Tant que notre index de lecture n'a pas rattrapé l'index d'écriture du DMA
    while (g_read_index != write_index) {
        
        uint8_t data = g_dma_rx_buffer[g_read_index];
        
        // Traitement de la commande
        if (data < CMD_MAX) {
            PRINTL("New cmd : %u\n", data);
            g_bluetooth_last_cmd_received = data;
        } else {
            WARNL("Invalid cmd : %u", data);
        }

        // Avancer l'index de lecture en mode circulaire
        g_read_index = (g_read_index + 1) % RX_BUFFER_SIZE;
    }
    if (has_received_data) {
        bluetooth_trigger_activity();
    }
}

/**
 * @brief Envoie une chaîne de caractères terminée par null
 */
void bluetooth_send_string(const char* str) {
    while (DMA2_Stream7->CR & 1);
    // Copie de la chaîne dans le buffer DMA
    uint16_t len = 0;
    while (str[len] != '\0' && len < (TX_BUFFER_SIZE - 1)) {
        g_dma_tx_buffer[len] = str[len];
        len++;
    }

    DMA2_HIFCR = (1 << 27);

    DMA2_Stream7->NDTR = len;

    // Activer le DMA
    DMA2_Stream7->CR |= 1;
    bluetooth_trigger_activity();
}

cmd_t bluetooth_get_last_cmd(void) {
    return g_bluetooth_last_cmd_received;
}

/**
 * @brief Vérifie si un appareil est connecté au Bluetooth
 * @return true (1) si connecté, false (0) sinon
 */
bool bluetooth_is_connected(void) {
    // Lire le registre d'entrée (IDR) bit 8
    if (GPIOA->IDR & (1 << BT_STATE_PIN)) {
        return true;  
    } else {
        return false;
    }
}