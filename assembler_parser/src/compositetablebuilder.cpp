#include "compositetablebuilder.h"

/**
 * CompositeTableBuilder implementation
 */

CompositeTableBuilder::CompositeTableBuilder(std::vector<SymbolData*>& symbols)
{
	this->symbols = symbols;
}

bool CompositeTableBuilder::resolveToken(Token* token)
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
		current_section->resolveToken(token);
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
	IODirectiveToken* iotoken = dynamic_cast<IODirectiveToken*>(token);
	std::vector<std::string> symbols = iotoken->getSymbols();
	bool is_extern = iotoken->getSection() == "extern" ? true : false;
	for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if (is_extern) {
			this->symbols.push_back(new SymbolData(current_section->getLocation(), iotoken->getLineNumber(), iotoken->getPosition(), *it, current_section->getName(), "global"));
		} else {
			if (!labelGoPublic(*it)) {
				return;//throw error -> label not found
			}
		}
	}
}

bool CompositeTableBuilder::labelGoPublic(std::string label)
{
	for (std::vector<SymbolData*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if ((*it)->label == label) {
			(*it)->local = "global";
			return true;
		}
	}
	//label not found
	return false;
}

void CompositeTableBuilder::resolveSectionDirective(Token *token)
{
	SectionDirectiveToken* sdtoken = dynamic_cast<SectionDirectiveToken*>(token);
	current_section = new SectionContentTableBuilder(sdtoken->getSection(), &symbols);
	sections.push_back(current_section);
}

std::ostream& CompositeTableBuilder::dump(std::ostream& o) const
{
	o << "| Label | Section | Offset | Local? |\n";
	for (std::vector<SymbolData*>::const_iterator it = symbols.begin(); it != symbols.end(); ++it)
		o << *(*it);
	for (std::list<SectionContentTableBuilder*>::const_iterator it = sections.begin(); it != sections.end(); ++it)
		o << *(*it);
	return o;
}

std::vector<SymbolData*>& CompositeTableBuilder::getSymbols()
{
	return symbols;
}
