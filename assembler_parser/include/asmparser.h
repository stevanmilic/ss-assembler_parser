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
	AsmParser();
	virtual void parse() = 0;
	virtual void printTables() const = 0;
	virtual std::vector<SymbolData*> getSymbols() = 0;
	std::vector<Token*> getTokenList();
	virtual ~AsmParser();
protected:
	std::vector<Token*> tokens;
};

#endif //_ASMPARSER_H
