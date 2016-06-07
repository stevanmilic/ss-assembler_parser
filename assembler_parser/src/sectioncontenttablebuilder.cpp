#include "sectioncontenttablebuilder.h"

/**
 * SectionContentTableBuilder implementation
 */

SectionContentTableBuilder::SectionContentTableBuilder(std::string name, std::vector<SymbolData*>* symbols) : undefined_symbol("ERR183 - Undefined symbol: '([a-zA-Z0-9]+)'")
{
	this->name = name;
	this->symbols = symbols;
	location_counter = 0;
}

int SectionContentTableBuilder::getLocation() const
{
	return location_counter;
}

std::string SectionContentTableBuilder::getName() const
{
	return name;
}

bool SectionContentTableBuilder::resolveToken(Token* token)
{
	switch (token->getType()) {
	case type_directive:
		resolveTypeDirective(token);
		break;
	case skip_directive:
		resolveSkipDirective(token);
		break;
	case align_directive:
		resolveAlignDirective(token);
		break;
	case interrupt_instruction:
		resolveInterruptInstruction(token);
		break;
	case term_instruction:
		resolveTermInstruction(token);
		break;
	case io_instruction:
	case logical_instruction:
	case stack_instruction:
	case call_instruction:
	case move_instruction:
	case load_instruction:
		//TO DO: implement resolve for all tokens above:
		break;
	default:
		break;
	}
	return true;
}

std::ostream& SectionContentTableBuilder::dump(std::ostream& o) const
{
	o << "\nSection: " << name << "\n";
	o << "| Offset | Content |\n";
	for (std::vector<SectionData*>::const_iterator it = section_data.begin(); it != section_data.end(); ++it)
		o << *(*it);
	o << "| Section | Offset | Type | Serial |\n";
	for (std::list<ReloactionData*>::const_iterator it = relocations.begin(); it != relocations.end(); ++it)
		o << *(*it);
	return o;
}

void SectionContentTableBuilder::resolveTermInstruction(Token* token)
{
	TermInstructionToken* ttoken = dynamic_cast<TermInstructionToken*>(token);
	char dst = getRegister(ttoken->getDst());
	if(dst == 19 || (ttoken->getName() != "add" && ttoken->getName() != "sub" && dst > 15)){
		return;//throw error -> register not allowed
	}
	std::string bit = ttoken->getBit();
	std::string instruction = bit + std::bitset<5>(dst).to_string() + ttoken->getOC() + ttoken->getFlag() + ttoken->getCondition();
	if (bit == "0") {
		char src = getRegister(ttoken->getSrc());
		instruction.insert(0, std::bitset<5>(src).to_string());
	} else {
		int imm = resolveExpression(ttoken->getImm(), ttoken);
		instruction.insert(0, std::bitset<18>(imm).to_string());
	}
	addInstruction(instruction, ttoken);
}

void SectionContentTableBuilder::resolveInterruptInstruction(Token* token)
{
	InterruptInstructionToken* itoken = dynamic_cast<InterruptInstructionToken*>(token);
	char src = itoken->getSrc();
	std::string instruction = std::bitset<4>(src).to_string() + itoken->getOC() + itoken->getFlag() + itoken->getCondition();
	addInstruction(instruction, itoken);
}

void SectionContentTableBuilder::resolveAlignDirective(Token *token)
{
	AlignDirectiveToken* atoken = dynamic_cast<AlignDirectiveToken*>(token);
	int mod = location_counter % atoken->getAlignment();
	if (mod != 0) {
		int bytes_to_align = atoken->getAlignment() - mod;
		if (bytes_to_align <= atoken->getMax()) {
			std::string bytes;
			for (int i = 0; i < bytes_to_align; i++)
				bytes += valueToString<char>(atoken->getFill());
			section_data.push_back(new SectionData(location_counter, atoken->getLineNumber(), atoken->getPosition(), bytes));
			location_counter += bytes_to_align;
		}
	}
}

void SectionContentTableBuilder::resolveSkipDirective(Token* token)
{
	SkipDirectiveToken* stoken = dynamic_cast<SkipDirectiveToken*>(token);
	std::string bytes;
	for (int i = 0; i < stoken->getSize(); i++)
		bytes += valueToString<char>(stoken->getFill());
	section_data.push_back(new SectionData(location_counter, stoken->getLineNumber(), stoken->getPosition(), bytes));
	location_counter += stoken->getSize();
}

