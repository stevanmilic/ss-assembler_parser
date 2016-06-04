#ifndef _RELOACTIONDATA_H
#define _RELOACTIONDATA_H

#include "data.h"

class ReloactionData: public Data
{
public:
	std::string section;
	std::string type;

	ReloactionData(int offset, int line_number, int position, std::string section, std::string type) : Data(offset, line_number, position)
	{
		this->section = section;
		this->type = type;
	}

	std::ostream& dump(std::ostream& o) const
	{
		o << "| " << section << " | " << offset << " | " << type << " |\n";
	}
};

#endif //_RELOACTIONDATA_H
