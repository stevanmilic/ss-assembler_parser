#ifndef _SECTIONDATA_H
#define _SECTIONDATA_H

#include "data.h"

class SectionData: public Data
{
public:
	std::string content;

	SectionData(int offset, std::string content) : Data(offset)
	{
		this->content = content;
	}

	std::ostream& dump(std::ostream& o) const
	{
		o << "| " << offset << " | " << content << " |\n";
		return o;
	}
};

#endif //_SECTIONDATA_H
