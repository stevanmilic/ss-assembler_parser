#include "readobjectelffile.h"
#include "linemanipulation.h"
#include "my_exception.h"
#include <algorithm>

ReadObjectElfFile::ReadObjectElfFile(std::string object_file_name)
{
	file = fopen(object_file_name.c_str(), "r");
	if (!file) {
		throw MyException("Can't Open Elf object file!", 0);
	}
}

void ReadObjectElfFile::firstOperation()
{
	fread(&ehdr, sizeof(Elf32_Ehdr), 1, file);
}

void ReadObjectElfFile::secondOperation()
{
	shdr.resize(ehdr.e_shnum);
	fseek(file, ehdr.e_shoff, SEEK_SET);
	fread(&shdr[0], sizeof(Elf32_Shdr), ehdr.e_shnum, file);
}

void ReadObjectElfFile::thirdOperation()
{
	fseek(file, shdr[ehdr.e_shstrndx].sh_offset, SEEK_SET);
	int curr_byte = 0;
	while (curr_byte < shdr[ehdr.e_shstrndx].sh_size) {
		char* c_strtab;
		size_t len = 0;
		curr_byte += getdelim(&c_strtab, &len, '\0', file);
		strtab.push_back(c_strtab);
	}
}

void ReadObjectElfFile::fourthOperation()
{
	int symtab_index = getSectionIndex(getStringIndex(".symtab"));
	fseek(file, shdr[symtab_index].sh_offset, SEEK_SET);
	symtab.resize(shdr[symtab_index].sh_size / sizeof(Elf32_Sym));
	fread(&symtab[0], shdr[symtab_index].sh_size, 1, file);
}

void ReadObjectElfFile::fifthOperation()
{
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		std::string sh_strtype = *it != "" ? LineManipulation::split(*it, '.')[0] : "";
		if (sh_strtype == "rel") {
			int relsec_index = getSectionIndex(it - strtab.begin());
			std::vector<Elf32_Rel> relsec;
			relsec.resize(shdr[relsec_index].sh_size / sizeof(Elf32_Rel));
			fread(&relsec[0], shdr[relsec_index].sh_size, 1, file);
			reltabs.push_back(relsec);
		} else if (sh_strtype == "text" || sh_strtype == "data") {
			int sec_index = getSectionIndex(it - strtab.begin());
			std::vector<char> sec;
			sec.resize(shdr[sec_index].sh_size);
			fread(&sec[0], shdr[sec_index].sh_size, 1, file);
			sections.push_back(sec);
		}
	}
}

std::vector<std::string> ReadObjectElfFile::getUndSymbols()
{
	std::vector<std::string> und_symtab;
	for (std::vector<Elf32_Sym>::const_iterator it = symtab.begin(); it != symtab.end(); ++it) {
		if ((*it).st_shndx == SHN_UNDEF) {
			und_symtab.push_back(strtab[(*it).st_name]);
		}
	}
	return und_symtab;
}

Elf32_Shdr& ReadObjectElfFile::getSectionHeader(std::string str_sh_name)
{
	return shdr[findSectionIndex(str_sh_name)];
}

std::vector<char> ReadObjectElfFile::getSectionData(int sh_name)
{
	int sh_data_ndx = 0;
	for (std::vector<std::string>::const_iterator it = strtab.begin(); it != strtab.end(); ++it) {
		std::string sh_strtype = *it != "" ? LineManipulation::split(*it, '.')[0] : "";
		if (sh_strtype == "text" || sh_strtype == "data") {
			if(*it == strtab[sh_name]){
				return sections[sh_data_ndx];
			}
			sh_data_ndx++;
		}
	}
	return std::vector<char>();
}

std::vector<Elf32_Sym> ReadObjectElfFile::getSectionSymbols(int sh_name) const
{
	int shndx = getSectionIndex(sh_name);
	std::vector<Elf32_Sym> sh_symtab;
	for (std::vector<Elf32_Sym>::const_iterator it = symtab.begin(); it != symtab.end(); ++it) {
		if ((*it).st_shndx == shndx) {
			sh_symtab.push_back(*it);
		}
	}
	return sh_symtab;
}

std::vector<Elf32_Rel> ReadObjectElfFile::getRelocations(int sh_name) const
{
	int shndx = getSectionIndex(sh_name);
	int sh_rel_ndx = 0;
	for (std::vector<Elf32_Shdr>::const_iterator it = shdr.begin(); it != shdr.end(); ++it) {
		if(strtab[(*it).sh_name] == ".rel"){
			if((*it).sh_info == shndx){
				return reltabs[sh_rel_ndx];
			}
			sh_rel_ndx++;
		}
	}
	return std::vector<Elf32_Rel>();
}

std::string ReadObjectElfFile::getSymbolName(int st_name) const
{
	return strtab[st_name];
}

std::string ReadObjectElfFile::getSymbolNameByIndex(int stndx) const
{
	return strtab[symtab[stndx].st_name];
}

std::vector<std::string> ReadObjectElfFile::getOtherSectionNames(std::vector<std::string>& str_sh_names) const
{
	std::vector<std::string> other_str_sh_names = strtab;
	for (std::vector<std::string>::iterator str_sh_name = str_sh_names.begin(); str_sh_name != str_sh_names.end(); ++str_sh_name) {
		other_str_sh_names.erase(std::remove(other_str_sh_names.begin(), other_str_sh_names.end(), *str_sh_name), other_str_sh_names.end());
	}
	for (std::vector<std::string>::iterator it = other_str_sh_names.begin(); it != other_str_sh_names.end();) {
		std::string sh_strtype = *it != "" ? LineManipulation::split(*it, '.')[0] : "";
		if (sh_strtype != "text" && sh_strtype != "data") {
			it = other_str_sh_names.erase(it);
		}
		else{
			++it;
		}
	}
	return other_str_sh_names;
}
