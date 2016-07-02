;
; Simple memory transform-and-copy program
;
_start:

	push 0x1000
	cpget
	jmp @puts
	drop

	syscall
	
  push 0xFFFF00
	cpget
	jmp @cls
	hwio [ci:1] ; refresh screen
  drop 
	
  push 0x00FFFF
	cpget
	jmp @cls
	hwio [ci:1] ; refresh screen
  drop 
	
	jmp @_start
	syscall

puts:
	bpget
	spget
	bpset
	get -2 ; Get the string pointer
puts_copy:
	[i0:peek] load [ci:0]
	dup
	cmp 0
	[ex(z)=0] syscall [ci:1] [i0:pop]
	[ex(z)=0] [i0:arg] add 1
	[ex(z)=0] jmp @puts_copy
puts_stop:
	drop
	bpget
	spset
	bpset
	ret

	

cls:
	bpget
	spget
	bpset
	push 0
cls_copy:
	dup
	cmp 1228800 ; 640*480*4 screen size
	[ex(z)=1] jmp @cls_stop
	
	get -2 ; Get color argument
	get 1  ; Get iterator
	[i0:arg] add 4096 ; Add screen buffer offset
	
	storei [ci:2] ; Write 32bit value
	
	; Increase counter
	[i0:arg] add 4
	jmp @cls_copy

cls_stop:
	bpget
	spset
	bpset
	ret