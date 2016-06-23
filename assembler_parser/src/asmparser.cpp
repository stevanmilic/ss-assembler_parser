#include "asmparser.h"

/**
 * AsmParser implementation
 */

AsmParser::~AsmParser()
{
}

std::vector<Token*> AsmParser::getTokenList()
{
	return tokens;
}
