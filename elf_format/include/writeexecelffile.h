#ifndef _writeexecelffile_h_
#define _writeexecelffile_h_

#include "execelffile.h"
#include "symboldata.h"
#include "reloactiondata.h"

class WriteExecElfFile : public ExecElfFile
{
public:
	WriteExecElfFile(std::string);
	~WriteExecElfFile();
	std::ostream& dump(std::ostream&) const;
	void newSection(std::string, int, std::vector<char>);
	void newRelSection(std::string, std::string);
	void newSymbol(std::string, int, std::string, std::string);
	void newRelocation(int, std::string);
	void newSectionData(std::string);
protected:
	void firstOperation();
	void secondOperation();
	void thirdOperation();
	void applyRelocations();
private:
	int findSymbolIndex(std::string);
	bool checkSymbol(std::string, std::string);
	int current_offset;
	int current_size;
	std::string curr_sh_name;
	std::vector<SymbolData*> symtab;
	std::vector<LinkerRelocationData*> reltab;
};

#endif
