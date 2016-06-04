#ifndef _CONTENTTABLEBUILDER_H
#define _CONTENTTABLEBUILDER_H

#include "tablebuilder.h"
#include <exprtk.hpp>

class ContentTableBuilder: public TableBuilder {
public: 
	virtual bool resolveToken(Token* t) = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;
};

#endif //_CONTENTTABLEBUILDER_H
