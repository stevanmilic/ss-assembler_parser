#include "token.h"

/**
 * Token implementation
 */

Token::Token(int line_number, const boost::match_results<std::string::const_iterator>& content)
{
	this->line_number = line_number;
	this->position = content.position();
	this->content = content.str();
}

int Token::getLineNumber() const
{
	return line_number;
}


int Token::getPosition() const
{
	return position;
}

std::string Token::getContent() const{
	return content;
}
