#ifndef _elfparser_h_
#define _elfparser_h_

#include "asmparser.h"
#include <stdio.h>
#include "elfminformat.h"
#include "my_exception.h"

class ElfParser : public AsmParser
{
public:
	ElfParser(char*);
	void parse();
	void printTables() const;
	~ElfParser();
private:
	FILE *elf_file;
	char* strtab;

};
#endif //_elfparser_h_
