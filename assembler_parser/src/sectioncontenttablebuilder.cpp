#include "sectioncontenttablebuilder.h"

/**
 * SectionContentTableBuilder implementation
 */

bool SectionContentTableBuilder::resolveToken(Token* t)
{
	return true;
}

std::ostream& SectionContentTableBuilder::dump(std::ostream& o) const
{
	o << "| Offset | Content |\n";
	for (std::vector<SectionData*>::const_iterator it = sections.begin(); it != sections.end(); ++it)
		o << *(*it);
	return o;
}
