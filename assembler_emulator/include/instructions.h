#ifndef _instructions_h_
#define _instructions_h_

#include "emulatorheader.h"

#define INSTR_FLAG 0x00000008

typedef enum{
	cond_eq, cond_ne, cond_gt, cond_ge,
	cond_lt, cond_le, reserved, cond_al
} Cond;

typedef enum{
	t_int, t_add, t_sub, t_mul, t_div, t_cmp,
	t_and, t_or, t_not, t_test, t_ldr_str,
	t_call = 12, t_in_out, t_mov_shr_shl, t_ldc,
	t_unknown, t_shr, t_shl
} Instr_OC;

struct LazyFlags{
	Gen32u var1,var2,res;
	Instr_OC type;
	Bit8u oldcf;
} lf;

//TO DO: lazy flag stuff
int checkCondition(Cond);
void fillFlags();
int get_ZF();
int get_NF();
int get_OF();
int get_CF();

void intInstruction(Bit8u);
void add_srcInstruction(Bit8u, Bit8u);
void sub_srcInstruction(Bit8u, Bit8u);
void mul_srcInstruction(Bit8u, Bit8u);
void div_srcInstruction(Bit8u, Bit8u);
void cmp_srcInstruction(Bit8u, Bit8u);
void add_immInstruction(Bit8u, Bit32);
void sub_immInstruction(Bit8u, Bit32);
void mul_immInstruction(Bit8u, Bit32);
void div_immInstruction(Bit8u, Bit32);
void cmp_immInstruction(Bit8u, Bit32);
void andInstruction(Bit8u, Bit8u);
void orInstruction(Bit8u, Bit8u);
void notInstruction(Bit8u, Bit8u);
void testInstruction(Bit8u, Bit8u);
void ldrstrInstruction(Bit8u, Bit8u, Bit8u, Bit8u, Bit32);
void callInstruction(Bit8u, Bit32);
void ioInstruction(Bit8u, Bit8u, Bit8u);
void moveInstruction(Bit8u, Bit8u, Bit8u, Bit8u);
void ldcInstruction(Bit8u, Bit8u, Bit16);

#endif
