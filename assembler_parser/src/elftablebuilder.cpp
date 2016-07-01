#include "elftablebuilder.h"

ElfTableBuilder::~ElfTableBuilder()
{
	delete elf_file;
}

std::ostream& ElfTableBuilder::dump(std::ostream& o) const
{
	o << *elf_file;
	return o;
}

void ElfTableBuilder::writeTable()
{
	elf_file->operation();
}
