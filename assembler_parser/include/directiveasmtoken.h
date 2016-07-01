#ifndef _DIRECTIVEASMTOKEN_H
#define _DIRECTIVEASMTOKEN_H

#include "asmtoken.h"

class DirectiveAsmToken: public AsmToken
{
public:
	DirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	static DirectiveAsmToken* getDirective(int line_number, const boost::match_results<std::string::const_iterator>& content);
	const static boost::regex pattern;
	std::string getSection() const;
protected:
	std::string section;
};

class SectionDirectiveAsmToken: public DirectiveAsmToken
{
public:
	SectionDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
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

class IODirectiveAsmToken: public DirectiveAsmToken
{
public:
	IODirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
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

class TypeDirectiveAsmToken: public DirectiveAsmToken
{
public:
	TypeDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
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

class AlignDirectiveAsmToken: public DirectiveAsmToken
{
public:
	AlignDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
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

class SkipDirectiveAsmToken: public DirectiveAsmToken
{
public:
	SkipDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
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

class EndDirectiveAsmToken: public DirectiveAsmToken
{
public:
	EndDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return end_directive;
	}
};
#endif //_DIRECTIVEASMTOKEN_H
