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
	case logical_instruction:
		resolveLogicalInstruction(token);
		break;
	case stack_instruction:
		resolveStackInstruction(token);
		break;
	case call_instruction:
		resolveCallInstruction(token);
		break;
	case io_instruction:
		resolveIOInstruction(token);
		break;
	case move_instruction:
		resolveMoveInstruction(token);
		break;
	case load_instruction:
		resolveLoadInstruction(token);
		break;
	default:
		break;
	}
	return true;
}

int SectionContentTableBuilder::getLocation() const
{
	return location_counter;
}

std::string SectionContentTableBuilder::getName() const
{
	return name;
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

void SectionContentTableBuilder::resolveLoadInstruction(Token* token)
{
	LoadInstructionToken* ltoken = dynamic_cast<LoadInstructionToken*>(token);
	char dst = getRegister(ltoken->getDst());
	if (dst == -1 || dst >= 16) {
		throw MyException("Undefined destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	int c = resolveExpression(ltoken->getC(), ltoken);
	std::string instruction = std::bitset<16>(c).to_string() + "000" + ltoken->getHL() + std::bitset<4>(dst).to_string();
	addInstruction(instruction, ltoken);
	if (ltoken->getName() == "ldc") {
		std::string instruction = std::bitset<16>(c).to_string() + "000" + "1" + std::bitset<4>(dst).to_string();
		addInstruction(instruction, ltoken);
	}
}

void SectionContentTableBuilder::resolveMoveInstruction(Token* token)
{
	MoveInstructionToken* mtoken = dynamic_cast<MoveInstructionToken*>(token);
	char dst = getRegister(mtoken->getDst());
	if (dst == -1) {
		throw MyException("Undefined destination register for Instruction: ", mtoken->getLineNumber(), mtoken->getPosition(), mtoken->getName());
	}
	char src = getRegister(mtoken->getSrc());
	if (src == -1) {
		throw MyException("Undefined source register for Instruction: ", mtoken->getLineNumber(), mtoken->getPosition(), mtoken->getName());
	}
	int imm = resolveExpression(mtoken->getImm(), mtoken);
	if (imm < 0 || !checkRange<unsigned>(imm, 5)) {
		throw MyException("Immedite expression overflows for Instruction: ", mtoken->getLineNumber(), mtoken->getPosition(), mtoken->getName());
	}
	std::string instruction = mtoken->getLR() + std::bitset<5>(imm).to_string() + std::bitset<5>(src).to_string() + std::bitset<5>(dst).to_string();
	addInstruction(instruction, mtoken);
}

void SectionContentTableBuilder::resolveIOInstruction(Token* token)
{
	IOInstructionToken* iotoken = dynamic_cast<IOInstructionToken*>(token);
	char dst = getRegister(iotoken->getDst());
	if (dst == -1 || dst >= 16) {
		throw MyException("Undefined destination register for Instruction: ", iotoken->getLineNumber(), iotoken->getPosition(), iotoken->getName());
	}
	char src = getRegister(iotoken->getSrc());
	if (src == -1 || dst >= 16) {
		throw MyException("Undefined source register for Instruction: ", iotoken->getLineNumber(), iotoken->getPosition(), iotoken->getName());
	}
	std::string instruction = iotoken->getFlag() + std::bitset<4>(src).to_string() + std::bitset<4>(dst).to_string();
	addInstruction(instruction, iotoken);
}

void SectionContentTableBuilder::resolveCallInstruction(Token* token)
{
	CallInstructionToken* ctoken = dynamic_cast<CallInstructionToken*>(token);
	char dst = getRegister(ctoken->getDst());
	if (dst == -1) {
		throw MyException("Undefined destination register for Instruction: ", ctoken->getLineNumber(), ctoken->getPosition(), ctoken->getName());
	}
	int imm = resolveExpression(ctoken->getImm(), ctoken);
	if (!checkRange<int>(imm, 19)) {
		throw MyException("Immedite expression overflows for Instruction: ", ctoken->getLineNumber(), ctoken->getPosition(), ctoken->getName());
	}
	std::string instruction = std::bitset<19>(imm).to_string() + std::bitset<5>(dst).to_string();
	addInstruction(instruction, ctoken);
}

void SectionContentTableBuilder::resolveStackInstruction(Token* token)
{
	StackInstructionToken* stoken = dynamic_cast<StackInstructionToken*>(token);
	char r = getRegister(stoken->getR());
	if (r == -1) {
		throw MyException("Undefined result register for Instruction: ", stoken->getLineNumber(), stoken->getPosition(), stoken->getName());
	}
	char a = getRegister(stoken->getA());
	if (a == -1) {
		throw MyException("Undefined address register for Instruction: ", stoken->getLineNumber(), stoken->getPosition(), stoken->getName());
	}
	if (a == 19) {
		throw MyException("Register not allowed as address register for Instruction: ", stoken->getLineNumber(), stoken->getPosition(), stoken->getName());
	}
	int imm = resolveExpression(stoken->getImm(), stoken);
	if (!checkRange<int>(imm, 10)) {
		throw MyException("Immedite expression overflows for Instruction: ", stoken->getLineNumber(), stoken->getPosition(), stoken->getName());
	}
	std::string instruction = std::bitset<10>(imm).to_string()  + stoken->getLS() + stoken->getF() + std::bitset<5>(r).to_string() + std::bitset<5>(a).to_string();
	addInstruction(instruction, stoken);
}

void SectionContentTableBuilder::resolveLogicalInstruction(Token* token)
{
	LogicalInstructionToken* ltoken = dynamic_cast<LogicalInstructionToken*>(token);
	char dst = getRegister(ltoken->getDst());
	if (dst == -1) {
		throw MyException("Undefined destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	if (dst == 16 || dst == 17 || dst == 19 ) {
		throw MyException("Register not allowed as destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	char src = getRegister(ltoken->getSrc());
	if (src == -1 || dst >= 16) {
		throw MyException("Undefined source register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	if (src == 16 || src == 17 || src == 19 ) {
		throw MyException("Register not allowed as source register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	std::string instruction = std::bitset<5>(src).to_string() + std::bitset<5>(dst).to_string();
	addInstruction(instruction, ltoken);
}

void SectionContentTableBuilder::resolveTermInstruction(Token* token)
{
	TermInstructionToken* ttoken = dynamic_cast<TermInstructionToken*>(token);
	char dst = getRegister(ttoken->getDst());
	if (dst == -1) {
		throw MyException("Undefined destination register for Instruction: ", ttoken->getLineNumber(), ttoken->getPosition(), ttoken->getName());
	}
	if (dst == 19 || (ttoken->getName() != "add" && ttoken->getName() != "sub" && dst > 15)) {
		throw MyException("Register not allowed as destination register for Instruction: ", ttoken->getLineNumber(), ttoken->getPosition(), ttoken->getName());
	}
	std::string bit = getRegister(ttoken->getSrcImm()) >= 0 ? "0" : "1";
	std::string instruction = bit + std::bitset<5>(dst).to_string();
	if (bit == "0") {
		char src = getRegister(ttoken->getSrcImm());
		instruction.insert(0, std::bitset<5>(src).to_string());
	} else {
		int imm = resolveExpression(ttoken->getSrcImm(), ttoken);
		if (!checkRange<int>(imm, 18)) {
			throw MyException("Immedite expression overflows for Instruction: ", ttoken->getLineNumber(), ttoken->getPosition(), ttoken->getName());
		}
		instruction.insert(0, std::bitset<18>(imm).to_string());
	}
	addInstruction(instruction, ttoken);
}

void SectionContentTableBuilder::resolveInterruptInstruction(Token* token)
{
	InterruptInstructionToken* itoken = dynamic_cast<InterruptInstructionToken*>(token);
	char src = itoken->getSrc();
	std::string instruction = std::bitset<4>(src).to_string();
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
			section_data.push_back(new SectionData(location_counter, bytes));
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
	section_data.push_back(new SectionData(location_counter, bytes));
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
		try {
			//TO DO: check if value from expression is valid for 1, 2 and 4 bytes
			//throw error btw 
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
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		section_data.push_back(new SectionData(location_counter, expression_value));
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
		if (!resolveExpressionError(parser.error().c_str(), symbol_table)) {
			for (int i = 0; i < counter; i++) {
				relocations.pop_back();
			}
			throw MyException("Invalid expression for Instruction/Directive: ", token->getLineNumber(), token->getPosition());
		}
		counter++;
	}
	if (std::isnan(expression.value())) {
		//string type in expression -> hex/octal
		expression_string.erase(0, 1);
		expression_string.erase(expression_string.size() - 1);
		return std::stoi(expression_string, 0, 0);
	}
	return expression.value();
}

bool SectionContentTableBuilder::resolveExpressionError(std:: string error, symbol_table_t& symbol_table)
{
	boost::smatch symbol_match;
	if (boost::regex_match(error, symbol_match, undefined_symbol)) {
		std::string symbol = symbol_match[1].str();
		for (std::vector<SymbolData*>::const_iterator it = symbols->begin(); it != symbols->end(); ++it) {
			if ((*it)->section != name && (*it)->label == symbol) {
				symbol_table.add_constant((*it)->label, (*it)->offset);
				relocations.push_back(new ReloactionData(location_counter, name, "R_386_32", it - symbols->begin()));
				return true;
			}
		}
	}
	return false;
}

static inline unsigned short __builtin_bswap16(short int a)
{
  return (a<<8)|(a>>8);
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
	instruction += token->getOC() + token->getFlag() + token->getCondition();
	bitset<INSTRUCTION_SIZE_BITS> bs(instruction);
	instruction = valueToString<unsigned>(bs.to_ulong());
	section_data.push_back(new SectionData(location_counter, instruction));
	location_counter += token->getInstrSize();
}

char SectionContentTableBuilder::getRegister(std::string reg)
{
	if (reg == "r0") {
		return 0;
	}   else if (reg == "r1") {
		return 1;
	} else if (reg == "r2") {
		return 2;
	} else if (reg == "r3") {
		return 3;
	} else if (reg == "r4") {
		return 4;
	} else if (reg == "r5") {
		return 5;
	} else if (reg == "r6") {
		return 6;
	} else if (reg == "r7") {
		return 7;
	} else if (reg == "r8") {
		return 8;
	} else if (reg == "r9") {
		return 9;
	} else if (reg == "r10") {
		return 10;
	} else if (reg == "r11") {
		return 11;
	} else if (reg == "r12") {
		return 12;
	} else if (reg == "r13") {
		return 13;
	} else if (reg == "r14") {
		return 14;
	} else if (reg == "r15") {
		return 15;
	} else if (reg == "pc") {
		return 16;
	} else if (reg == "lr") {
		return 17;
	} else if (reg == "sp") {
		return 18;
	} else if (reg == "psw") {
		return 19;
	} else {
		return -1;//not register, maybe imm :)
	}
}

template <typename T>
bool SectionContentTableBuilder::checkRange(T imm, unsigned no_bits)
{
	if (typeid(T) == typeid(int)) {
		return (imm > 0 ? imm  : -imm) < (( 1 << (no_bits - 1) ) - 1);
	} else if (typeid(T) == typeid(unsigned)) {
		return imm < (1 << no_bits);
	}
	return false;
}
