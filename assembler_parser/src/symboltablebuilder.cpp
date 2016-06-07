#include "symboltablebuilder.h"

/**
 * SymbolTableBuilder implementation
 */

SymbolTableBuilder::SymbolTableBuilder()
{
	location_counter = 0;
}

bool SymbolTableBuilder::resolveToken(Token* token)
{
	switch (token->getType()) {
	case label:
		resolveLabel(token);
		break;
	case section_directive:
		resolveSectionDirective(token);
		break;
	case type_directive:
		resolveTypeDirective(token);
		break;
	case skip_directive:
		resolveSkipDirective(token);
		break;
	case align_directive:
		resolveAlignDirective(token);
		break;
	case end_directive:
		return false;
		break;
	case interrupt_instruction:
	case term_instruction:
	case io_instruction:
	case logical_instruction:
	case stack_instruction:
	case call_instruction:
	case move_instruction:
	case load_instruction:
		if (current_section != "text") {
			return false;//throw error -> instruction not allowed in current section
		}
		resolveInstruction(token);
		break;
	default:
		break;
	}
	return true;
}

void SymbolTableBuilder::resolveLabel(Token *token)
{
	LabelToken* ltoken = dynamic_cast<LabelToken*>(token);
	if (checkIfExists(ltoken->getName())) {
		return;//throws error
	}
	symbols.push_back(new SymbolData(location_counter, ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName(), current_section, "local"));
}

void SymbolTableBuilder::resolveSectionDirective(Token *token)
{
	SectionDirectiveToken* sdtoken = dynamic_cast<SectionDirectiveToken*>(token);
	std::string label = sdtoken->getSubSection().empty() ? "." + sdtoken->getSection() : "." + sdtoken->getSection() + "." + sdtoken->getSubSection();
	location_counter = 0;
	if (checkIfExists(label)) {
		return;//throws error
	}
	current_section = sdtoken->getSection();
	symbols.push_back(new SymbolData(location_counter, sdtoken->getLineNumber(), sdtoken->getPosition(), label, current_section, "local"));
}

void SymbolTableBuilder::resolveTypeDirective(Token *token)
{
	TypeDirectiveToken* ttoken = dynamic_cast<TypeDirectiveToken*>(token);
	int factor = 0;
	std::string type = ttoken->getSection();
	if (type == "long")
		factor = 4;
	else if (type == "word")
		factor = 2;
	else
		factor = 1;
	location_counter += factor * ttoken->getNumberOfExpressions();
}

void SymbolTableBuilder::resolveSkipDirective(Token *token)
{
	SkipDirectiveToken* stoken = dynamic_cast<SkipDirectiveToken*>(token);
	location_counter += stoken->getSize();
}

void SymbolTableBuilder::resolveAlignDirective(Token *token)
{
	AlignDirectiveToken* atoken = dynamic_cast<AlignDirectiveToken*>(token);
	int mod = location_counter % atoken->getAlignment();
	if (mod != 0) {
		int bytes_to_align = atoken->getAlignment() - mod;
		if (bytes_to_align <= atoken->getMax()) {
			location_counter += bytes_to_align;
		}
	}
}

void SymbolTableBuilder::resolveInstruction(Token *token)
{
	InstructionToken* itoken = dynamic_cast<InstructionToken*>(token);
	location_counter += itoken->getInstrSize();
}

bool SymbolTableBuilder::checkIfExists(std::string label) const
{
	for (std::vector<SymbolData*>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if ((*it)->label == label)
			return 1;
	}
	return 0;
}

std::ostream& SymbolTableBuilder::dump(std::ostream& o) const
{
	o << "| Label | Section | Offset | Local? |\n";
	for (std::vector<SymbolData*>::const_iterator it = symbols.begin(); it != symbols.end(); ++it)
		o << *(*it);
	return o;
}

std::vector<SymbolData*>& SymbolTableBuilder::getSymbols()
{
	return symbols;
}
