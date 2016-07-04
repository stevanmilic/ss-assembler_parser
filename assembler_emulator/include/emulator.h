#ifndef _emulator_h_
#define _emulator_h_

#include "instructions.h"

int emulator(Bit8u*, unsigned int);
Gen32u fetchDWORD(); 
int decodeInstruction(Bit8);
void executeInstruction(Instr_OC, Gen32u);
void create_io_threads();

#endif
