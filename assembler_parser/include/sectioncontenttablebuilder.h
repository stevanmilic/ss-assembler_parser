#ifndef _SECTIONCONTENTTABLEBUILDER_H
#define _SECTIONCONTENTTABLEBUILDER_H

#include "contenttablebuilder.h"
#include "sectiondata.h"

class SectionContentTableBuilder: public ContentTableBuilder
{
public:
	bool resolveToken(Token* t);
	std::ostream& dump(std::ostream& o) const;
private:
	std::vector<SectionData*> sections;
};

#endif //_SECTIONCONTENTTABLEBUILDER_H
