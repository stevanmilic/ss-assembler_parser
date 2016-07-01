#ifndef _readobjectelffile_h_
#define _readobjectelffile_h_

#include "objectelffile.h"
#include "elfminformat.h"

class ReadObjectElfFile : public ObjectElfFile
{
public:
	ReadObjectElfFile(std::string);
	std::vector<std::string> getUndSymbols();
	Elf32_Shdr& getSectionHeader(std::string);
	std::vector<char> getSectionData(int);
	std::vector<Elf32_Sym> getSectionSymbols(int) const;
	std::vector<Elf32_Rel> getRelocations(int) const;
	std::string getSymbolName(int) const;
	std::string getSymbolNameByIndex(int) const;
	std::vector<std::string> getOtherSectionNames(std::vector<std::string>&) const;
	//TO DO: more get stuff
protected:
	void firstOperation();//read elf header
	void secondOperation();//read section header
	void thirdOperation();//read string table
	void fourthOperation();//read symbol table
	void fifthOperation();//read sections
};

#endif
