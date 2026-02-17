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

// --- USART1 Definitions ---
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
#define USART_DMAR      (1 << 6) // DMA Enable Receiver bit
#define USART_DMAT      (1 << 7) // DMA Enable Transmitter bit

// --- DMA & Buffer Definitions ---
// Note: DMA2 Stream 7 Channel 4 = USART1_TX
//       DMA2 Stream 2 Channel 4 = USART1_RX
#define RX_BUFFER_SIZE  64
#define TX_BUFFER_SIZE  128
#define BT_STATE_PIN    8

// --- Global Variables ---
volatile uint8_t g_dma_rx_buffer[RX_BUFFER_SIZE];
static uint32_t  g_read_index = 0;
static uint8_t   g_dma_tx_buffer[TX_BUFFER_SIZE];

cmd_t g_bluetooth_last_cmd_received = CMD_STOP;

/**
 * @brief Initializes the Bluetooth module hardware (USART1, GPIOs, and DMA).
 * * Configures PA8 (State), PA9 (TX), and PA10 (RX). 
 * Sets up DMA2 Stream 2 for UART RX (Circular mode) and DMA2 Stream 7 for UART TX.
 * UART is configured at 9600 baud.
 */
void init_module_bluetooth(void) {
    // Enable Clocks (Bit 0 = GPIOA, Bit 22 = DMA2, Bit 4 = USART1)
    RCC->AHB1ENR |= (1 << 0) | (1 << 22); 
    RCC->APB2ENR |= (1 << 4); 

    // Configure GPIOs (PA9/PA10 for Alternate Function USART1)
    GPIOA->MODER &= ~((0x3 << 18) | (0x3 << 20)); 
    GPIOA->MODER |=  ((0x2 << 18) | (0x2 << 20)); 
    GPIOA->AFRH  &= ~((0xF << 4)  | (0xF << 8)); 
    GPIOA->AFRH  |=  ((0x7 << 4)  | (0x7 << 8)); 

    // Configure PA8 as Input with Pull-down to read Bluetooth state
    GPIOA->MODER &= ~(3 << (BT_STATE_PIN * 2));
    GPIOA->PUPDR &= ~(3 << (BT_STATE_PIN * 2)); // Reset
    GPIOA->PUPDR |=  (2 << (BT_STATE_PIN * 2)); // Set Pull-down

    // Configure DMA RX (DMA2 Stream 2)
    DMA2_Stream2->CR &= ~1; 
    while(DMA2_Stream2->CR & 1); // Wait for DMA to disable
    
    DMA2_Stream2->PAR  = (uint32_t)&USART1_DR;
    DMA2_Stream2->M0AR = (uint32_t)g_dma_rx_buffer;
    DMA2_Stream2->NDTR = RX_BUFFER_SIZE;            
    
    // Channel 4, High Priority, Circular mode, Memory Increment
    DMA2_Stream2->CR  = (4 << 25) | (2 << 16) | (1 << 8) | (1 << 10) | (1 << 6);
    DMA2_Stream2->CR &= ~(3 << 6); // Force DIR = 00 (Peripheral to Memory)
    DMA2_Stream2->CR |= 1;         // Enable RX DMA

    // Configure DMA TX (DMA2 Stream 7)
    DMA2_Stream7->CR &= ~1;
    while(DMA2_Stream7->CR & 1); // Wait for DMA to disable

    DMA2_Stream7->PAR  = (uint32_t)&USART1_DR;       // Destination: UART DR
    DMA2_Stream7->M0AR = (uint32_t)g_dma_tx_buffer;  // Source: TX Buffer
    
    // CHSEL[2:0] = 100 (Channel 4), MINC = 1 (Mem Inc), DIR[1:0] = 01 (Mem to Peripheral)
    DMA2_Stream7->CR = (4 << 25) | (1 << 10) | (1 << 6); 

    // Configure UART (9600 Bauds)
    USART1_BRR = 0x222E; 
    
    // Enable DMAR (RX) and DMAT (TX) triggers for USART
    USART1_CR3 |= USART_DMAR | USART_DMAT;

    // Enable UART, TX, and RX
    USART1_CR1 = USART_UE | USART_TE | USART_RE;
}

