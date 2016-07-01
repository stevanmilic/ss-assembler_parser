#ifndef _linkerparser_h_
#define _linkerparser_h_

#include <vector>
#include <fstream>
#include "parser.h"
#include "elffile.h"

class LinkerParser : public Parser
{
public:
	LinkerParser(char**, int);
	void parse();
	void writeTables();
	~LinkerParser();
protected:
	bool parseLine(std::string&, int);
	void lex(std::string&, int);
private:
	void getElfTokens(std::string&);
	void getOtherElfTokens();
	std::ifstream formation;
	std::vector<ElfFile*> elf_files;
	std::vector<std::string> used_sections;
	bool error;
	unsigned current_token;
};

#endif
