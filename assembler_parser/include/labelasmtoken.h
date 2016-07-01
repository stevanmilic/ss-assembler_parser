#ifndef _LABELASMTOKEN_H
#define _LABELASMTOKEN_H

#include "asmtoken.h"

class LabelAsmToken: public AsmToken
{
public:
	LabelAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const;
	std::string getName() const;
	const static boost::regex pattern;
private:
	std::string name;
};

#endif //_LABELASMTOKEN_H
