#ifndef _ASMPARSERFIRSTPASS_H
#define _ASMPARSERFIRSTPASS_H

#include "asmparser.h"

class AsmParserFirstPass: public AsmParser
{
public:
	AsmParserFirstPass(std::string);
	void parse();
	void printTables() const;
	std::vector<SymbolData*> getSymbols();
protected:
	bool parseLine(std::string& line, int line_number);
	void lex(std::string& line, int line_number);
private:
	void getTokens(std::string& curr_line, int line_number, token_type type);
	std::ifstream program;
	unsigned current_token;
	SymbolTableBuilder symbol_table;
};

#endif //_ASMPARSERFIRSTPASS_H
