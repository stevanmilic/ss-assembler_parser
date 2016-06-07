#ifndef _COMPOSITETABLEBUILDER_H
#define _COMPOSITETABLEBUILDER_H

#include "tablebuilder.h"
#include "sectioncontenttablebuilder.h"
#include "symboldata.h"

class CompositeTableBuilder: public TableBuilder
{
public:
	CompositeTableBuilder(std::vector<SymbolData*>& symbols);
	bool resolveToken(Token* token);
	std::ostream& dump(std::ostream& o) const;
	std::vector<SymbolData*>& getSymbols();
protected:
	void resolveTypeDirective(Token *token);
	void resolveSectionDirective(Token *token);
	void resolveIODirective(Token *token);
private:
	bool labelGoPublic(std::string label);
	SectionContentTableBuilder* current_section;
	std::list<SectionContentTableBuilder*> sections;
	std::vector<SymbolData*> symbols;
};

#endif //_COMPOSITETABLEBUILDER_H
