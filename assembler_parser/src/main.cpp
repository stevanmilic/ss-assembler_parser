#include <iostream>
#include "asmparserfirstpass.h"
#include "asmparsersecondpass.h"

int main(void)
{
	/* -------- start first pass -------- */
	AsmParser* parser = new AsmParserFirstPass("/home/stevan/etf/sistSoft/projekat/input0.asm");
	parser->parse();
	std::vector<SymbolData*> symbols = parser->getSymbols();
	std::vector<Token*> tokens = parser->getTokenList();
	delete parser;
	/* -------- start second pass -------- */
	parser = new AsmParserSecondPass(tokens, symbols);
	parser->parse();
	parser->printTables();
	delete parser;
	return 0;
}
