#ifndef _RELOACTIONDATA_H
#define _RELOACTIONDATA_H

#include "data.h"

class ReloactionData: public Data
{
public:
	int symbol_index;

	ReloactionData(int offset, int symbol_index) : Data(offset)
	{
		this->symbol_index = symbol_index;
	}

	std::ostream& dump(std::ostream& o) const
	{
		o << "| " << symbol_index << " | " << offset << " |\n";
		return o;
	}
};

class LinkerRelocationData : public ReloactionData
{
public:
	std::string symbol_name;
	LinkerRelocationData(int offset, int symbol_index, std::string symbol_name) : ReloactionData(offset, symbol_index)
	{
		this->symbol_name = symbol_name;
	}
};

#endif //_RELOACTIONDATA_H
