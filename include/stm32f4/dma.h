/**
 * @file dma.h
 * @brief Direct Memory Access (DMA) register definitions for STM32F4.
 * * This file contains the memory map, register structures, and base 
 * addresses for configuring the DMA1 and DMA2 controllers.
 */

#ifndef STM32F4_DMA_H
#define STM32F4_DMA_H

#include <stdint.h>

/* ========================================================================= *
 * DMA STREAM STRUCTURE                                                      *
 * ========================================================================= */

/**
 * @brief Structure representing the registers of a single DMA Stream.
 */
typedef struct {
    volatile uint32_t CR;     /*!< DMA stream x configuration register      */
    volatile uint32_t NDTR;   /*!< DMA stream x number of data register     */
    volatile uint32_t PAR;    /*!< DMA stream x peripheral address register */
    volatile uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
    volatile uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
    volatile uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;


/* ========================================================================= *
 * DMA CONTROLLER BASE ADDRESSES                                             *
 * ========================================================================= */

/**
 * @brief Base memory addresses for DMA1 and DMA2 controllers.
 */
#define DMA1_BASE             (0x40026000UL)
#define DMA2_BASE             (0x40026400UL)


/* ========================================================================= *
 * DMA2 STREAM BASE ADDRESSES                                                *
 * ========================================================================= */

/**
 * @brief Memory addresses for individual DMA2 streams.
 * Streams start at offset 0x10 from the base controller address.
 * Each stream register block occupies 0x18 bytes (24 bytes).
 */
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)


/* ========================================================================= *
 * DMA2 STREAM POINTERS                                                      *
 * ========================================================================= */

/**
 * @brief Pointers to access the DMA structures directly in memory.
 * Casts the base addresses into the DMA_Stream_TypeDef structure.
 */
#define DMA2_Stream0          ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1          ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2          ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3          ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4          ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5          ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6          ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7          ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)


/* ========================================================================= *
 * DMA GLOBAL INTERRUPT REGISTERS                                            *
 * ========================================================================= */

/**
 * @brief Global interrupt registers (Common to all streams of the controller).
 */
#define DMA2_LISR             (*(volatile uint32_t *)(DMA2_BASE + 0x00)) /*!< Low Interrupt Status Register       */
#define DMA2_HISR             (*(volatile uint32_t *)(DMA2_BASE + 0x04)) /*!< High Interrupt Status Register      */
#define DMA2_LIFCR            (*(volatile uint32_t *)(DMA2_BASE + 0x08)) /*!< Low Interrupt Flag Clear Register   */
#define DMA2_HIFCR            (*(volatile uint32_t *)(DMA2_BASE + 0x0C)) /*!< High Interrupt Flag Clear Register  */

#endif // STM32F4_DMA_H