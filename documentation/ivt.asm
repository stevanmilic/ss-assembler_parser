.extern main
.extern stackInit
.data.ivt
	.long initInterrupt
	.skip 60

.text.ivt

stack: .long stackInit
start: .long main

initInterrupt:
	ldc r0, 0
	ldr sp, r0, stack
	strdb r0, sp, 0
	ldr lr, r0, start
	strdb lr, sp, 0
	ldria pc, sp, 0
.end
