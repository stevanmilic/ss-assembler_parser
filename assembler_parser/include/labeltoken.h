#ifndef _LABELTOKEN_H
#define _LABELTOKEN_H

#include "token.h"

class LabelToken: public Token
{
public:
	LabelToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const;
	std::string getName() const;
	const static boost::regex pattern;
private:
	std::string name;
};

#endif //_LABELTOKEN_H
