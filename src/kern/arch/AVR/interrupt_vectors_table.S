/**
 * @file interrupt_vectors_table.S
 *
 * Set up the interrupt vectors table.
 */

    /**
     * Compute the right address of the interrupt vector, after the address
     * specified in the datasheet.
     *
     * @param X The address with which to compute the placement.
     */
#define INTERRUPT_VECTOR_ADDRESS(X) (X * 2)

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
    reti

    reti                         ; PCINT0       Pin change interrupt request 0
    reti                         ; PCINT1       Pin change interrupt request 1
    reti                         ; PCINT2       Pin change interrupt request 2
    reti                         ; WDT          Watchdog time-out interrupt
    reti                         ; TIMER2COMPA  Timer/counter2 compare match A
    reti                         ; TIMER2COMPB  Timer/counter2 compare match B
    reti                         ; TIMER2OVF    Timer/counter2 overflow
    reti                         ; TIMER1CAPT   Timer/counter1 capture event
    reti                         ; TIMER1COMPA  Timer/counter1 compare match A
    reti                         ; TIMER1COMPB  Timer/counter1 compare match B
    reti                         ; TIMER1OVF    Timer/counter1 overflow

    ;; TIMER0COMPA - Timer/counter0 compare match A
    AT(0x001c)
    jmp timer0_compare_match_A_handler

    reti                         ; TIMER0COMPB  Timer/counter0 compare match B

    ;; TIMER0OVF - Timer/counter0 overflow
    AT(0x0020)
    reti

    reti                         ; SPISTC       SPI serial transfer complete
    reti                         ; USARTRX      USART Rx complete
    reti                         ; USARTUDRE    USART data register empty
    reti                         ; USARTTX      USART Tx complete
    reti                         ; ADC          ADC conversion complete
    reti                         ; EEREADY      EEPROM ready
    reti                         ; ANALOGCOMP   Analog comparator
    reti                         ; TWI          2-wire serial interface
    reti                         ; SPMREADY     Store program memory ready
