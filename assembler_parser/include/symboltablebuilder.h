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
private:
	bool checkIfExists(std::string label) const;
	void resolveLabel(Token *token);
	void resolveSectionDirective(Token *token);
	void resolveSkipDirective(Token *token);
	void resolveAlignDirective(Token *token);
	void resolveTypeDirective(Token *token);
	void resolveInstruction(Token *token);
	std::vector<SymbolData*> symbols;
	int location_counter;
	std::string current_section;
};

#endif //_SYMBOLTABLEBUILDER_H
