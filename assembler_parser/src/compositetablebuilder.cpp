#include "compositetablebuilder.h"
#include "sectioncontenttablebuilder.h"
#include "symboldata.h"

/**
 * CompositeTableBuilder implementation
 */

CompositeTableBuilder::CompositeTableBuilder(AsmTableBuilder* symbol_table)
{
	sections.push_back(symbol_table);
}

CompositeTableBuilder::~CompositeTableBuilder()
{
	for (std::vector<AsmTableBuilder*>::iterator it = sections.begin(); it != sections.end(); ++it)
		delete *it;
}

bool CompositeTableBuilder::resolveToken(Token *token)
{
	switch (token->getType()) {
	case section_directive:
		resolveSectionDirective(token);
		break;
	case io_directive:
		resolveIODirective(token);
		break;
	case type_directive:
	case skip_directive:
	case align_directive:
	case interrupt_instruction:
	case term_instruction:
	case io_instruction:
	case logical_instruction:
	case stack_instruction:
	case call_instruction:
	case move_instruction:
	case load_instruction:
		sections.back()->resolveToken(token);
		break;
	case end_directive:
		return false;
		break;
	default:
		break;
	}
	return true;
}

void CompositeTableBuilder::resolveIODirective(Token *token)
{
	IODirectiveAsmToken* iotoken = static_cast<IODirectiveAsmToken*>(token);
	std::vector<std::string> symbols = iotoken->getSymbols();
	bool is_extern = iotoken->getSection() == "extern" ? true : false;
	std::vector<Data*>& symbol_table = sections.front()->getSymbols();
	for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if (is_extern) {
			symbol_table.push_back(new SymbolData(0, *it, "UND", "global"));
		} else {
			if (!labelGoPublic(*it)) {
				throw MyException("Label not found for Directive: ", iotoken->getLineNumber(), iotoken->getPosition(), iotoken->getSection());
			}
		}
	}
}

bool CompositeTableBuilder::labelGoPublic(std::string label)
{
	std::vector<Data*>& symbols = sections.front()->getSymbols();
	for (std::vector<Data*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
		SymbolData* symbol = static_cast<SymbolData*>(*it);
		if (symbol->label == label) {
			symbol->local = "global";
			return true;
		}
	}
	//label not found
	return false;
}

void CompositeTableBuilder::resolveSectionDirective(Token *token)
{
	SectionDirectiveAsmToken* sdtoken = static_cast<SectionDirectiveAsmToken*>(token);
	std::vector<Data*>& symbols = sections.front()->getSymbols();
	std::string label = sdtoken->getSubSection().empty() ? "." + sdtoken->getSection() : "." + sdtoken->getSection() + "." + sdtoken->getSubSection();
	sections.push_back(new SectionContentTableBuilder(label, &symbols));
}

std::ostream& CompositeTableBuilder::dump(std::ostream& o) const
{
	for (std::vector<AsmTableBuilder*>::const_iterator it = sections.begin(); it != sections.end(); ++it)
		o << *(*it);
	return o;
}

int CompositeTableBuilder::getLocation() const
{
	return sections.back()->getLocation();
}

std::vector<Data*>& CompositeTableBuilder::getSymbols()
{
	return sections.front()->getSymbols();
}

std::vector<SectionInfo*> CompositeTableBuilder::getSections()
{
	std::vector<SectionInfo*> sections_data;
	for (std::vector<AsmTableBuilder*>::iterator it = sections.begin(); it != sections.end(); ++it) {
		SectionInfo* section = (*it)->getSectionInfo();
		if (section != 0) {
			sections_data.push_back((*it)->getSectionInfo());
		}
	}
	return sections_data;
}
