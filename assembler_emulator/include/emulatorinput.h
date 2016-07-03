#ifndef _emulatorinput_h_
#define _emulatorinput_h_

#include "elfminformat.h"

typedef unsigned char Bit8u;
typedef unsigned short int Bit16u;
typedef unsigned int Bit32u;

typedef char Bit8;
typedef short int Bit16;
typedef int Bit32;

typedef struct{
	Bit8u* segments;
	Elf32_Phdr* phdr;
	int e_phnum;
	int segments_size;
} Elf32_Emu;

#endif
