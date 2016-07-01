#include "asmtoken.h"

/**
 * AsmToken implementation
 */

AsmToken::AsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content)
{
	this->line_number = line_number;
	this->position = content.position();
	this->content = content.str();
}

int AsmToken::getLineNumber() const
{
	return line_number;
}


int AsmToken::getPosition() const
{
	return position;
}

std::string AsmToken::getContent() const{
	return content;
}
