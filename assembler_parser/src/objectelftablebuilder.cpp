#include "objectelftablebuilder.h"
#include "writeobjectelffile.h"
#include "elftoken.h"

ObjectElfTableBuilder::ObjectElfTableBuilder(std::string object_file_name)
{
	elf_file = new WriteObjectElfFile(object_file_name);
}

bool ObjectElfTableBuilder::resolveToken(Token* token)
{
	switch (token->getType()) {
	case elf_section:
		resolveSectionToken(token);
		break;
	case elf_reltab:
		resolveRelSectionToken(token);
		break;
	case elf_symbol:
		resolveSymbolToken(token);
		break;
	case elf_rel:
		resolveRelToken(token);
		break;
	case elf_section_data:
		resolveSectionDataToken(token);
		break;
	default:
		break;
	}
	return true;
}

void ObjectElfTableBuilder::resolveSectionToken(Token* token)
{
	WriteObjectElfFile* object_elf_file = static_cast<WriteObjectElfFile*>(elf_file);
	SectionElfToken *setoken = static_cast<SectionElfToken*>(token);
	object_elf_file->newSection(setoken->getName(), setoken->getSize());
}

void ObjectElfTableBuilder::resolveRelSectionToken(Token* token)
{
	WriteObjectElfFile* object_elf_file = static_cast<WriteObjectElfFile*>(elf_file);
	RelSectionElfToken* rsetoken = static_cast<RelSectionElfToken*>(token);
	object_elf_file->newRelSection(rsetoken->getLink(), rsetoken->getInfo());
}

void ObjectElfTableBuilder::resolveSymbolToken(Token *token)
{
	WriteObjectElfFile* object_elf_file = static_cast<WriteObjectElfFile*>(elf_file);
	SymbolElfToken *setoken = static_cast<SymbolElfToken*>(token);
	object_elf_file->newSymbol(setoken->getName(), setoken->getValue(), setoken->getSectionType(), setoken->getSection());
}

void ObjectElfTableBuilder::resolveRelToken(Token *token)
{
	WriteObjectElfFile* object_elf_file = static_cast<WriteObjectElfFile*>(elf_file);
	RelElfToken* retoken = static_cast<RelElfToken*>(token);
	object_elf_file->newRelocation(retoken->getOffset(), retoken->getSymbolIndex());
}

void ObjectElfTableBuilder::resolveSectionDataToken(Token *token)
{
	WriteObjectElfFile* object_elf_file = static_cast<WriteObjectElfFile*>(elf_file);
	SectionDataElfToken* sdetoken = static_cast<SectionDataElfToken*>(token);
	object_elf_file->newSectionData(sdetoken->getData());
}
