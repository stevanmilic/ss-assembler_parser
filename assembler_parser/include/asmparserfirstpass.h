#ifndef _ASMPARSERFIRSTPASS_H
#define _ASMPARSERFIRSTPASS_H

#include <fstream>
#include "asmparser.h"

class AsmParserFirstPass: public AsmParser
{
public:
	AsmParserFirstPass(std::string);
	void parse();
	void printTables() const;
protected:
	bool parseLine(std::string& line, int line_number);
	void lex(std::string& line, int line_number);
private:
	void getAsmTokens(std::string& curr_line, int line_number, token_type type);
	std::ifstream program;
	unsigned current_token;
};

#endif //_ASMPARSERFIRSTPASS_H
