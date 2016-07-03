#ifndef _execelffile_h_
#define _execelffile_h_

#include <vector>
#include "elffile.h"

class ExecElfFile : public ElfFile
{
public:
	void operation();
	virtual std::ostream& dump(std::ostream&) const;
	virtual ~ExecElfFile();
	std::vector<char>& getSegments();
	std::vector<Elf32_Phdr>& getProgramHeader();
protected:
	virtual void firstOperation() = 0;
	virtual void secondOperation() = 0;
	std::vector<Elf32_Phdr> phdr;
	std::vector<char> segments;
};

#endif
