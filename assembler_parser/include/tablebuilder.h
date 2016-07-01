#ifndef _TABLEBUILDER_H
#define _TABLEBUILDER_H

#include "token.h"
#include <iostream>

class TableBuilder
{
public:
	virtual bool resolveToken(Token* token) = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;
	friend std::ostream& operator<<(std::ostream& o, const TableBuilder& tb)
	{
		return tb.dump(o);
	}
	virtual ~TableBuilder() {}
};

#endif //_TABLEBUILDER_H
