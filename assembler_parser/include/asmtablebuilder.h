#ifndef _asmtablebuilder_h_
#define _asmtablebuilder_h_

#include <data.h>
#include <vector>
#include "tablebuilder.h"
#include "labelasmtoken.h"
#include "directiveasmtoken.h"
#include "instructionasmtoken.h"

struct SectionInfo{
	std::vector<Data*> section_data;
	std::vector<Data*> relocations;
	std::string name;
	int size;
	SectionInfo(std::vector<Data*>&, std::vector<Data*>&, std::string, int);
	~SectionInfo();
};

class AsmTableBuilder : public TableBuilder
{
public:
	virtual bool resolveToken(Token*) = 0;
	virtual std::vector<Data*>& getSymbols() = 0;
	virtual SectionInfo* getSectionInfo()
	{
		return 0;
	}
	virtual int getLocation() const = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;
	virtual ~AsmTableBuilder() {}
};

#endif
