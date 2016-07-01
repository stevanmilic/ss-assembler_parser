#ifndef _elfparser_h_
#define _elfparser_h_

#include <cstdio>
#include "parser.h"
#include "asmtablebuilder.h"

class ElfParser : public Parser
{
public:
	ElfParser(std::string, TableBuilder*);
	void parse();
	void writeTables();
	~ElfParser();//TO DO: delete all dynamic allocated pointers
private:
	void getElfTokens(std::vector<Data*>&, std::vector<SectionInfo*>);
};
#endif //_elfparser_h_
