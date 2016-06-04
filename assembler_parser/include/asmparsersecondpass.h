#ifndef _ASMPARSERSECONDPASS_H
#define _ASMPARSERSECONDPASS_H

#include "asmparser.h"
#include "compositetablebuilder.h"

class AsmParserSecondPass: public AsmParser
{
public:
	AsmParserSecondPass(std::vector<SymbolData*>& symbols);
	bool parseLine(std::string& line, int line_number);
	void printTables() const;
	std::vector<SymbolData*> getSymbols();
private:
	void lex(std::string& line, int line_number);
	CompositeTableBuilder composite_table;
};

#endif //_ASMPARSERSECONDPASS_H
