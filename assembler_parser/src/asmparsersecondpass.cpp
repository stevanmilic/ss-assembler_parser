#include "asmparsersecondpass.h"

/**
 * AsmParserSecondPass implementation
 */

AsmParserSecondPass::AsmParserSecondPass(std::vector<Token*> tokens, std::vector<SymbolData*>& symbols) : composite_table(symbols)
{
	this->tokens = tokens;
}

void AsmParserSecondPass::parse()
{
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		try {
			if (!composite_table.resolveToken(*token))
				break;
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void AsmParserSecondPass::printTables() const
{
	std::cout << composite_table << endl;
}

std::vector<SymbolData*> AsmParserSecondPass::getSymbols()
{
	return composite_table.getSymbols();
}
