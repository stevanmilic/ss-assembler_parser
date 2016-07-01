#ifndef _SYMBOLTABLEBUILDER_H
#define _SYMBOLTABLEBUILDER_H

#include "asmtablebuilder.h"
#include "symboldata.h"

class SymbolTableBuilder: public AsmTableBuilder
{
public:
	SymbolTableBuilder();
	~SymbolTableBuilder();
	bool resolveToken(Token*);
	int getLocation() const;
	std::vector<Data*>& getSymbols();
	std::ostream& dump(std::ostream& o) const;
protected:
	void resolveSectionDirective(Token*);
	void resolveSkipDirective(Token*);
	void resolveAlignDirective(Token*);
	void resolveTypeDirective(Token*);
	void resolveInstruction(Token*);
	void resolveLabel(Token*);
	std::vector<Data*> symbols;
private:
	bool checkIfExists(std::string) const;
	std::string current_section;
	int location_counter;
};

#endif //_SYMBOLTABLEBUILDER_H
