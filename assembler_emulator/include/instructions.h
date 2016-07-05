#ifndef _instructions_h_
#define _instructions_h_

#include "interrupts.h"

#define INSTR_FLAG 0x00000008

#define FLAG_ZF 0x00000001
#define FLAG_OF 0x00000002
#define FLAG_CF 0x00000004
#define FLAG_NF 0x00000008

void init();
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
