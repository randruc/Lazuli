	;;
	;; startup.s
	;; Set up the runtime environment and run the kernel
	;;

	;; Frequently used registers
	.equ spl,  0x3d
	.equ sph,  0x3e
	.equ sreg, 0x3f

	;;
	;; Main entry point of the whole system.
	;;
	.global reset_system
reset_system:

	;; TODO: Switch off the watchdog (in case of a software reset)

	;; Disable interrupts
	cli
	
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
	ldi r16, lo8(_ramend)
	ldi r17, hi8(_ramend)
	out spl, r16
	out sph, r17

	;; Start the kernel
	jmp KMain

	;; Parachute trap, infinite loop, should never be reached
	;; TODO: Should this be left here or thrown away ?
infinite_loop:
	rjmp infinite_loop

	.global GlobalInterruptsEnable
GlobalInterruptsEnable:
	sei
	ret

	.global GlobalInterruptsDisable
GlobalInterruptsDisable:
	cli
	ret
	
	.global int0_handler
int0_handler:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	in r16, sreg
	push r16
	
	call Int0Handler
	
	pop r16
	out sreg, r16	
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	
	reti
