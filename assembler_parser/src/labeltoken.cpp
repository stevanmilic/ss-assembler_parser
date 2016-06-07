#include "labeltoken.h"

/**
 * LabelToken implementation
 */

const boost::regex LabelToken::pattern("([a-zA-Z0-9]+):");


LabelToken::LabelToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : Token(line_number, content)
{
	name = content[1].str();
}

token_type LabelToken::getType() const
{
	return label;
}

std::string LabelToken::getName() const
{
	return name;
}
