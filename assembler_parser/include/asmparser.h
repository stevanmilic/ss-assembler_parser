#ifndef _ASMPARSER_H
#define _ASMPARSER_H

#include <iostream>
#include <fstream>
#include <list>
#include "linemanipulation.h"
#include "labeltoken.h"
#include "directivetoken.h"
#include "instructiontoken.h"
#include "symboltablebuilder.h"

class AsmParser
{
public:
	void parse(std::ifstream& program);
	virtual void printTables() const = 0;
	virtual ~AsmParser() {}
	virtual std::vector<SymbolData*> getSymbols() = 0;
protected:
	virtual bool parseLine(std::string& line, int line_number) = 0;
	virtual void lex(std::string& line, int line_number) = 0;
	void getTokens(std::string& curr_line, int line_number, token_type type);
	std::list<Token*> tokens;
};

#endif //_ASMPARSER_H
