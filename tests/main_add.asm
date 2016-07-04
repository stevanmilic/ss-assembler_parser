.extern add
.public main
.public stackinit
.public mykeypressed

.data.main
	.skip 0x100
stackinit:

.text

func: .long add
mykeypressed: .long 0

main:
	ldc r2,'0x1010'
	ldc r3,'0x200'
	ldc r5, 0
getchar:
	in r1, r2
	cmp r1,r3
	callne pc, getchar
	ldr r1, r5, mykeypressed
	out r5, r2
getchar2:
	in r4, r2
	cmp r4,r3
	callne pc, getchar2
	out r5, r2
	ldr r2, r5, mykeypressed
	ldr r4, r5, func
	call r4
	ldcl r1, '0x2000'
	out r3, r1
	int 0

.end


