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
	cpget
	jmp @main
	syscall
