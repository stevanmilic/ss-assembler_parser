#ifndef _COMPOSITETABLEBUILDER_H
#define _COMPOSITETABLEBUILDER_H

#include "asmtablebuilder.h"

class CompositeTableBuilder: public AsmTableBuilder
{
public:
	CompositeTableBuilder(AsmTableBuilder*);
	~CompositeTableBuilder();
	bool resolveToken(Token*);
	int getLocation() const;
	std::vector<Data*>& getSymbols();
	std::ostream& dump(std::ostream& o) const;
	std::vector<SectionInfo*> getSections();
protected:
	void resolveTypeDirective(Token*);
	void resolveSectionDirective(Token*);
	void resolveIODirective(Token*);
private:
	bool labelGoPublic(std::string);
	std::vector<AsmTableBuilder*> sections;//front -> symbol table, back -> other section tables
};

#endif //_COMPOSITETABLEBUILDER_H
