/**
 * @file uart.h
 * @brief Memory-mapped peripheral definitions and register structures for RP2350 UART0.
 *
 * Provides physical base addresses, MMIO register structures, atomic bitmask offsets,
 * peripheral configuration constants, and public API function declarations for UART0.
 *
 * SOURCES: RP2350 Datasheet (RP-008373-DS-3)
 * SECTIONS: 2.2.4 (Memory Map), 12.1 (UART / PL011)
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "serial.h"

/**
 * @name Peripheral Base Addresses (RP2350 Memory Map)
 * SECTION: 2.2.4, page 32
 * @{
 */
#define CLOCKS_BASE_ADDR		0x40010000UL /**< CLOCK_BASE eripheral base address */
#define RESETS_BASE_ADDR		0x40020000UL /**< RESETS peripheral base address */
#define IO_BANK0_BASE_ADDR		0x40028000UL /**< IO_BANK0 peripheral base address */
#define PADS_BANK0_BASE_ADDR	0x40038000UL /**< PADS_BANK0 peripheral base address */
#define UART0_BASE_ADDR			0x40070000UL /**< UART0 peripheral base address */
#define UART1_BASE_ADDR			0x40078000UL /**< UART1 peripheral base address */
/** @} */

/**
 * @brief System Clock Control Layout (CLOCKS).
 */
typedef struct {
    uint32_t reserved[18];        /**< 0x00 - 0x44: Reserved registers */
    volatile uint32_t CLK_PERI_CTRL; /**< 0x48: Peripheral clock control register */
} CLOCKS_Regs_t;

/**
 * @name Atomic Bit Manipulation Offsets
 * @{
 */
#define ATOMIC_XOR_OFFSET	0x1000U /**< Atomic XOR alias offset */
#define ATOMIC_SET_OFFSET	0x2000U /**< Atomic SET alias offset */
#define ATOMIC_CLR_OFFSET	0x3000U /**< Atomic CLR (clear) alias offset */
/** @} */

/**
 * @name UART Register Offsets
 * SECTION: 12.1.8, pages 972-983
 * @{
 */
#define UARTDR_OFFSET			0x000U /**< Data Register */
#define UARTRSR_OFFSET			0x004U /**< Receive Status / Error Clear Register */
#define UARTFR_OFFSET			0x018U /**< Flag Register */
#define UARTILPR_OFFSET			0x020U /**< IrDA Low-Power Counter Register */
#define UARTIBRD_OFFSET			0x024U /**< Integer Baud Rate Register */
#define UARTFBRD_OFFSET			0x028U /**< Fractional Baud Rate Register */
#define UARTLCR_H_OFFSET		0x02CU /**< Line Control Register */
#define UARTCR_OFFSET			0x030U /**< Control Register */
#define UARTIFLS_OFFSET			0x034U /**< Interrupt FIFO Level Select Register */
#define UARTIMSC_OFFSET			0x038U /**< Interrupt Mask Set/Clear Register */
#define UARTRIS_OFFSET			0x03CU /**< Raw Interrupt Status Register */
#define UARTMIS_OFFSET			0x040U /**< Masked Interrupt Status Register */
#define UARTICR_OFFSET			0x044U /**< Interrupt Clear Register */
#define UARTDMACR_OFFSET		0x048U /**< DMA Control Register */
#define UARTPERIPHID0_OFFSET	0xFE0U /**< Peripheral ID Register 0 */
#define UARTPERIPHID1_OFFSET	0xFE4U /**< Peripheral ID Register 1 */
#define UARTPERIPHID2_OFFSET	0xFE8U /**< Peripheral ID Register 2 */
#define UARTPERIPHID3_OFFSET	0xFECU /**< Peripheral ID Register 3 */
#define UARTPCELLID0_OFFSET		0xFF0U /**< PrimeCell ID Register 0 */
#define UARTPCELLID1_OFFSET		0xFF4U /**< PrimeCell ID Register 1 */
#define UARTPCELLID2_OFFSET		0xFF8U /**< PrimeCell ID Register 2 */
#define UARTPCELLID3_OFFSET		0xFFCU /**< PrimeCell ID Register 3 */
/** @} */