void SectionContentTableBuilder::resolveTypeDirective(Token* token)
{
	TypeDirectiveToken* ttoken = dynamic_cast<TypeDirectiveToken*>(token);
	std::string type = ttoken->getSection();
	std::vector<std::string> expressions = ttoken->getExpressions();
	for (std::vector<std::string>::const_iterator it = expressions.begin(); it != expressions.end(); ++it) {
		int increment = 0;
		std::string expression_value;
		if (type == "long") {
			increment = 4;
			expression_value = valueToString<int>(resolveExpression(*it, token));
		} else if (type == "word") {
			increment = 2;
			expression_value = valueToString<short int>(resolveExpression(*it, token));
		} else {
			increment = 1;
			expression_value = valueToString<char>(resolveExpression(*it, token));
		}
		section_data.push_back(new SectionData(location_counter, ttoken->getLineNumber(), ttoken->getPosition(), expression_value));
		location_counter += increment;
	}
}

double SectionContentTableBuilder::resolveExpression(std::string expression_string, Token* token)
{
	symbol_table_t symbol_table;
	for (std::vector<SymbolData*>::const_iterator it = symbols->begin(); it != symbols->end(); ++it) {
		if ((*it)->section == name) {
			symbol_table.add_constant((*it)->label, (*it)->offset);
		}
	}

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	int counter = 0;
	while (!parser.compile(expression_string, expression)) {
		if (!resolveExpressionError(parser.error().c_str(), symbol_table, token)) {
			for (int i = 0; i < counter; i++)
				relocations.pop_back();
			return 0;//throw error: invalid expression
		}
		counter++;
	}
	return expression.value();
}

bool SectionContentTableBuilder::resolveExpressionError(std:: string error, symbol_table_t& symbol_table, Token* token)
{
	boost::smatch symbol_match;
	if (boost::regex_match(error, symbol_match, undefined_symbol)) {
		std::string symbol = symbol_match[1].str();
		for (std::vector<SymbolData*>::const_iterator it = symbols->begin(); it != symbols->end(); ++it) {
			if ((*it)->section != name && (*it)->label == symbol) {
				symbol_table.add_constant((*it)->label, (*it)->offset);
				relocations.push_back(new ReloactionData(location_counter, token->getLineNumber(), token->getPosition(), name, "R_386_32", it - symbols->begin()));
				return true;
			}
		}
	}
	return false;
}

template <typename T>
std::string SectionContentTableBuilder::valueToString(T value)
{
	if (typeid(T) == typeid(int) || typeid(T) == typeid(unsigned))
		value =  __builtin_bswap32(value);
	else if (typeid(T) == typeid(short int))
		value =  __builtin_bswap16(value);
	std::stringstream stream;
	stream << std::setfill ('0') << std::setw(2 * sizeof(T))
	       << std::hex << (int)value;
	return stream.str();
}

void SectionContentTableBuilder::addInstruction(std::string& instruction, InstructionToken* token)
{
	bitset<INSTRUCTION_SIZE_BITS> bs(instruction);
	instruction = valueToString<unsigned>(bs.to_ulong());
	section_data.push_back(new SectionData(location_counter, token->getLineNumber(), token->getPosition(), instruction));
	location_counter += token->getInstrSize();
}


char SectionContentTableBuilder::getRegister(std::string reg)
{
	if(reg == "r0"){
		return 0;
	}	else if(reg == "r1"){
		return 1;
	} else if (reg == "r2"){
		return 2;
	} else if (reg == "r3"){
		return 3;
	} else if (reg == "r4"){
		return 4;
	} else if (reg == "r5"){
		return 5;
	} else if (reg == "r6"){
		return 6;
	} else if (reg == "r7"){
		return 7;
	} else if (reg == "r8"){
		return 8;
	} else if (reg == "r9"){
		return 9;
	} else if (reg == "r10"){
		return 10;
	} else if (reg == "r11"){
		return 11;
	} else if (reg == "r12"){
		return 12;
	} else if (reg == "r13"){
		return 13;
	} else if (reg == "r14"){
		return 14;
	} else if (reg == "r15"){
		return 15;
	} else if (reg == "pc"){
		return 16;
	} else if (reg == "lr"){
		return 17;
	} else if (reg == "sp"){
		return 18;
	} else if (reg == "psw"){
		return 19;
	}else {
		return -1;//throw error -> undefined register
	}
}
