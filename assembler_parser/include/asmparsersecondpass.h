#ifndef _ASMPARSERSECONDPASS_H
#define _ASMPARSERSECONDPASS_H

#include "asmparser.h"

class AsmParserSecondPass: public AsmParser
{
public:
	AsmParserSecondPass(TableBuilder*);
	~AsmParserSecondPass();
	void parse();
	void setTableBuilder(TableBuilder*);
	void printTables() const;
};

#endif //_ASMPARSERSECONDPASS_H
