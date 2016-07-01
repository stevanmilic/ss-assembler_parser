#include <ostream>
#include <iomanip>
#include "execelffile.h"
#include <sstream>

ExecElfFile::~ExecElfFile()
{
}

void ExecElfFile::operation()
{
	firstOperation();
	secondOperation();
}

std::ostream& ExecElfFile::dump(std::ostream& o) const
{
	for(std::vector<Elf32_Phdr>::const_iterator it = phdr.begin(); it != phdr.end(); ++it) {
		o << "| " << (*it).p_type << " | " << (*it).p_offset << " | " << " | " << (*it).p_filesz << " | " << (*it).p_flags << " | " << (*it).p_align << " |\n";
	}
	return o;
}
