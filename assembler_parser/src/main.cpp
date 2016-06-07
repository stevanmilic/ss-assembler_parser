#include <iostream>
#include <fstream>
#include "asmparserfirstpass.h"
#include "asmparsersecondpass.h"

int main(void)
{
	std::ifstream my_program("/home/stevan/etf/sistSoft/projekat/input0.asm");
	AsmParser* parser = new AsmParserFirstPass();
	parser->parse(my_program);
	parser->printTables();
	std::vector<SymbolData*> symbols = parser->getSymbols();
	delete parser;
	my_program.clear();
	my_program.seekg(0, ios::beg);
	parser = new AsmParserSecondPass(symbols);
	parser->parse(my_program);
	parser->printTables();
	delete parser;
	return 0;
}