/**
 * @brief Subsystem Reset Control Register Layout (RESETS).
 */
typedef struct {
	volatile uint32_t RESET;		/**< 0x00: Reset control register */
	volatile uint32_t WDSEL;		/**< 0x04: Watchdog reset select */
	volatile uint32_t RESET_DONE;	/**< 0x08: Reset done status register */
} RESETS_Regs_t;

/**
 * @brief General Purpose Input/Output Bank 0 Control Layout (IO_BANK0).
 */
typedef struct {
	struct {
		volatile uint32_t STATUS; /**< Pin status register */
		volatile uint32_t CTRL;   /**< Pin control and function selection register */
	} GPIO[30];
} IO_BANK0_Regs_t;

/**
 * @brief Pad Control Bank 0 Register Layout (PADS_BANK0).
 */
typedef struct {
	volatile uint32_t VOLTAGE_SELECT; 	/**< 0x00: Voltage select register */
	volatile uint32_t PAD[30];	  		/**< Pad electrical configuration registers */
} PADS_BANK0_Regs_t;

/**
 * @brief ARM PrimeCell UART (PL011) Architecture Register Layout.
 * SECTION: 12.1.8, page 972
 */
typedef struct {
	volatile uint32_t UARTDR;	 	/**< 0x000: Data Register */
	volatile uint32_t UARTRSR;	 	/**< 0x004: Receive Status / Error Clear Register */
	uint32_t reserved0[4];		 	/**< 0x008 - 0x014: Reserved space */
	volatile uint32_t UARTFR;	 	/**< 0x018: Flag Register */
	uint32_t reserved1;		 		/**< 0x01C: Reserved space */
	volatile uint32_t UARTILPR;	 	/**< 0x020: IrDA Low-Power Counter Register */
	volatile uint32_t UARTIBRD;	 	/**< 0x024: Integer Baud Rate Register */
	volatile uint32_t UARTFBRD;	 	/**< 0x028: Fractional Baud Rate Register */
	volatile uint32_t UARTLCR_H;	/**< 0x02C: Line Control Register */
	volatile uint32_t UARTCR;	 	/**< 0x030: Control Register */
	volatile uint32_t UARTIFLS;	 	/**< 0x034: Interrupt FIFO Level Select Register */
	volatile uint32_t UARTIMSC;	 	/**< 0x038: Interrupt Mask Set/Clear Register */
	volatile uint32_t UARTRIS;	 	/**< 0x03C: Raw Interrupt Status Register */
	volatile uint32_t UARTMIS;	 	/**< 0x040: Masked Interrupt Status Register */
	volatile uint32_t UARTICR;	 	/**< 0x044: Interrupt Clear Register */
	volatile uint32_t UARTDMACR;	/**< 0x048: DMA Control Register */
	uint32_t reserved2[997];	 	/**< 0x04C - 0x0FDC: Reserved space */
	volatile uint32_t UARTPERIPHID0; /**< 0xFE0: Peripheral ID Register 0 */
	volatile uint32_t UARTPERIPHID1; /**< 0xFE4: Peripheral ID Register 1 */
	volatile uint32_t UARTPERIPHID2; /**< 0xFE8: Peripheral ID Register 2 */
	volatile uint32_t UARTPERIPHID3; /**< 0xFEC: Peripheral ID Register 3 */
	volatile uint32_t UARTPCELLID0;  /**< 0xFF0: PrimeCell ID Register 0 */
	volatile uint32_t UARTPCELLID1;  /**< 0xFF4: PrimeCell ID Register 1 */
	volatile uint32_t UARTPCELLID2;  /**< 0xFF8: PrimeCell ID Register 2 */
	volatile uint32_t UARTPCELLID3;  /**< 0xFFC: PrimeCell ID Register 3 */
} UART_Regs_t;

/**
 * @name Peripheral Access Instances
 * @{
 */
