	jmp @start

	; interrupt list
	ret ; 1
	ret ; 2
	ret ; 3
	ret ; 4
	ret ; 5
	ret ; 6
	ret ; 7
	ret ; 8

; wait for incoming connection and input
start:
	in 0x20000 [f:yes] [r:discard]
	[ex(n)=1] jmp @start
	
	push $welcomeText
	cpget
	jmp @puts
	drop

	in 0x40001 ; Width
	cpget
	jmp @puti
	drop
	
	push ' '
	out 0x20000
	
	push 'x'
	out 0x20000
	
	push ' '
	out 0x20000
	
	in 0x40002 ; Height
	cpget
	jmp @puti
	drop
	
	push '\n'
	out 0x20000

mainloop:
	jmp @mainloop

:INCLUDE "runtime.asm"

:VAR welcomeText ZSTRING "Welcome to the GPU test system:\nResolution: "

:ORIGIN 0x180000
:VAR image FILE "sexy.raw"