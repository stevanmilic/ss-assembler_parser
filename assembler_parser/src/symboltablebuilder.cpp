#include "symboltablebuilder.h"

/**
 * SymbolTableBuilder implementation
 */

SymbolTableBuilder::SymbolTableBuilder()
{
	location_counter = 0;
}

SymbolTableBuilder::~SymbolTableBuilder()
{
	for (std::vector<Data*>::iterator it = symbols.begin(); it != symbols.end(); ++it)
		delete *it;
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
		resolveInstruction(token);
		break;
	default:
		break;
	}
	return true;
}

void SymbolTableBuilder::resolveLabel(Token *token)
{
	LabelAsmToken* ltoken = static_cast<LabelAsmToken*>(token);
	if (checkIfExists(ltoken->getName())) {
		throw MyException("Label already exists: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	symbols.push_back(new SymbolData(location_counter, ltoken->getName(), current_section, "local"));
}

void SymbolTableBuilder::resolveSectionDirective(Token *token)
{
	SectionDirectiveAsmToken* sdtoken = static_cast<SectionDirectiveAsmToken*>(token);
	std::string label = sdtoken->getSubSection().empty() ? "." + sdtoken->getSection() : "." + sdtoken->getSection() + "." + sdtoken->getSubSection();
	location_counter = 0;
	if (checkIfExists(label)) {
		throw MyException("Section already exists: ", sdtoken->getLineNumber(), sdtoken->getPosition(), label);
	}
	current_section = label;
	symbols.push_back(new SymbolData(location_counter, label, current_section, "local"));
}

void SymbolTableBuilder::resolveTypeDirective(Token *token)
{
	TypeDirectiveAsmToken* ttoken = static_cast<TypeDirectiveAsmToken*>(token);
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
	SkipDirectiveAsmToken* stoken = static_cast<SkipDirectiveAsmToken*>(token);
	location_counter += stoken->getSize();
}

void SymbolTableBuilder::resolveAlignDirective(Token *token)
{
	AlignDirectiveAsmToken* atoken = static_cast<AlignDirectiveAsmToken*>(token);
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
	//TO DO: Throw error if no current_section and you are resolving other ...
	InstructionAsmToken* itoken = static_cast<InstructionAsmToken*>(token);
	std::string section = LineManipulation::split(current_section, '.')[0];
	if (section != "text") {
		throw MyException("Instruction not allowed in current Section: ", itoken->getLineNumber(), itoken->getPosition(), current_section);
	}
	location_counter += itoken->getInstrSize();
}

bool SymbolTableBuilder::checkIfExists(std::string label) const
{
	for (std::vector<Data*>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
		SymbolData* symbol = static_cast<SymbolData*>(*it);
		if (symbol->label == label)
			return 1;
	}
	return 0;
}

std::ostream& SymbolTableBuilder::dump(std::ostream& o) const
{
	o << "| Label | Section | Offset | Local? |\n";
	for (std::vector<Data*>::const_iterator it = symbols.begin(); it != symbols.end(); ++it)
		o << *(*it);
	return o;
}

int SymbolTableBuilder::getLocation() const
{
	return location_counter;
}

std::vector<Data*>& SymbolTableBuilder::getSymbols()
{
	return symbols;
}