#define CLOCKS      ((CLOCKS_Regs_t *)(uintptr_t)CLOCKS_BASE_ADDR)
#define RESETS		((RESETS_Regs_t *)(uintptr_t)RESETS_BASE_ADDR)
#define RESETS_SET	((RESETS_Regs_t *)(uintptr_t)(RESETS_BASE_ADDR | ATOMIC_SET_OFFSET))
#define RESETS_CLR	((RESETS_Regs_t *)(uintptr_t)(RESETS_BASE_ADDR | ATOMIC_CLR_OFFSET))
#define UART0		((UART_Regs_t *)(uintptr_t)UART0_BASE_ADDR)
#define UART1		((UART_Regs_t *)(uintptr_t)UART1_BASE_ADDR)
#define IO_BANK0	((IO_BANK0_Regs_t *)(uintptr_t)IO_BANK0_BASE_ADDR)
#define PADS_BANK0	((PADS_BANK0_Regs_t *)(uintptr_t)PADS_BANK0_BASE_ADDR)
/** @} */

/**
 * @name UART Configuration Constants (115200 Baud @ 12 MHz clk_peri)
 * @{
 */
#define UART_IBAUD	6U								/**< Integer divisor part */
#define UART_FBAUD	33U								/**< Fractional divisor part */
#define UART_8N1	((uint32_t)3U << 5U)			/**< 8 data bits, no parity, 1 stop bit */
#define UART_FIFO	((uint32_t)1U << 4U)			/**< Enable hardware FIFOs */
#define UART_ENABLE	(((uint32_t)1U << 9U) | \
			 ((uint32_t)1U << 8U) | \
			 ((uint32_t)1U << 0U))					/**< Control bitmask: RXE | TXE | UARTEN */
/** @} */

/**
 * @name Subsystem Reset and GPIO Multiplexing Masks (RP2350 Architecture)
 * @{
 */
#define CLK_PERI_ENABLE_BIT     ((uint32_t)1U << 11U)   /**< RP2350 CLK_PERI enable bit */

/* Subsystem Reset Bit Positions (RP2350 Datasheet & Peripheral Map) */
#define RESETS_IO_BANK0_BIT     ((uint32_t)1U << 6U)    /**< Bit 6: IO_BANK0 */
#define RESETS_PADS_BANK0_BIT   ((uint32_t)1U << 10U)   /**< Bit 10: PADS_BANK0 */
#define RESETS_RESET_UART0_BIT  ((uint32_t)1U << 26U)   /**< Bit 26: UART0 (RP2350 Hardware Reset Bit) */
#define RESETS_RESET_UART1_BIT  ((uint32_t)1U << 27U)   /**< Bit 27: UART1 */

#define GPIO_FUNC_UART          2U                      /**< GPIO multiplexer function 2 (UART) */

#define PAD_TX_CONFIG           0x04U                   /**< Pad TX drive strength and slew rate */
#define PAD_RX_CONFIG           0x40U                   /**< Pad RX input enable configuration */

#define UART_FR_TXFF_BIT        ((uint32_t)1U << 5U)    /**< Transmit FIFO full status bit */
#define UART_FR_RXFE_BIT        ((uint32_t)1U << 4U)    /**< Receive FIFO empty status bit */

#define RESETS_UART0_BIT        RESETS_RESET_UART0_BIT  /**< Alias for UART0 reset bit */
#define RESETS_ALL_REQUIRED     (RESETS_RESET_UART0_BIT | \
                                 RESETS_IO_BANK0_BIT | \
                                 RESETS_PADS_BANK0_BIT) /**< Combined RP2350 reset mask (0x04000440U) */
/** @} */

__attribute__((always_inline)) static inline void uart_set_reset(void);
__attribute__((always_inline)) static inline int uart_reset_wait(void);
__attribute__((always_inline)) static inline void uart_init_gpio(void);
__attribute__((always_inline)) static inline void uart_configure(void);
__attribute__((always_inline)) static inline void uart_set_clk(void);

/**
 * @brief Initializes the UART0 hardware module and associated GPIO lines.
 *
 * @return 0 on successful initialization, -1 on hardware timeout.
 */
int uart_init(void);

__attribute__((always_inline)) static inline void uart_wait_tx_free_space(void);

/**
 * @brief Transmits a single byte over UART0 TX.
 *
 * @param[in] byte The 8-bit data payload to transmit.
 */
void uart_write_byte(uint8_t byte);

#endif /* UART_H */
