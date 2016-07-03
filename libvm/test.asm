;
; Interrupts
;
jmp @_start
jmp @interrupt1
jmp @interrupt2
jmp @interrupt3
jmp @interrupt4
jmp @interrupt5

;
; Entry point
;
_start:

;	sei

;	push 0x1000
;	cpget
;	jmp @puts
;	drop
	
	; Print a '1'
;	int 1
	
  push 0x000044 ; dark blue
	cpget
	jmp @cls
  drop 
	
	push 92 ; height
	push 92  ; width
	push 200 ; y
	push 50  ; x
	push 0x12D034
	cpget
	jmp @blit24
	drop
	drop
	drop
	drop
	drop

	push 92 ; height
	push 92  ; width
	push 150 ; y
	push 200  ; x
	push 0x12D034
	cpget
	jmp @blit24
	drop
	drop
	drop
	drop
	drop
	
	hwio [ci:1] ; refresh screen
	
	syscall
	
_loop:
	
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
	
	jmp @_loop
	syscall

interrupt1:
	syscall [ci:1] [i0:arg] '1'
	jmp @interrupt
interrupt2:
	syscall [ci:1] [i0:arg] '2'
	jmp @interrupt
interrupt3:
	syscall [ci:1] [i0:arg] '3'
	jmp @interrupt
interrupt4:
	syscall [ci:1] [i0:arg] '4'
	jmp @interrupt
interrupt5:
	syscall [ci:1] [i0:arg] '5'
	jmp @interrupt
	
interrupt:
	syscall [ci:1] [i0:arg] '\n'
	ret
	
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