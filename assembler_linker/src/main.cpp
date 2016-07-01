#include <iostream>
#include "linkerparser.h"

int main(int argc, char* argv[])
{
	if(argc < 3){
		std::cout << "Not enough arguments!" << std::endl;
		return -2;
	}
	/* -------- start linking -------- */
	Parser* parser = new LinkerParser(++argv, --argc);
	try {
		parser->parse();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "Linking terminated..." << std::endl;
		exit(-1);
	}
	LinkerParser* linkerparser = static_cast<LinkerParser*>(parser);
	linkerparser->writeTables();
	delete parser;
	return 0;
}
