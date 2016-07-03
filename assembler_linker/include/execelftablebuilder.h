#ifndef _execelftablebuilder_h_
#define _execelftablebuilder_h_

#include <vector>
#include "elftablebuilder.h"

class ExecElfTableBuilder : public ElfTableBuilder
{
public:
	ExecElfTableBuilder(std::string);
	bool resolveToken(Token*);
	std::vector<char>& getSegments();
	std::vector<Elf32_Phdr>& getProgramHeader();
protected:
	void resolveSectionToken(Token*);
	void resolveSymbolToken(Token*);
	void resolveRelocationToken(Token*);
};

#endif
