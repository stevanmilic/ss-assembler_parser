.extern endadd
.public add
.data
value: .long '0x10'
.text.add
endfunc: .long endadd
add:
	ldc r3, 0
	add r3, r1
	add r3, r2
	ldc r5, 0
	ldr r4, r5, endfunc
	call r4
.end
