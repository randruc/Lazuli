	;; Set up the interrupt vectors table

	.global interrupt_vectors_table
interrupt_vectors_table:
	.org 0x0000
	rjmp reset_system  ; RESET        Reset, Entry point at boot

	.org 0x0020
	rjmp int0_handler            ; INT0         External interrupt request 0
	reti                         ; INT1         External interrupt request 1
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
	rjmp timer0_compare_match_A  ; TIMER0COMPA  Timer/counter0 compare match A
	reti                         ; TIMER0COMPB  Timer/counter0 compare match B
	reti                         ; TIMER0OVF    Timer/counter0 overflow
	reti                         ; SPISTC       SPI serial transfer complete
	reti                         ; USARTRX      USART Rx complete
	reti                         ; USARTUDRE    USART data register empty
	reti                         ; USARTTX      USART Tx complete
	reti                         ; ADC          ADC conversion complete
	reti                         ; EEREADY      EEPROM ready
	reti                         ; ANALOGCOMP   Analog comparator
	reti                         ; TWI          2-wire serial interface
	reti                         ; SPMREADY     Store program memory ready
