#ifndef _emulatorinput_h_
#define _emulatorinput_h_

#include "elfminformat.h"

typedef struct{
	Bit8u* segments;
	Elf32_Phdr* phdr;
	int e_phnum;
	int segments_size;
} Elf32_Emu;

#endif
