#include "relocationcontenttablebuilder.h"

/**
 * RelocationContentTableBuilder implementation
 */

bool RelocationContentTableBuilder::resolveToken(Token* t)
{
	return true;
}

std::ostream& RelocationContentTableBuilder::dump(std::ostream& o) const
{
	o << "| Offset | Content |\n";
	for (std::vector<ReloactionData*>::const_iterator it = relocations.begin(); it != relocations.end(); ++it)
		o << *(*it);
	return o;
}
