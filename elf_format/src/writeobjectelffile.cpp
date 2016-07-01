#include "writeobjectelffile.h"
#include "linemanipulation.h"
#include "my_exception.h"

WriteObjectElfFile::WriteObjectElfFile(std::string object_file_name)
{
	file = fopen(object_file_name.c_str(), "w");
	if (!file) {
		throw MyException("Can't Create Elf object file!", 0);
	}
	strtab.push_back("");
	Elf32_Shdr empty_section;
	empty_section.sh_name = 0;
	empty_section.sh_type = SHT_NULL;
	empty_section.sh_offset = SHN_UNDEF;
	empty_section.sh_size = 0;
	empty_section.sh_link = 0;
	empty_section.sh_info = 0;
	shdr.push_back(empty_section);
	current_offset = sizeof(Elf32_Ehdr);
}

void WriteObjectElfFile::firstOperation()
{
	ehdr.e_shoff = current_offset;
	ehdr.e_shnum = shdr.size();
	ehdr.e_shstrndx = shdr.size() - 1;
	ehdr.e_phoff = 0;//no program header table
	ehdr.e_phnum = 0;//no program header table
	fwrite(&ehdr, sizeof(Elf32_Ehdr), 1, file);
}

void WriteObjectElfFile::secondOperation()
{
	fwrite(&symtab[0], sizeof(Elf32_Sym), symtab.size(), file);
}

void WriteObjectElfFile::thirdOperation()
{
	std::vector<std::vector<char>>::iterator section = sections.begin();
	std::vector<std::vector<Elf32_Rel>>::iterator reltab = reltabs.begin();
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		std::string sh_strtype = *it != "" ? LineManipulation::split(*it, '.')[0] : "";
		if (sh_strtype == "rel") {
			if (reltab != reltabs.end()) {
				fwrite(&(*reltab)[0], sizeof(Elf32_Rel), (*reltab).size(), file);
				++reltab;
			}
		} else if (sh_strtype == "text" || sh_strtype == "data") {
			if (section != sections.end()) {
				std::vector<char>& bytes = *section;
				fwrite(&bytes[0], sizeof(char), bytes.size(), file);
				++section;
			}
		}
	}
}

void WriteObjectElfFile::fourthOperation()
{
	for (std::vector<std::string>::iterator name = strtab.begin(); name != strtab.end(); ++name) {
		fwrite((*name).c_str(), (*name).size() + 1, 1, file);
	}
}

void WriteObjectElfFile::fifthOperation()
{
	fwrite(&shdr[0], sizeof(Elf32_Shdr), shdr.size(), file);
}

void WriteObjectElfFile::newSection(std::string name, int no_symbols)
{
	strtab.push_back(name);
	Elf32_Shdr section;
	section.sh_name = strtab.size() - 1;
	std::string sh_strtype = LineManipulation::split(name, '.')[0];
	if (sh_strtype == "text" || sh_strtype == "data") {
		section.sh_type = SHT_PROGBITS;
	} else if (sh_strtype == "symtab") {
		section.sh_type = SHT_SYMTAB;
	} else if (sh_strtype == "strtab") {
		section.sh_type = SHT_STRTAB;
	} else if (sh_strtype == "bss") {
		section.sh_type = SHT_NOBITS;
	}
	section.sh_offset = current_offset;
	if (sh_strtype == "text" || sh_strtype == "data") {
		section.sh_size = current_section.size() / 2;
		sections.push_back(HexToBytes(current_section));
	} else if (sh_strtype == "symtab") {
		section.sh_size = no_symbols * sizeof(Elf32_Sym);
	} else if (sh_strtype == "strtab") {
		section.sh_size = getStrTabSize();
	} else if (sh_strtype == "bss") {
		section.sh_size = current_section.size() / 2;
	}
	section.sh_link = 0;
	section.sh_info = 0;
	shdr.push_back(section);
	if ( section.sh_type != SHT_NOBITS) {
		current_offset += section.sh_size;
	}
	current_section.clear();
}

void WriteObjectElfFile::newRelSection(std::string link, std::string info)
{
	strtab.push_back(".rel");
	Elf32_Shdr rel_section;
	rel_section.sh_name = strtab.size() - 1;
	rel_section.sh_type = SHT_REL;
	rel_section.sh_offset = current_offset;
	rel_section.sh_size = current_relsection.size() * sizeof(Elf32_Rel);
	rel_section.sh_link = findSectionIndex(link);
	rel_section.sh_info = findSectionIndex(info);
	shdr.push_back(rel_section);
	reltabs.push_back(current_relsection);
	current_offset += rel_section.sh_size;
	current_relsection.clear();
}

void WriteObjectElfFile::newSymbol(std::string name, int value, std::string type, std::string section)
{
	strtab.push_back(name);
	Elf32_Sym symbol;
	symbol.st_name = getStringIndex(name);
	symbol.st_value = value;
	if (type == "local") {
		symbol.st_type = STB_LOCAL;
	} else if (type == "global") {
		symbol.st_type = STB_GLOBAL;
	}
	std::string st_strbinding = LineManipulation::split(name, '.')[0];
	if (name[0] == '.' && (st_strbinding == "text" || st_strbinding == "data" || st_strbinding == "bss")) {
		symbol.st_binding = STT_SECTION;
	} else {
		symbol.st_binding = STT_NOTYPE;
	}
	symbol.st_shndx = findSectionIndex(section);
	symtab.push_back(symbol);
}

void WriteObjectElfFile::newRelocation(int offset, int symbol_index)
{
	Elf32_Rel relocation;
	relocation.r_offset = offset;
	relocation.r_info = symbol_index;
	current_relsection.push_back(relocation);
}

void WriteObjectElfFile::newSectionData(std::string hex_data)
{
	current_section += hex_data;
}

int WriteObjectElfFile::getStrTabSize() const
{
	int strtab_size = 0;
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		strtab_size += (*it).size() + 1;
	}
	return strtab_size;
}

std::vector<char> WriteObjectElfFile::HexToBytes(const std::string& hex)
{
	std::vector<char> bytes;
	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		char byte = (char) strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}
	return bytes;
}
