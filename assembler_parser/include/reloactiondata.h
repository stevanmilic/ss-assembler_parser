#ifndef _RELOACTIONDATA_H
#define _RELOACTIONDATA_H

#include "data.h"

class ReloactionData: public Data
{
public:
	std::string section;
	std::string type;
	int serial_number;

	ReloactionData(int offset, int line_number, int position, std::string section, std::string type, int serial_number) : Data(offset, line_number, position)
	{
		this->section = section;
		this->type = type;
		this->serial_number = serial_number;
	}

	std::ostream& dump(std::ostream& o) const
	{
		o << "| " << section << " | " << offset << " | " << type << " | " << serial_number << " |\n";
		return o;
	}
};

#endif //_RELOACTIONDATA_H
