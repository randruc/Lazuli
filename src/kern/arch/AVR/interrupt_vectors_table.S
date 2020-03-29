/**
 * @file interrupt_vectors_table.S
 * @brief Interrupt vectors table.
 * @date Jan 2017
 * @author Remi Andruccioli
 *
 * Set up the interrupt vectors table.
 */

    /**
     * Compute the right address of the interrupt vector, after the address
     * specified in the datasheet.
     *
     * @param X The address with which to compute the placement.
     */
#define INTERRUPT_VECTOR_ADDRESS(X) ((X) * 2)

    /**
     * Specify the placement of the interrupt vector.
     *
     * @param X The interrupt vector address.
     */
#define AT(X) .org INTERRUPT_VECTOR_ADDRESS(X)

    .section .ivt, "x"
    .global interrupt_vectors_table
interrupt_vectors_table:

    ;; RESET - Reset, Entry point at boot
    AT(0x0000)
    jmp reset_system

    ;; INT0 - External interrupt request 0
    AT(0x0002)
    jmp int0_handler

    ;; INT1 - External interrupt request 1
    AT(0x0004)
    jmp int1_handler

    ;; PCINT0 - Pin change interrupt request 0
    AT(0x0006)
    jmp pcint0_handler

    ;; PCINT1 - Pin change interrupt request 1
    AT(0x0008)
    jmp pcint1_handler

    ;; PCINT2 - Pin change interrupt request 2
    AT(0x000A)
    jmp pcint2_handler

    ;; WDT - Watchdog time-out interrupt
    AT(0x000C)
    jmp wdt_handler

    ;; TIMER2COMPA - Timer/counter2 compare match A
    AT(0x000E)
    jmp timer2compA_handler

    ;; TIMER2COMPB - Timer/counter2 compare match B
    AT(0x0010)
    jmp timer2compB_handler

    ;; TIMER2OVF - Timer/counter2 overflow
    AT(0x0012)
    jmp timer2ovf_handler

    ;; TIMER1CAPT - Timer/counter1 capture event
    AT(0x0014)
    jmp timer1capt_handler

    ;; TIMER1COMPA - Timer/counter1 compare match A
    AT(0x0016)
    jmp timer1compA_handler

    ;; TIMER1COMPB - Timer/counter1 compare match B
    AT(0x0018)
    jmp timer1compB_handler

    ;; TIMER1OVF - Timer/counter1 overflow
    AT(0x001A)
    jmp timer1ovf_handler

    ;; TIMER0COMPA - Timer/counter0 compare match A
    AT(0x001C)
    jmp timer0compA_handler

    ;; TIMER0COMPB - Timer/counter0 compare match B
    AT(0x001E)
    jmp timer0compB_handler

    ;; TIMER0OVF - Timer/counter0 overflow
    AT(0x0020)
    jmp timer0ovf_handler

    ;; SPISTC - SPI serial transfer complete
    AT(0x0022)
    jmp spistc_handler

    ;; USARTRX - USART Rx complete
    AT(0x024)
    jmp usartrx_handler

    ;; USARTUDRE - USART data register empty
    AT(0x0026)
    jmp usartudre_handler

    ;; USARTTX - USART Tx complete
    AT(0x0028)
    jmp usarttx_handler

    ;; ADC - ADC conversion complete
    AT(0x002A)
    jmp adc_handler

    ;; EEREADY - EEPROM ready
    AT(0x002C)
    jmp eeready_handler

    ;; ANALOGCOMP - Analog comparator
    AT(0x002E)
    jmp analogcomp_handler

    ;; TWI - 2-wire serial interface
    AT(0x0030)
    jmp twi_handler

    ;; SPMREADY - Store program memory ready
    AT(0x0032)
    jmp spmready_handler
