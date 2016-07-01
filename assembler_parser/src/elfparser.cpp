#include "elfparser.h"
#include "my_exception.h"
#include "elftoken.h"
#include "compositetablebuilder.h"
#include "objectelftablebuilder.h"
#include "symboldata.h"
#include "reloactiondata.h"
#include "sectiondata.h"

ElfParser::ElfParser(std::string elf_file_name, TableBuilder *table_builder)
{
	elf_file_name += ".o";
	CompositeTableBuilder *asmtablebuilder = static_cast<CompositeTableBuilder*>(table_builder);
	getElfTokens(asmtablebuilder->getSymbols(), asmtablebuilder->getSections());
	delete asmtablebuilder;
	this->table_builder = new ObjectElfTableBuilder(elf_file_name);
}

ElfParser::~ElfParser()
{
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		delete *token;
	}
	delete table_builder;
}

void ElfParser::parse()
{
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		table_builder->resolveToken(*token);
	}
}

void ElfParser::getElfTokens(std::vector<Data*>& symbols, std::vector<SectionInfo*> sections)
{
	tokens.push_back(new SectionElfToken(".symtab", symbols.size()));
	for (std::vector<SectionInfo*>::iterator it = sections.begin(); it != sections.end(); ++it) {

		for (std::vector<Data*>::iterator it1 = (*it)->section_data.begin(); it1 != (*it)->section_data.end(); ++it1) {
			SectionData* sdata = static_cast<SectionData*>(*it1);
			tokens.push_back(new SectionDataElfToken(sdata->content));
		}
		tokens.push_back(new SectionElfToken((*it)->name));
		if (!(*it)->relocations.empty()) {
			for (std::vector<Data*>::iterator it2 = (*it)->relocations.begin(); it2 != (*it)->relocations.end(); ++it2) {
				ReloactionData* rel = static_cast<ReloactionData*>(*it2);
				tokens.push_back(new RelElfToken(rel->offset, rel->symbol_index));
			}
			tokens.push_back(new RelSectionElfToken(".rel", (*it)->relocations.size(), ".symtab", (*it)->name));
		}

		delete *it;
	}
	for (std::vector<Data*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
		SymbolData* symbol = static_cast<SymbolData*>(*it);
		tokens.push_back(new SymbolElfToken(symbol->label, symbol->offset, symbol->local , symbol->section));
		delete *it;
	}
	tokens.push_back(new SectionElfToken(".strtab"));
	symbols.clear();
	sections.clear();
}

void ElfParser::writeTables()
{
	std::cout << *table_builder;
	ElfTableBuilder *elf_tb = static_cast<ElfTableBuilder*>(table_builder);
	elf_tb->writeTable();
}
