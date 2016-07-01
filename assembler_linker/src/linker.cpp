#include <iostream>
#include "linkerparser.h"
#include "readobjectelffile.h"
#include "execelftablebuilder.h"
#include "my_exception.h"
#include "linemanipulation.h"
#include "elfminformat.h"
#include "elftoken.h"

LinkerParser::LinkerParser(char* object_file_names[], int no_object_files)
{
	formation.open(object_file_names[0]);
	for (int i = 1; i < no_object_files; i++) {
		elf_files.push_back(new ReadObjectElfFile(object_file_names[i]));
		elf_files.back()->operation();//read elf object file
		// std::cout << *elf_files.back();
	}
	table_builder = new ExecElfTableBuilder("test.out");
	error = false;
	current_token = 0;
}

LinkerParser::~LinkerParser()
{
	for (std::vector<ElfFile*>::iterator it = elf_files.begin(); it != elf_files.end(); ++it) {
		delete *it;
	}
	elf_files.clear();
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		delete *token;
	}
	tokens.clear();
	delete table_builder;
}

void LinkerParser::parse()
{
	std::string line;
	int line_number = 0;
	while (std::getline(formation, line)) {
		if (!parseLine(line, line_number++)) {
			break;//wrong code
		}
	}
	if (error) {
		throw MyException("There was an error in Linking", 0);
		return;
	}
	getOtherElfTokens();
	std::vector<Token*>::iterator token;
	for (token = tokens.begin() + current_token; token != tokens.end(); ++token) {
		try {
			table_builder->resolveToken(*token);
		} catch (MyException& e) {
			std::cout << e.what() << std::endl;
			error = true;
		}
	}
}

bool LinkerParser::parseLine(std::string& line, int line_number)
{
	try {
		lex(line, line_number);
	} catch (MyException& e) {
		std::cout << e.what() << std::endl;
		error = true;
	}
	std::vector<Token*>::iterator token;
	for (token = tokens.begin() + current_token; token != tokens.end(); ++token) {
		try {
			if (!table_builder->resolveToken(*token))
				return false;
		} catch (MyException& e) {
			std::cout << e.what() << std::endl;
			error = true;
		}
	}
	current_token = token - tokens.begin();
	return true;

}

void LinkerParser::lex(std::string& line, int line_number)
{
	LineManipulation::trim(line);
	LineManipulation::whiteToOne(line);
	unsigned start_number_of_tokens = tokens.size();
	getElfTokens(line);
	if ( line.length() && start_number_of_tokens == tokens.size()) {
		throw (MyException("Not declared in this scope:", line_number, 0, line));
	}
}

void LinkerParser::getElfTokens(std::string& section_name)
{
	for (std::vector<ElfFile*>::iterator elf_file = elf_files.begin(); elf_file != elf_files.end(); ++elf_file) {
		ReadObjectElfFile* object_elf_file = static_cast<ReadObjectElfFile*>(*elf_file);
		Elf32_Shdr shdr = object_elf_file->getSectionHeader(section_name);
		if (shdr.sh_name) {
			used_sections.push_back(section_name);
			tokens.push_back(new LinkerSectionElfToken(section_name, shdr.sh_size, object_elf_file->getSectionData(shdr.sh_name)));
			std::vector<Elf32_Sym> elf_symtab = object_elf_file->getSectionSymbols(shdr.sh_name);
			for (std::vector<Elf32_Sym>::iterator sym = elf_symtab.begin(); sym != elf_symtab.end(); ++sym) {
				std::string type = (*sym).st_type ? "global" : "local";
				tokens.push_back(new LinkerSymbolElfToken(object_elf_file->getSymbolName((*sym).st_name), (*sym).st_value, type, section_name));
			}
			std::vector<Elf32_Rel> elf_reltab = object_elf_file->getRelocations(shdr.sh_name);
			for ( std::vector<Elf32_Rel>::iterator rel = elf_reltab.begin(); rel != elf_reltab.end(); ++rel) {
				tokens.push_back(new LinkerRelElfToken((*rel).r_offset, object_elf_file->getSymbolNameByIndex((*rel).r_info)));
			}
		}
	}
}

void LinkerParser::getOtherElfTokens()
{
	for (std::vector<ElfFile*>::iterator elf_file = elf_files.begin(); elf_file != elf_files.end(); ++elf_file) {
		ReadObjectElfFile* object_elf_file = static_cast<ReadObjectElfFile*>(*elf_file);
		std::vector<std::string> section_names = object_elf_file->getOtherSectionNames(used_sections);
		for (std::vector<std::string>::iterator section_name = section_names.begin(); section_name != section_names.end(); ++section_name) {
			getElfTokens(*section_name);
		}
	}
	//and in the end the love you take ... ooohohoohooo indiaaa
	for (std::vector<ElfFile*>::iterator elf_file = elf_files.begin(); elf_file != elf_files.end(); ++elf_file) {
		ReadObjectElfFile* object_elf_file = static_cast<ReadObjectElfFile*>(*elf_file);
		std::vector<std::string> elf_symtab = object_elf_file->getUndSymbols();
		for (std::vector<std::string>::iterator sym = elf_symtab.begin(); sym != elf_symtab.end(); ++sym) {
			tokens.push_back(new LinkerSymbolElfToken(*sym, 0, "global", "UND"));
		}
	}
}

void LinkerParser::writeTables()
{
	ElfTableBuilder *elf_tb = static_cast<ElfTableBuilder*>(table_builder);
	elf_tb->writeTable();
	std::cout << *table_builder;
}
