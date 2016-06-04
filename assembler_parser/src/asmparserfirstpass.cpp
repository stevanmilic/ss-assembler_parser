#include "asmparserfirstpass.h"

/**
 * AsmParserFirstPass implementation
 */

bool AsmParserFirstPass::parseLine(std::string& line, int line_number)
{
	lex(line, line_number);
	for (std::list<Token*>::iterator it = tokens.begin(); it != tokens.end();) {
		if (!symbol_table.resolveToken(*it))
			return false;
		// cout << (*it)->getContent() << endl;
		it = tokens.erase(it);
	}
	return true;
}

void AsmParserFirstPass::lex(std::string& line, int line_number)
{
	LineManipulation::trim(line);
	LineManipulation::whiteToOne(line);
	//search for labels
	getTokens(line, line_number, label);
	//search for directives
	getTokens(line, line_number, directive);
	//search for instructions
	getTokens(line, line_number, instruction);
}

void AsmParserFirstPass::printTables() const
{
	std::cout << symbol_table << endl;
}

std::vector<SymbolData*> AsmParserFirstPass::getSymbols()
{
	return symbol_table.getSymbols();
}
