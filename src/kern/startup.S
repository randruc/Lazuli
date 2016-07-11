	;; Startup
	;; Set up the runtime environment and run the kernel

	.global reset_handler
reset_handler:

	;; TODO: Switch off the watchdog (in case of a software reset)

	;; TODO: Disable interrupts
	
	;; Clear bss section
	;; TODO: Find a way to omit this code if no .bss
	ldi r26, lo8(_bss_start)
	ldi r27, hi8(_bss_start)
	ldi r16, _bss_size
clear_bss:	
	cpi r16, 0
	breq out_bss
	ldi r17, 0
	st X+, r17
	dec r16
	rjmp clear_bss
out_bss:	

	;; Load data section
	;; TODO: Find a way to omit this code if no .data
	ldi r30, lo8(_data_load_start)
	ldi r31, hi8(_data_load_start)
	ldi r26, lo8(_data_start)
	ldi r27, hi8(_data_start)
	ldi r16, _data_size
copy_data:	
	cpi r16, 0
	breq out_data
	lpm r17, Z+
	st X+, r17
	dec r16
	rjmp copy_data
out_data:	
	
	;; Set kernel stack pointer
	.equ spl, 0x3d
	.equ sph, 0x3e
	ldi r16, lo8(_ramend)
	ldi r17, hi8(_ramend)
	out spl, r16
	out sph, r17

	;; Start the kernel
	jmp kmain

	;; Parachute trap, infinite loop, should never be reached
	;; TODO: Should this be left here or thrown away ?
infinite_loop:
	rjmp infinite_loop
