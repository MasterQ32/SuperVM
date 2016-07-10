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
	; simple echo from COM1 ←→ COM2
	
	in 0x20000 [f:yes]
	[ex(n)=0] out 0x30000
	[ex(n)=1] drop
	
	in 0x30000 [f:yes]
	[ex(n)=0] out 0x20000
	[ex(n)=1] drop
	
	jmp @start