#ifndef _parser_h_
#define _parser_h_

#include <vector>
#include "token.h"
#include "tablebuilder.h"

class Parser
{
public:
	virtual void parse() = 0;
	std::vector<Token*>& getTokenList();
	TableBuilder* getTableBuilder();
	void setTokenList(std::vector<Token*>&);
	void setTableBuilder(TableBuilder*);
	void printTables() const;
	virtual ~Parser();
protected:
	std::vector<Token*> tokens;
	TableBuilder *table_builder;
};
#endif
