#include "writeexecelffile.h"
#include "my_exception.h"
#include "linemanipulation.h"

WriteExecElfFile::WriteExecElfFile(std::string exec_file_name)
{
	file = fopen(exec_file_name.c_str(), "w");
	current_offset = 0;
	current_size = 0;
	curr_sh_name = "";
}

WriteExecElfFile::~WriteExecElfFile()
{
	for (std::vector<SymbolData*>::iterator it = symtab.begin(); it != symtab.end(); ++it)
		delete *it;
	for (std::vector<LinkerRelocationData*>::iterator it = reltab.begin(); it != reltab.end(); ++it)
		delete *it;
}

void WriteExecElfFile::newSection(std::string str_sh_name, int size, std::vector<char> sh_data)
{
	if (curr_sh_name != "" && str_sh_name != curr_sh_name) {
		curr_sh_name = str_sh_name;
		current_offset += current_size;
	} else if (curr_sh_name == "") {
		curr_sh_name = str_sh_name;
	}
	Elf32_Phdr sh_phdr;
	sh_phdr.p_offset = current_offset;
	sh_phdr.p_memsz = size;
	std::string sh_strtype = LineManipulation::split(str_sh_name, '.')[0];
	if (sh_strtype == "text") {
		sh_phdr.p_flags = PF_X | PF_R;
	} else {
		sh_phdr.p_flags = PF_W | PF_R;
	}
	sh_phdr.p_align = 0; //for now
	phdr.push_back(sh_phdr);
	current_size = size;
	if (sh_strtype == "bss") {
		sh_data.resize(size);
		std::fill(sh_data.begin(), sh_data.end(), 0);
	}
	segments.insert(segments.end(), sh_data.begin(), sh_data.end());
	symtab.push_back(new SectionSymbolData(current_offset, str_sh_name,  curr_sh_name, "local", size));
}

void WriteExecElfFile::newSymbol(std::string name, int value, std::string type, std::string section)
{
	if (curr_sh_name != name  && checkSymbol(name, section) && section != "UND") {
		symtab.push_back(new SymbolData(current_offset + value, name,  section, type));
	}
}

bool WriteExecElfFile::checkSymbol(std::string name, std::string section)
{
	bool undefined_found = false;
	for (std::vector<SymbolData*>::const_iterator sym = symtab.begin(); sym != symtab.end(); ++sym) {
		if (section != "UND" && (*sym)->label == name) {
			throw MyException("Multiple symbol definition error for(symbol): ", 0, 0, name);
		} else if (section == "UND" && (*sym)->label == name) {
			for (std::vector<LinkerRelocationData*>::iterator rel = reltab.begin(); rel != reltab.end(); ++rel) {
				if ((*rel)->symbol_index == -1 && (*rel)->symbol_name == name) {
					(*rel)->symbol_index = sym - symtab.begin();
				}
			}
			undefined_found = true;
		}
	}
	if (section == "UND" && !undefined_found) {
		throw MyException("Undefined symbol not found: ", 0, 0, name);
	}
	return true;
}

//TO DO: rename to RelocationData please :)
void WriteExecElfFile::newRelocation(int offset, std::string symbol)
{
	reltab.push_back(new LinkerRelocationData(current_offset + offset, findSymbolIndex(symbol), symbol));
}

int WriteExecElfFile::findSymbolIndex(std::string str_st_name)
{
	for (std::vector<SymbolData*>::const_iterator it = symtab.begin(); it != symtab.end(); ++it) {
		if ((*it)->label == str_st_name) {
			return it - symtab.begin();
		}
	}
	return -1;//maybe it's undefined
}

void WriteExecElfFile::checkForUnresolvedSymbols()
{
	for (std::vector<SymbolData*>::const_iterator sym = symtab.begin(); sym != symtab.end(); ++sym) {
		if ((*sym)->section == "UND") {
			throw MyException("Symbol defintion not found for(symbol): ", 0, 0, (*sym)->label);
		}
	}
	for (std::vector<LinkerRelocationData*>::const_iterator rel = reltab.begin(); rel != reltab.end(); ++rel) {

		if ((*rel)->symbol_index == -1) {
			//try to find symbol for relocation
			(*rel)->symbol_index = findSymbolIndex((*rel)->symbol_name);
			if ((*rel)->symbol_index == -1) {
				throw MyException("Relocation not resolved for(relocation): ", 0, 0, (*rel)->symbol_name);
			}
		}
	}
}

void WriteExecElfFile::applyRelocations()
{
	for (std::vector<LinkerRelocationData*>::const_iterator rel = reltab.begin(); rel != reltab.end(); ++rel) {
		//endian safe cast
		int value = (segments[(*rel)->offset] << 24) | (segments[(*rel)->offset + 1] << 16) | (segments[(*rel)->offset + 2] << 8) | (segments[(*rel)->offset + 3]);
		value += symtab[(*rel)->symbol_index]->offset;
		for (int i = 0; i < 4; i++) {
			segments[(*rel)->offset + 3 - i] = (value >> (i * 8));
		}
	}
}

void WriteExecElfFile::firstOperation()
{
	checkForUnresolvedSymbols();
	applyRelocations();
	fwrite(&segments[0], sizeof(char), segments.size(), file);
}

void WriteExecElfFile::secondOperation()
{
}

void WriteExecElfFile::thirdOperation()
{
}

std::ostream& WriteExecElfFile::dump(std::ostream& o) const
{
	ExecElfFile::dump(o);
	for (std::vector<SymbolData*>::const_iterator it = symtab.begin(); it != symtab.end(); ++it) {
		o << "Index(" << it - symtab.begin() << ") : " << *(*it) << std::endl;
	}
	for (std::vector<LinkerRelocationData*>::const_iterator it = reltab.begin(); it != reltab.end(); ++it) {
		o << *(*it);
	}
	return o;
}
