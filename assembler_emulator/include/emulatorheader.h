#ifndef _emulatorheader_h_
#define _emulatorheader_h_

typedef unsigned char Bit8u;
typedef unsigned short int Bit16u;
typedef unsigned int Bit32u;

typedef char Bit8;
typedef short int Bit16;
typedef int Bit32;

#define mask(no_bits, start_bit) (((1 << no_bits) - 1) << start_bit)
#define isolate_bits(value, no_bits, start_bit) (value & mask(no_bits,start_bit))
#define bits_to_value(value, no_bits, start_bit) ((value & mask(no_bits,start_bit)) >> start_bit)
#define sign_ext(value, start_bit) ((Bit32)value >> start_bit)

typedef union {
	Bit32 dword;
	Bit16 word[2];
	Bit8 byte[4];
} Gen32;

typedef union{
	Bit32u dword;
	Bit16u word[2];
	Bit8u byte[4];
} Gen32u;

typedef enum {
	R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,
	R11,R12,R13,R14,R15,PC,LR,SP,PSW
} Reg;

typedef enum{
	cond_eq, cond_ne, cond_gt, cond_ge,
	cond_lt, cond_le, reserved, cond_al
} Cond;

typedef enum{
	t_int, t_add, t_sub, t_mul, t_div, t_cmp,
	t_and, t_or, t_not, t_test, t_ldr_str, t_unknown,
	t_call, t_in_out, t_mov_shr_shl, t_ldc, t_skip,
	t_shr, t_shl
} Instr_OC;

struct LazyFlags{
	Gen32u var1,var2,res;
	Instr_OC type;
	Bit8u oldcf;
} lf;

#define MAX_MEM_SIZE 0x10000000
#define NO_REG 20

#define FLAG_ZF 0x00000001
#define FLAG_OF 0x00000002
#define FLAG_CF 0x00000004
#define FLAG_NF 0x00000008

Bit8u *mem;
Bit32u mem_size;
Gen32u cpu_regs[NO_REG];
Gen32u *ivt;
Bit8 change_flags;
Bit8u flags;

#endif
