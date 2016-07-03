#ifndef _emulator_h_
#define _emulator_h_

#include "instructions.h"

int emulator(Elf32_Emu);
Gen32u fetchDWORD(); 
int decodeInstruction(Bit8);
int executeInstruction(Instr_OC, Gen32u);
void CPU_Core_Run(void);

#endif
