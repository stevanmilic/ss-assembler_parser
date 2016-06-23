#ifndef _MY_EXCEPTION_H
#define _MY_EXCEPTION_H

#include <exception>
#include <stdexcept>

class MyException: public std::runtime_error
{
public:
	MyException(std::string msg, int line_number, int position = 0, std::string content = "") : std::runtime_error(msg)
	{
		this->line_number = line_number;
		this->position = position;
		this->content = content;
	}

	virtual ~MyException() throw() {}

	virtual const char* what() const throw()
	{
		std::string msg = (std::runtime_error::what() + content) +  std::string(" at Line Number: ") + std::to_string(line_number) + std::string(" and Position: ") + std::to_string(position);
		return msg.c_str();
	}

protected:
	int line_number;
	int position;
	std::string content;
};

#endif //_MY_EXCEPTION_H
