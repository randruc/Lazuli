	;; Set up the interrupt vectors table

	.global interrupt_vectors_table
interrupt_vectors_table:	
	rjmp reset_handler			; RESET        Reset, Entry point at boot
	rjmp reset_handler			; INT0         External interrupt request 0
	rjmp reset_handler			; INT1         External interrupt request 1
	rjmp reset_handler			; PCINT0       Pin change interrupt request 0
	rjmp reset_handler			; PCINT1       Pin change interrupt request 1
	rjmp reset_handler			; PCINT2       Pin change interrupt request 2
	rjmp reset_handler			; WDT          Watchdog time-out interrupt
	rjmp reset_handler			; TIMER2COMPA  Timer/counter2 compare match A
	rjmp reset_handler			; TIMER2COMPB  Timer/counter2 compare match B
	rjmp reset_handler			; TIMER2OVF    Timer/counter2 overflow
	rjmp reset_handler			; TIMER1CAPT   Timer/counter1 capture event
	rjmp reset_handler			; TIMER1COMPA  Timer/counter1 compare match A
	rjmp reset_handler			; TIMER1COMPB  Timer/counter1 compare match B
	rjmp reset_handler			; TIMER1OVF    Timer/counter1 overflow
	rjmp reset_handler			; TIMER0COMPA  Timer/counter0 compare match A
	rjmp reset_handler			; TIMER0COMPB  Timer/counter0 compare match B
	rjmp reset_handler			; TIMER0OVF    Timer/counter0 overflow
	rjmp reset_handler			; SPISTC       SPI serial transfer complete
	rjmp reset_handler			; USARTRX      USART Rx complete
	rjmp reset_handler			; USARTUDRE    USART data register empty
	rjmp reset_handler			; USARTTX      USART Tx complete
	rjmp reset_handler			; ADC          ADC conversion complete
	rjmp reset_handler			; EEREADY      EEPROM ready
	rjmp reset_handler			; ANALOGCOMP   Analog comparator
	rjmp reset_handler			; TWI          2-wire serial interface
	rjmp reset_handler			; SPMREADY     Store program memory ready
