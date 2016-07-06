jmp @_start
jmp @increment
ret
ret
ret
ret

_start:
;	hwio [ci:0] [i0:arg] 0x10000 [r:push]
;	[i0:pop] syscall [ci:2]

	push 1000;ms
	hwio [ci:1] [i0:arg] [i1:pop] 0x10002

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