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
		std::string str_line_number = "";
		if(line_number){
			str_line_number = " at Line Number: " + std::to_string(line_number);
		}
		std::string str_position = "";
		if(position){
			str_position = " and Position:" + std::to_string(position);
		}
		std::string msg = (std::runtime_error::what() + content) +  str_line_number + str_position;
		return msg.c_str();
	}

protected:
	int line_number;
	int position;
	std::string content;
};

#endif //_MY_EXCEPTION_H
