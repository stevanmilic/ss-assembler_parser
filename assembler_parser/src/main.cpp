#include <iostream>
#include "asmparserfirstpass.h"
#include "asmparsersecondpass.h"
#include "elfparser.h"

int main(int argc, char* argv[])
{
	Parser *parser;
	std::vector<Token*> tokens;//tokens from asm program
	TableBuilder* table_builder;
	if (argc < 3) {
		std::cout << "Not enough arguments!" << std::endl;
		return -2;
	}
	/* -------- start first pass -------- */
	parser = new AsmParserFirstPass(argv[1]);
	try {
		parser->parse();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "Parsing terminated..." << std::endl;
		exit(-1);
	}
	tokens = parser->getTokenList();
	table_builder = parser->getTableBuilder();
	// AsmParser* asmparser = static_cast<AsmParser*>(parser);
	// asmparser->printTables();
	delete parser;
	/* -------- start second pass -------- */
	parser = new AsmParserSecondPass(table_builder);
	parser->setTokenList(tokens);
	try {
		parser->parse();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		cout << "Parsing terminated..." << std::endl;
		exit(-1);
	}
	tokens = parser->getTokenList();
	table_builder = parser->getTableBuilder();
	AsmParser* asmparser = static_cast<AsmParser*>(parser);
	asmparser->printTables();
	delete parser;
	// delete table_builder;
	/* -------- build elf(format) file -------- */
	parser = new ElfParser(argv[2], table_builder);
	parser->parse();
	ElfParser* elfparser = static_cast<ElfParser*>(parser);
	elfparser->writeTables();
	delete parser;
	return 0;
}
