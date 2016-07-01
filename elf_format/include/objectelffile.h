#ifndef _objectelffile_h_
#define _objectelffile_h_

#include <vector>
#include "elffile.h"

class ObjectElfFile : public ElfFile
{
public:
	void operation();
	std::ostream& dump(std::ostream&) const;
	virtual ~ObjectElfFile();
protected:
	virtual void firstOperation() = 0;
	virtual void secondOperation() = 0;
	virtual void thirdOperation() = 0;
	virtual void fourthOperation() = 0;
	virtual void fifthOperation() = 0;
	int getStringIndex(std::string name) const;
	int getSectionIndex(int sh_name) const;
	int findSectionIndex(std::string) const;
	std::vector<Elf32_Sym> symtab;
	std::vector<std::vector<char>> sections;
	std::vector<std::vector<Elf32_Rel>> reltabs;
	std::vector<Elf32_Shdr> shdr;
	std::vector<std::string> strtab;
};

#endif
