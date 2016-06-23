#ifndef _ASMPARSERSECONDPASS_H
#define _ASMPARSERSECONDPASS_H

#include "asmparser.h"
#include "compositetablebuilder.h"

class AsmParserSecondPass: public AsmParser
{
public:
	AsmParserSecondPass(std::vector<Token*>, std::vector<SymbolData*>&);
	void parse();
	void printTables() const;
	std::vector<SymbolData*> getSymbols();
private:
	CompositeTableBuilder composite_table;
};

#endif //_ASMPARSERSECONDPASS_H
