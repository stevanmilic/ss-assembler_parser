#include <iostream>
#include <fstream>
#include "asmparserfirstpass.h"
#include "asmparsersecondpass.h"

int main(void){
	std::ifstream my_program("/home/stevan/etf/sistSoft/projekat/input0.txt");
	AsmParser* parser = new AsmParserFirstPass();
	parser->parse(my_program);
	parser->printTables();
	std::vector<SymbolData*> symbols = parser->getSymbols();
	delete parser;
	parser = new AsmParserSecondPass(symbols);
	parser->parse(my_program);
	parser->printTables();
	delete parser;
	return 0;
}
