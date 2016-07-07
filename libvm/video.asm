; void cls(color)
; Clears the screen
; color: The color of the screen
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
	
	store32i
	
	; Increase counter
	[i0:arg] add 4
	jmp @cls_copy

cls_stop:
	bpget
	spset
	bpset
	ret

; void blit24(ptr, x, y, w, h)
; Blits a 24 bit bitmap to the screen
; ptr: pointer to the raw bitmap
; x:   The x coordinate to blit to
; y:   The y coordinate to blit to
; w:   The width of the bitmap
; h:   The height of the bitmap
blit24:
	bpget
	spget
	bpset
	push 0 ; py
	push 0 ; px
	get -2 ; uint8_t *it
	push 0 ; tmp
blit24_loop_y:
	; px = 0
	push 0
	set 2
	
blit24_loop_x:

	; tmp = (*it<<16)
	get 3 ; get it
	[i0:peek] load8i ; *it
	[i0:arg] shl 16
	set 4
	[i0:arg] add 1 ; it++
	
	; tmp |= (*it++ << 8)
	[i0:peek] load8i; *it
	[i0:arg] shl 8
	get 4 
	or
	set 4 ; 
	[i0:arg] add 1 ; it++
	
	; tmp |= (*it++)
	[i0:peek] load8i ; *it
	get 4 
	or
	; color is now loaded completly
	; calculate target address
	; addr = 0x1000 + 4 * (x + px + pitch * (y+py))
	get -4
	get 1
	add
	[i0:arg] mul 640
	get -3
	add
	get 2
	add
	[i0:arg] mul 4
	[i0:arg] add 0x1000
	store32i ; write 32bit bal
	
	[i0:arg] add 1 ; it++
	set 3

blit24_loop_xe:
	; px++
	get 2
	[i0:arg] add 1
	dup
	set 2
	; if(px == w)
	get -5 ; w
	cmpi
	; break;
	[ex(z)=0] jmp @blit24_loop_x

blit24_loop_ye:
	; py++
	get 1
	[i0:arg] add 1
	dup
	set 1
	; if(py == h)
	get -6 ; h
	cmpi
	; break;
	[ex(z)=0] jmp @blit24_loop_y
	
blit24_stop:
	;	return
	bpget
	spset
	bpset
	ret