/**
 * @brief Sends a single character over Bluetooth using polling (blocking).
 * * @param c The character to send.
 */
void send_char(char c) {
    // Wait until the Transmit Data Register is empty (TXE)
    while (!(USART1_SR & USART_TXE));
    USART1_DR = c;
    bluetooth_trigger_activity();
}

/**
 * @brief Prints current DMA buffer state and pointers for debugging purposes.
 */
void bluetooth_print_dma_debug(void) {
    PRINTL("--- DMA DEBUG INFO ---\n");
    
    // Display remaining data transfers (NDTR)
    uint32_t remaining = DMA2_Stream2->NDTR;
    PRINTL("DMA Data remaining to transfer (NDTR): %d\n", remaining);
    
    PRINTL("Buffer Content (Non-zero):\n");
    for (int i = 0; i < RX_BUFFER_SIZE; i++) {
        // Only display non-empty slots
        if (g_dma_rx_buffer[i] != 0) {
            PRINTL("[%d]: 0x%02X (%d)\n", i, g_dma_rx_buffer[i], g_dma_rx_buffer[i]);
        }
    }
    PRINTL("----------------------\n");
    bluetooth_trigger_activity();
}

/**
 * @brief Polls the circular DMA buffer for new incoming Bluetooth commands.
 * * Compares the software read pointer with the hardware DMA write pointer.
 * Updates the global state if a valid command is found.
 */
void bluetooth_receive_cmd(void) {
    // Calculate current DMA write position
    uint32_t write_index = RX_BUFFER_SIZE - DMA2_Stream2->NDTR;
    bool has_received_data = false;

    // Process data until the read index catches up to the DMA write index
    while (g_read_index != write_index) {
        uint8_t data = g_dma_rx_buffer[g_read_index];
        has_received_data = true; 
        
        // Command processing
        if (data < CMD_MAX) {
            PRINTL("New cmd : %u\n", data);
            g_bluetooth_last_cmd_received = data;
        } else {
            WARNL("Invalid cmd : %u", data);
        }

        // Advance read index in circular mode
        g_read_index = (g_read_index + 1) % RX_BUFFER_SIZE;
    }

    if (has_received_data) {
        bluetooth_trigger_activity();
    }
}

/**
 * @brief Sends a null-terminated string over Bluetooth using DMA.
 * * @param str Pointer to the null-terminated string to send.
 * @note This function blocks if a previous DMA TX transfer is still running.
 */
void bluetooth_send_string(const char* str) {
    // Wait for any ongoing DMA TX transfer to finish
    while (DMA2_Stream7->CR & 1);
    
    // Copy the string into the DMA transmit buffer
    uint16_t len = 0;
    while (str[len] != '\0' && len < (TX_BUFFER_SIZE - 1)) {
        g_dma_tx_buffer[len] = str[len];
        len++;
    }

    // Clear Stream 7 Transfer Complete Interrupt Flag (CTCIF7)
    DMA2_HIFCR = (1 << 27);

    // Set number of bytes to transfer
    DMA2_Stream7->NDTR = len;

    // Enable DMA TX Stream
    DMA2_Stream7->CR |= 1;
    bluetooth_trigger_activity();
}

/**
 * @brief Retrieves the last valid command received via Bluetooth.
 * * @return The last command enumeration value.
 */
cmd_t bluetooth_get_last_cmd(void) {
    return g_bluetooth_last_cmd_received;
}

/**
 * @brief Checks if a device is currently connected to the Bluetooth module.
 * * @return true if connected (State pin is high), false otherwise.
 */
bool bluetooth_is_connected(void) {
    // Read the Input Data Register (IDR) at the specific state pin
    return (GPIOA->IDR & (1 << BT_STATE_PIN)) != 0;
}