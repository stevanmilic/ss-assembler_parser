#ifndef _RELOCATIONCONTENTTABLEBUILDER_H
#define _RELOCATIONCONTENTTABLEBUILDER_H

#include "contenttablebuilder.h"
#include "reloactiondata.h"


class RelocationContentTableBuilder: public ContentTableBuilder
{
public:
	bool resolveToken(Token* t);
	std::ostream& dump(std::ostream& o) const;
private:
	std::vector<ReloactionData*> relocations;
};

#endif //_RELOCATIONCONTENTTABLEBUILDER_H
