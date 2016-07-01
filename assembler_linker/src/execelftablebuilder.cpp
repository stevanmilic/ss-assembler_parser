#include "execelftablebuilder.h"
#include "writeexecelffile.h"
#include "elftoken.h"

ExecElfTableBuilder::ExecElfTableBuilder(std::string exec_file_name)
{
	elf_file = new WriteExecElfFile(exec_file_name);
}

bool ExecElfTableBuilder::resolveToken(Token* token)
{
	switch (token->getType()) {
	case linker_elf_section:
		resolveSectionToken(token);
		break;
	case linker_elf_symbol:
		resolveSymbolToken(token);
		break;
	case linker_elf_rel:
		resolveRelocationToken(token);
		break;
	default:
		break;
	}
	return true;
}

void ExecElfTableBuilder::resolveSectionToken(Token* token)
{
	WriteExecElfFile* exec_elf_file = static_cast<WriteExecElfFile*>(elf_file);
	LinkerSectionElfToken *lsetoken = static_cast<LinkerSectionElfToken*>(token);
	exec_elf_file->newSection(lsetoken->getName(), lsetoken->getSize(), lsetoken->getData());
}

void ExecElfTableBuilder::resolveRelocationToken(Token* token)
{
	WriteExecElfFile* exec_elf_file = static_cast<WriteExecElfFile*>(elf_file);
	LinkerRelElfToken* lrtoken = static_cast<LinkerRelElfToken*>(token);
	exec_elf_file->newRelocation(lrtoken->getOffset(), lrtoken->getSymbolName());
}

void ExecElfTableBuilder::resolveSymbolToken(Token *token)
{
	WriteExecElfFile* exec_elf_file = static_cast<WriteExecElfFile*>(elf_file);
	LinkerSymbolElfToken *lsetoken = static_cast<LinkerSymbolElfToken*>(token);
	exec_elf_file->newSymbol(lsetoken->getName(), lsetoken->getValue(), lsetoken->getSectionType(), lsetoken->getSection());
}
