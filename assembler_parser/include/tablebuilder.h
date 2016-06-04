#ifndef _TABLEBUILDER_H
#define _TABLEBUILDER_H

#include <data.h>
#include <vector>
#include "labeltoken.h"
#include "directivetoken.h"
#include "instructiontoken.h"

class TableBuilder
{
public:
	virtual bool resolveToken(Token* token) = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;
	friend std::ostream& operator<<(std::ostream& o, const TableBuilder& tb)
	{
		return tb.dump(o);
	}
	virtual ~TableBuilder()
	{
	}
};


#endif //_TABLEBUILDER_H
