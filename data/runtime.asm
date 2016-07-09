



;
; void puts(char *sz)
;
puts:
	bpget
	spget
	bpset
	get -2 ; sz
puts_loop:
	[i0:peek] load8i [f:yes]
	[ex(z)=1] jmp @puts_end
	; [i0:pop] syscall [ci:1]
	out 0x20000
	[i0:arg] add 1 
	jmp @puts_loop
puts_end:
	bpget
	spset
	bpset
	ret

puti:
	bpget
	spget
	bpset
	get -2 ; num




puti_end:
	bpget
	spset
	bpset
	ret