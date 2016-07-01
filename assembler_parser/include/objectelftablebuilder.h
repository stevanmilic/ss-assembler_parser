#ifndef _compositeelftablebuilder_h_
#define _compositeelftablebuilder_h_

#include <vector>
#include "elftablebuilder.h"

class ObjectElfTableBuilder : public ElfTableBuilder
{
public:
	ObjectElfTableBuilder(std::string);
	bool resolveToken(Token*);
protected:
	void resolveSectionToken(Token*);
	void resolveRelSectionToken(Token* token);
	void resolveSymbolToken(Token*);
	void resolveRelToken(Token*);
	void resolveSectionDataToken(Token*);
};

#endif
