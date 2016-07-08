jmp @start
jmp @int_timer ; 1
ret ; 2
ret ; 3
ret ; 4
ret ; 5
ret ; 6
ret ; 7
ret ; 8

start:
	push 0x1000
	cpget
	jmp @puts
	drop
	
	in 0x10000 ; Timer Frequency
	[i0:peek] syscall [ci:2]
	
	out 0x10002 ; Timer Limit
	
	push 0x1012 ; " Hz\n"
	cpget
	jmp @puts
	drop
	
	sei
	
mainloop:
	jmp @mainloop
	
	syscall

int_timer:
	
	load32 0x101F
	[i0:peek] syscall [ci:2]
	[i0:arg] add 1
	store32 0x101F
	
	push 0x1017 ; "tick\n"
	cpget
	jmp @puts

	ret