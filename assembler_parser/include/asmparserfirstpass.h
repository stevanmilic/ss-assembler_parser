#ifndef _ASMPARSERFIRSTPASS_H
#define _ASMPARSERFIRSTPASS_H

#include "asmparser.h"

class AsmParserFirstPass: public AsmParser
{
public:
	bool parseLine(std::string& line, int line_number);
	void printTables() const;
	std::vector<SymbolData*> getSymbols();
private:
	void lex(std::string & line, int line_number);
	SymbolTableBuilder symbol_table;
};

#endif //_ASMPARSERFIRSTPASS_H
