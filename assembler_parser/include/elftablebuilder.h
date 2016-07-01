#ifndef _elftablebuilder_h_
#define _elftablebuilder_h_

#include "tablebuilder.h"
#include "elffile.h"

class ElfTableBuilder : public TableBuilder
{
public:
	virtual bool resolveToken(Token* token) = 0;
	virtual ~ElfTableBuilder();
	std::ostream& dump(std::ostream& o) const;
	void writeTable();
protected:
	ElfFile* elf_file;
};

#endif
