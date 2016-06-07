#ifndef _SYMBOLTABLEBUILDER_H
#define _SYMBOLTABLEBUILDER_H

#include "tablebuilder.h"
#include "symboldata.h"

class SymbolTableBuilder: public TableBuilder
{
public:
	SymbolTableBuilder();
	bool resolveToken(Token* token);
	std::vector<SymbolData*>& getSymbols();
	std::ostream& dump(std::ostream& o) const;
protected:
	void resolveSectionDirective(Token *token);
	void resolveSkipDirective(Token *token);
	void resolveAlignDirective(Token *token);
	void resolveTypeDirective(Token *token);
	void resolveInstruction(Token *token);
private:
	bool checkIfExists(std::string label) const;
	void resolveLabel(Token *token);
	std::vector<SymbolData*> symbols;
	std::string current_section;
	int location_counter;
};

#endif //_SYMBOLTABLEBUILDER_H
