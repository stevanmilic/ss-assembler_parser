#ifndef _elffile_h_
#define _elffile_h_

#include <cstdio>
#include <string>
#include "elfminformat.h"

class ElfFile
{
public:
	virtual void operation() = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;
	friend std::ostream& operator<<(std::ostream& o, const ElfFile& ef)
	{
		return ef.dump(o);
	}
	virtual ~ElfFile();
protected:
	Elf32_Ehdr ehdr;
	FILE *file;
};

#endif
