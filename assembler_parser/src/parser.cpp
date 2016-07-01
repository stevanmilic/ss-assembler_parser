#include "parser.h"

Parser::~Parser()
{
}

std::vector<Token*>& Parser::getTokenList()
{
	return tokens;
}

TableBuilder* Parser::getTableBuilder()
{
	return table_builder;
}

void Parser::setTokenList(std::vector<Token*>& tokens)
{
	this->tokens = tokens;
} 

void Parser::setTableBuilder(TableBuilder* table_builder)
{
	this->table_builder = table_builder;
}

void Parser::printTables() const
{
	std::cout << *table_builder;
}
