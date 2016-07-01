#include "elffile.h"

ElfFile::~ElfFile()
{
	fclose(file);
}
