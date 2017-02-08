    ;; Set up the interrupt vectors table

    .global interrupt_vectors_table
interrupt_vectors_table:
    ;; RESET - Reset, Entry point at boot
    .org 0x0000
    jmp reset_system

    ;; INT0 - External interrupt request 0
    .org 0x00004
    jmp int0_handler

    ;; INT1 - External interrupt request 1
    .org 0x00008
    jmp int1_handler

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
    .org 0x0038
    jmp timer0_compare_match_A_handler

    reti                         ; TIMER0COMPB  Timer/counter0 compare match B

    ;; TIMER0OVF - Timer/counter0 overflow
    .org 0x0040
    jmp timer0_overflow_handler

    reti                         ; SPISTC       SPI serial transfer complete
    reti                         ; USARTRX      USART Rx complete
    reti                         ; USARTUDRE    USART data register empty
    reti                         ; USARTTX      USART Tx complete
    reti                         ; ADC          ADC conversion complete
    reti                         ; EEREADY      EEPROM ready
    reti                         ; ANALOGCOMP   Analog comparator
    reti                         ; TWI          2-wire serial interface
    reti                         ; SPMREADY     Store program memory ready
