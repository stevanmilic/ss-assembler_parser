.extern sab
.public main
main:
	ldc r2,0x1000
	in r1, r2
	ldc r3,0x1000
	in r2, r3
	call sab
end_sab:
	ldc r4,0x1010
	out r3, r4
.end


