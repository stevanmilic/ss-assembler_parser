#include "labelasmtoken.h"

/**
 * LabelAsmToken implementation
 */

const boost::regex LabelAsmToken::pattern("([a-zA-Z0-9]+):");


LabelAsmToken::LabelAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : AsmToken(line_number, content)
{
	name = content[1].str();
}

token_type LabelAsmToken::getType() const
{
	return label;
}

std::string LabelAsmToken::getName() const
{
	return name;
}
