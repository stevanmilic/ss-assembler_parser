.extern main
.extern stackinit
.extern mykeypressed

.data.ivt
	.long initinterrupt
	.long pswinterrupt
	.long instrinterrupt
	.long keyinterrupt
	.skip 44
	.long timerinterrupt

.text.ivt

keypressed: .long mykeypressed
timecounter: .long 0
stack: .long stackinit
start: .long main

initinterrupt:
	ldc r0, 0
	ldr sp, r0, stack
	strdb r0, sp, 0
	ldr lr, r0, start
	movs pc, lr, 0

keyinterrupt:
	strdb r0, sp, 0
	strdb r1, sp, 0
	strdb r2, sp, 0
	ldc r2, 0
	ldc r0, '0x1000'
	in r1, r0
	ldr r0, r2, keypressed
	str r1, r0
	ldria r2, sp, 0
	ldria r1, sp, 0
	ldria r0, sp, 0
	movs pc, lr, 0

timerinterrupt:
	strdb r0, sp, 0
	strdb r1, sp, 0
	ldc r1, 0
	ldr r0, r1, counter
	add r0, r0, 1
	str r0, r1, counter
	ldc r1, 0x2000
	out r0, r1
	ldria r1, sp, 0
	ldria r0, sp, 0
	movs pc, lr, 0

pswinterrupt:
	strdb r0, sp, 0
	strdb r1, sp, 0
	ldc r0, 19
	ldc r1, 0x2000
	out r0, r1
	movs pc, lr, 0

instrinterrupt:
	strdb r0, sp, 0
	strdb r1, sp, 0
	ldc r0, 0
	ldc r1, 0x2000
	out r0, r1
	movs pc, lr, 0
	
.end
