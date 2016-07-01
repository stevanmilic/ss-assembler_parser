#ifndef _ASMPARSER_H
#define _ASMPARSER_H

#include <iostream>
#include "parser.h"
#include "linemanipulation.h"

class AsmParser : public Parser
{
public:
	AsmParser();
	virtual void parse() = 0;
	virtual ~AsmParser();
protected:
	bool error;
};

#endif //_ASMPARSER_H
