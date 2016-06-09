#ifndef _TOKEN_H
#define _TOKEN_H
#include <string>
#include <boost/regex.hpp>
#include "linemanipulation.h"
#include "my_exception.h"

enum token_type {label, directive, instruction, section_directive, io_directive, type_directive, align_directive, skip_directive, end_directive, interrupt_instruction, term_instruction, io_instruction, logical_instruction, stack_instruction, call_instruction, move_instruction, load_instruction};

class Token
{
public:
	Token(int line_number, const boost::match_results<std::string::const_iterator>& content);
	virtual ~Token()
	{
	}
	int getLineNumber() const;
	int getPosition() const;
	std::string getContent() const;
	virtual token_type getType() const = 0;
protected:
	int line_number;
	int position;
	std::string content;
	// const boost::match_results<std::string::const_iterator> content;
};

#endif //_TOKEN_H
