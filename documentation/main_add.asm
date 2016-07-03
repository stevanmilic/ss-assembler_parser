.extern add
.public endadd
.public main
.public stackInit
.data.main
stackInit:
	.skip 0x1000
.text
func: .long add
main:
	ldria psw, sp, 0
	ldc r2,'0x1000'
	in r1, r2
	ldc r3,'0x1000'
	in r2, r3
	ldc r5,0
	ldr r4,r5,func
	call r4
endadd:
	ldc r4,'0x1010'
	out r3, r4
	int 0
.end


