#ifndef _DIRECTIVETOKEN_H
#define _DIRECTIVETOKEN_H

#include "token.h"

class DirectiveToken: public Token
{
public:
	DirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	static DirectiveToken* getDirective(int line_number, const boost::match_results<std::string::const_iterator>& content);
	const static boost::regex pattern;
	std::string getSection() const;
protected:
	std::string section;
};

class SectionDirectiveToken: public DirectiveToken
{
public:
	SectionDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	std::string getSubSection() const
	{
		return sub_section;
	}
	token_type getType() const
	{
		return section_directive;
	}
protected:
	std::string sub_section;
};

class IODirectiveToken: public DirectiveToken
{
public:
	IODirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return io_directive;
	}
	std::vector<std::string> getSymbols() const
	{
		return symbols;
	}
protected:
	std::vector<std::string> symbols;
};

class TypeDirectiveToken: public DirectiveToken
{
public:
	TypeDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return type_directive;
	}
	std::vector<std::string> getExpressions() const
	{
		return expressions;
	}
	int getNumberOfExpressions() const{
		return expressions.size();
	}
protected:
	std::vector<std::string> expressions;
};

class AlignDirectiveToken: public DirectiveToken
{
public:
	AlignDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return align_directive;
	}
	int getAlignment() const
	{
		return alignment;
	}
	char getFill() const
	{
		return fill;
	}
	int getMax() const
	{
		return max;
	}
protected:
	int alignment,  max;
	char fill;
};

class SkipDirectiveToken: public DirectiveToken
{
public:
	SkipDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return skip_directive;
	}
	int getSize() const
	{
		return size;
	}
	char getFill() const
	{
		return fill;
	}
protected:
	int size;
	char fill;
};

class EndDirectiveToken: public DirectiveToken
{
public:
	EndDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return end_directive;
	}
};
#endif //_DIRECTIVETOKEN_H
