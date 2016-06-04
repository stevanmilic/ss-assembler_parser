#ifndef _COMPOSITETABLEBUILDER_H
#define _COMPOSITETABLEBUILDER_H

#include "contenttablebuilder.h"
#include "relocationcontenttablebuilder.h"
#include "sectioncontenttablebuilder.h"

class CompositeTableBuilder: public ContentTableBuilder
{
public:
	CompositeTableBuilder(std::vector<SymbolData*>& symbols);
	bool resolveToken(Token* t);
	std::ostream& dump(std::ostream& o) const;
	std::vector<SymbolData*>& getSymbols();
private:
	struct SectionTable{
		std::string name;
		int location_counter;
		SectionContentTableBuilder* section_content;
		RelocationContentTableBuilder* relocation_content;
		SectionTable(std:: string , int, SectionContentTableBuilder*, RelocationContentTableBuilder*);
	};
	std::list<SectionTable*> section_tables;
	std::vector<SymbolData*> symbols;
};

#endif //_COMPOSITETABLEBUILDER_H
