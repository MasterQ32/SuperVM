jmp @_start
jmp @increment
ret
ret
ret
ret

_start:
	in 0x10000 
	[i0:pop] syscall [ci:2]
	[i0:arg] syscall [ci:1] '\n'

	push 1000;ms
	out 0x10002

	sei

loop:
	jmp @loop

	syscall
increment:
	load 0x4000
	[i0:arg] add 1
	[i0:peek] syscall [ci:2]
	[i0:arg] syscall [ci:1] 's'
	[i0:arg] syscall [ci:1] '\n'
	store 0x4000
	ret