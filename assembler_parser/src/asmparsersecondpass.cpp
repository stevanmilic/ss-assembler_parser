#include "asmparsersecondpass.h"

/**
 * AsmParserSecondPass implementation
 */

AsmParserSecondPass::AsmParserSecondPass(std::vector<SymbolData*>& symbols) : composite_table(symbols)
{	
}

bool AsmParserSecondPass::parseLine(std::string& line, int line_number)
{
	lex(line, line_number);
	for (std::list<Token*>::iterator it = tokens.begin(); it != tokens.end();) {
		if(!composite_table.resolveToken(*it))
			return false;
		// cout << (*it)->getContent() << endl;
		it = tokens.erase(it);
	}
	return true;
}

void AsmParserSecondPass::lex(std::string& line, int line_number)
{
	LineManipulation::trim(line);
	LineManipulation::whiteToOne(line);
	//search for directives
	getTokens(line, line_number, directive);
	//search for instructions
	getTokens(line, line_number, instruction);
}

void AsmParserSecondPass::printTables() const
{
	std::cout << composite_table << endl;
}

std::vector<SymbolData*> AsmParserSecondPass::getSymbols()
{
	return composite_table.getSymbols();	
}
