#ifndef _writeobjectelffile_h_
#define _writeobjectelffile_h_

#include "objectelffile.h"

class WriteObjectElfFile : public ObjectElfFile
{
public:
	WriteObjectElfFile(std::string);
	void newSection(std::string, int);
	void newRelSection(std::string, std::string);
	void newSymbol(std::string, int, std::string, std::string);
	void newRelocation(int, int);
	void newSectionData(std::string);
protected:
	void firstOperation();//write elf header
	void secondOperation();//write symbol table
	void thirdOperation();//write sections
	void fourthOperation();//write string table
	void fifthOperation();//write section header
private:
	int getStrTabSize() const;
	std::vector<char> HexToBytes(const std::string& hex);
	int current_offset;
	std::string current_section;
	std::vector<Elf32_Rel> current_relsection;
};

#endif
