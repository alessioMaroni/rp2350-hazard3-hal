#include "uart.h"
#include "helpers.h"

#include <stdint.h>

#define UART_TIMEOUT_CYCLES 1000000U

/**
 * @brief Releases UART0, IO_BANK0, and PADS_BANK0 subsystems from reset.
 *
 * Drives the hardware reset register high and low to force a clean peripheral reset.
 */
__attribute__((always_inline)) static inline void uart_set_reset(void)
{
	// cppcheck-suppress misra-c2012-11.4
	RESETS->RESET |= RESETS_ALL_REQUIRED;

	for (volatile int i = 0; i < 100; i++) {
		arch_asm_nop();
	}

	// cppcheck-suppress misra-c2012-11.4
	RESETS->RESET &= ~RESETS_ALL_REQUIRED;

	ASSERT((RESETS->RESET & RESETS_ALL_REQUIRED) == 0U);
}

/**
 * @brief Polls until the peripheral reset cycle completes (RESET_DONE high).
 *
 * Decrements a hardware timeout counter while performing pipeline delays.
 * Logs the completion state to SIO Scratchpad 1.
 *
 * @return 0 on success, -1 on timeout.
 */
__attribute__((always_inline)) static inline int uart_reset_wait(void)
{
	uint32_t timeout = UART_TIMEOUT_CYCLES;

	while (((RESETS->RESET_DONE & RESETS_ALL_REQUIRED) != RESETS_ALL_REQUIRED) && (timeout > 0U)) {
		timeout--;
		arch_asm_nop();
	}

	*SIO_SCRATCH1 = RESETS->RESET_DONE;

	ASSERT(timeout > 0U);

	return ((RESETS->RESET_DONE & RESETS_ALL_REQUIRED) == RESETS_ALL_REQUIRED) ? 0 : -1;
}

/**
 * @brief Configures GPIO multiplexer functions and pad electrical settings for UART0.
 *
 * Selects hardware Function 2 (UART) on GPIO 0 (TX) and GPIO 1 (RX) and applies
 * pad drive strength, slew rate, and input enable parameters.
 */
__attribute__((always_inline)) static inline void uart_init_gpio(void)
{
	// cppcheck-suppress misra-c2012-11.4
	IO_BANK0->GPIO[0].CTRL = GPIO_FUNC_UART;
	// cppcheck-suppress misra-c2012-11.4
	IO_BANK0->GPIO[1].CTRL = GPIO_FUNC_UART;

	// cppcheck-suppress misra-c2012-11.4
	PADS_BANK0->PAD[0] = PAD_TX_CONFIG;
	// cppcheck-suppress misra-c2012-11.4
	PADS_BANK0->PAD[1] = PAD_RX_CONFIG;

	ASSERT((IO_BANK0->GPIO[0].CTRL & 0x1FU) == GPIO_FUNC_UART);
}

/**
 * @brief Configures baud rate divisors, line control format, and enables UART0.
 *
 * Disables the controller via Control Register (UARTCR), sets Integer (UARTIBRD) and
 * Fractional (UARTFBRD) divisors, configures 8N1 frame format with FIFOs enabled (UARTLCR_H),
 * and enables the peripheral, TX, and RX modules.
 */
__attribute__((always_inline)) static inline void uart_configure(void)
{
	ASSERT(UART_IBAUD > 0U);

	// cppcheck-suppress misra-c2012-11.4
	UART0->UARTCR = 0U;

	// cppcheck-suppress misra-c2012-11.4
	UART0->UARTIBRD = UART_IBAUD;
	// cppcheck-suppress misra-c2012-11.4
	UART0->UARTFBRD = UART_FBAUD;

	// cppcheck-suppress misra-c2012-11.4
	UART0->UARTLCR_H = UART_8N1 | UART_FIFO;

	// cppcheck-suppress misra-c2012-11.4
	UART0->UARTCR = UART_ENABLE;

	ASSERT((UART0->UARTCR & 1U) != 0U);
}

// TODO: Document
__attribute__((always_inline)) static inline void uart_set_clk(void)
{
	CLOCKS->CLK_PERI_CTRL = CLK_PERI_ENABLE_BIT;
}

int uart_init(void)
{
	int ret;

	uart_set_clk();
	uart_set_reset();
	ret = uart_reset_wait();

	if (ret != 0) {
		*SIO_SCRATCH0 = SIO_FAULT_TIMEOUT;
		ret = -1; /* Hardware timeout */
	} else {
		uart_init_gpio();
		uart_configure();
	}

	ASSERT(ret == 0);

	return ret;
}

/**
 * @brief Polls until space becomes available in the UART0 Transmit FIFO.
 *
 * Checks the Transmit FIFO Full (TXFF) flag in the Flag Register (UARTFR) and
 * issues NOP assembly instructions during wait states.
 */
__attribute__((always_inline)) static inline void uart_wait_tx_free_space(void)
{
	ASSERT((UART0->UARTCR & 1U) != 0U);

	while ((UART0->UARTFR & UART_FR_TXFF_BIT) != 0U) {
		arch_asm_nop();
	}
}

void uart_write_byte(uint8_t byte)
{
	ASSERT((UART0->UARTCR & ((uint32_t)1U << 8U)) != 0U);

	uart_wait_tx_free_space();

	UART0->UARTDR = (uint32_t)byte;

	ASSERT((UART0->UARTCR & 1U) != 0U);
}
