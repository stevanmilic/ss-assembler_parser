#ifndef _SYMBOLDATA_H
#define _SYMBOLDATA_H

#include <ostream>
#include "data.h"

class SymbolData: public Data
{
public:
	std::string label;
	std::string section;
	std::string local;
	SymbolData(int offset, std::string label, std::string section, std::string local) : Data(offset)
	{
		this->label = label;
		this->section = section;
		this->local = local;
	}

	std::ostream& dump(std::ostream& o) const
	{
		o << "| " << label << " | " << section << " | " << offset << " | " << local << " |\n";
		return o;
	}
};

class SectionSymbolData : public SymbolData
{
public:
	int size;
	SectionSymbolData(int offset, std::string label, std::string section, std::string local, int size) : SymbolData(offset, label, section, local)
	{
		this->size = size;
	}
};

#endif //_SYMBOLDATA_H
