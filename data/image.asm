
fast_blit24:
	bpget
	spget
	bpset
	push 0 ; allocate local x
	push 0 ; allocate local y
	push 0 ; allocate local c

	; while(y [Differs] h) { ... }
	; begin while
loop_y:
	get -6  ; argument h
	get 2  ; local y
	cmpi
	[ex(z)=1] jmp @loop_y_end
	; x := int32(0)
	push 0 
	set 1 ; local x

	; while(x [Differs] w) { ... }
	; begin while
loop_x:
	get -5  ; argument w
	get 1  ; local x
	cmpi
	[ex(z)=1] jmp @loop_x_end

	; determine color
	
	get 1  ; local x
	get -5  ; argument w
	get 2  ; local y
	mul 
	add  
	mul [i0:arg] 3

	get -2  ; argument source
	add 

	load8i
	shl 16
	
	get 1  ; local x
	get -5  ; argument w
	get 2  ; local y
	mul 
	add 
	mul [i0:arg] 3
	add [i0:arg] 1

	get -2  ; argument source
	add 

	load8i
	shl 8
	add
	
	get 1  ; local x
	get -5  ; argument w
	get 2  ; local y
	mul 
	add 
	mul [i0:arg] 3
	add [i0:arg] 2

	get -2  ; argument source
	add 

	load8i
	add 
	
	get -3  ; argument _x
	get 1  ; local x
	add 

	; determine pixel address
	in 0x40001
	get -4  ; argument _y
	get 2  ; local y
	add 
	mul 
	add 
	mul [i0:arg] 4

	in 0x40004
	add 

	store32i

	; x := x [Addition] int32(1)
	get 1  ; local x
	add [i0:arg] 1
	set 1 ; local x

	jmp @loop_x ; end while
loop_x_end:

	; y := y [Addition] int32(1)
	get 2  ; local y
	add [i0:arg] 1
	set 2 ; local y

	jmp @loop_y ; end while
loop_y_end:

	bpget
	spset
	bpset
	ret

:ORIGIN 0x160000

:VAR isometric FILE "isometric.raw"