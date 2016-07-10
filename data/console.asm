  jmp @start
  ret
  ret
  ret
  ret
  ret
  ret
  ret
  ret

start:
	; simple echo
	
	in 0x20000 [f:yes]
	[ex(n)=0] out 0x20000
	[ex(n)=1] drop
	
	jmp @start