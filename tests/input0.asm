.text
.extern foo, goo
.public function, main

.long 10
.long '0x100', main - function + 10, main + 10, main - foo - goo + function + bonjour

data:.long 40, 50, 60, 90

function:
	ldc r2,'0x1000'
	in r1, r2
	out r2, r2
	mov r3, r2
	shr r4, r2, 31
	shr r5, r2, 1

Interrupt:
	int 5
	int 0xa
	int 0
		
Arithmetic:
	add r1, r1
	add r1, -200
	add r1, 500
	add r1, main - function + bonjour
	addeq r1,r1
	adds r1,r2
	addnes r2,r2
	add r1, main - 10
	add sp, 8
	add lr, 8
	add pc, 8

Logical:
	and r1,r1
	and sp, r1

MemoryAccess:
	ldr r1, r2
	ldr r1, r2, 100
	ldr r1, r2, main - 5
	ldria r1, r2
	ldribeq r1, r2
	ldrdanes r1, r2
	ldr r1, r2, data+main 

FunctionCall:
	call r1, 100
	call r1, -100
	call r1, main - function
	call r1, 10 - function
	call r2, '0x10000'

IO:
	in r1, r2
	out r1, r2
	in r1, psw

main:
	ldc r1, 100
	ldc r2, '0x20'
	ldc r3, -200
	ldc r4, 2 - main + 1
	ldc r4, main - function
	add r1, r2
	add r3, 200
	add pc, 200
	call r2, 100
	ldr r2, r3
	ldr r2, r4
	ldr r2, r1, data
	ldr r2, r3, data + 8
	and r2, r3
	andeqs r2,r2
	addnes r2,r3
	mulgts r4,r5

.data.first
.skip 0x20
.char 10
.word 20
.align 16
bonjour:
.bss.first
.end
