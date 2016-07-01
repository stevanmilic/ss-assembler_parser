#ifndef _ASMTOKEN_H
#define _ASMTOKEN_H

#include <string>
#include <boost/regex.hpp>
#include "token.h"
#include "linemanipulation.h"
#include "my_exception.h"

class AsmToken : public Token
{
public:
	AsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	virtual ~AsmToken() {}
	int getLineNumber() const;
	int getPosition() const;
	std::string getContent() const;
	virtual token_type getType() const = 0;
protected:
	int line_number;
	int position;
	std::string content;
};

#endif //_ASMTOKEN_H
