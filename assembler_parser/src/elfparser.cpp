#include "elfparser.h"

ElfParser::ElfParser(char *elf_file_name)
{
	elf_file = fopen(elf_file_name , "wb");
	if (!elf_file) {
		throw MyException("Can't create out file!", 0);
	}
}

ElfParser::~ElfParser()
{
	fclose(elf_file);
}
