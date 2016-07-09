; includes a memory specification file
:INCLUDE "timer-test.mem"

; entry point
:ORIGIN 0x0000
	jmp @start

	; interrupt list
	jmp @int_timer ; 1
	ret ; 2
	ret ; 3
	ret ; 4
	ret ; 5
	ret ; 6
	ret ; 7
	ret ; 8

start:
	push $strFreq
	cpget
	jmp @puts
	drop
	
	in 0x10000
	cpget
	jmp @puti	
	out 0x10002 ; Timer Limit
	
	push $strHz
	cpget
	jmp @puts
	drop
	
	sei
	
mainloop:
	jmp @mainloop
	
	syscall

int_timer:
	
	load32 $iCounter
	cpget
	jmp @puti
	[i0:arg] add 1
	store32 $iCounter

	push $strTicks
	cpget
	jmp @puts

	ret
