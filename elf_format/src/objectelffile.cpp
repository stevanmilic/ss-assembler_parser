#include <cstdio>
#include <iostream>
#include "objectelffile.h"

ObjectElfFile::~ObjectElfFile()
{
}

void ObjectElfFile::operation()
{
	firstOperation();
	secondOperation();
	thirdOperation();
	fourthOperation();
	fifthOperation();
}

int ObjectElfFile::getSectionIndex(int sh_name) const
{
	for(std::vector<Elf32_Shdr>::const_iterator it = shdr.begin(); it != shdr.end(); ++it){
		if((*it).sh_name == sh_name)
			return (it - shdr.begin());
	}
	return SHN_UNDEF;//undefined
}

int ObjectElfFile::getStringIndex(std::string name) const
{
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		if (name == (*it))
			return it - strtab.begin();
	}
	return 0;
}

int ObjectElfFile::findSectionIndex(std::string name) const
{
	return getSectionIndex(getStringIndex(name));
}

std::ostream& ObjectElfFile::dump(std::ostream& o) const
{
	o << "String Table:" << std::endl;
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		o << "Index(" << it - strtab.begin() << ") : " << *it << std::endl;
	}
	o << "Symbol Table:" << std::endl << "| Name | Value | Type | Binding | Index |" << std::endl;
	for (std::vector<Elf32_Sym>::const_iterator it = symtab.begin(); it != symtab.end(); ++it) {
		o << "| " << (*it).st_name << " | " << (*it).st_value << " | " << std::to_string((*it).st_type) << " | " << std::to_string((*it).st_binding) << " | " << (*it).st_shndx << " |\n";
	}
	o << "Section Header" << std::endl << "| Name | Type | Offset | Size | Link | Info |" << std::endl;
	for (std::vector<Elf32_Shdr>::const_iterator it = shdr.begin(); it != shdr.end(); ++it) {
		o << "| " << (*it).sh_name << " | " << (*it).sh_type << " | " << (*it).sh_offset << " | " << (*it).sh_size << " | " << (*it).sh_link << " | " << (*it).sh_info << " |\n";
	}
	return o;
}
