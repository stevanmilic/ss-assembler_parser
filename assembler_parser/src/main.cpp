#include <iostream>
#include <fstream>
#include "asmparserfirstpass.h"
#include "asmparsersecondpass.h"

int main(void)
{
	std::ifstream my_program("/home/stevan/etf/sistSoft/projekat/input0.asm");
	/* -------- start first pass -------- */
	AsmParser* parser = new AsmParserFirstPass();
	parser->parse(my_program);
	std::vector<SymbolData*> symbols = parser->getSymbols();
	std::vector<Token*> tokens = parser->getTokenList();
	delete parser;
	/* -------- start second pass -------- */
	parser = new AsmParserSecondPass(tokens, symbols);
	parser->parse(my_program);
	parser->printTables();
	delete parser;
	return 0;
}
