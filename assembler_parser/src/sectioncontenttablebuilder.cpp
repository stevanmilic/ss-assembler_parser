#include "sectioncontenttablebuilder.h"
#include <iomanip>
#include <bitset>
#include <iostream>
#include "symboldata.h"
#include "sectiondata.h"
#include "reloactiondata.h"

/**
 * SectionContentTableBuilder implementation
 */


SectionContentTableBuilder::SectionContentTableBuilder(std::string name, std::vector<Data*>* symbols)
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

SectionInfo* SectionContentTableBuilder::getSectionInfo()
{
	return new SectionInfo(section_data, relocations, name, location_counter);
}

std::ostream& SectionContentTableBuilder::dump(std::ostream& o) const
{
	o << "\nSection: " << name << "\n";
	o << "| Offset | Content |\n";
	for (std::vector<Data*>::const_iterator it = section_data.begin(); it != section_data.end(); ++it)
		o << *(*it);
	o << "| SectionIndex | Offset |\n";
	for (std::vector<Data*>::const_iterator it = relocations.begin(); it != relocations.end(); ++it)
		o << *(*it);
	return o;
}

void SectionContentTableBuilder::resolveLoadInstruction(Token* token)
{
	LoadInstructionAsmToken* ltoken = static_cast<LoadInstructionAsmToken*>(token);
	char dst = getRegister(ltoken->getDst());
	if (dst == -1 || dst >= 16) {
		throw MyException("Undefined destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	int c = resolveExpression(ltoken->getC(), ltoken);
	if (ltoken->getName() != "ldc") {
		std::string instruction = std::bitset<16>(c).to_string() + "000" + ltoken->getHL() + std::bitset<4>(dst).to_string();
		addInstruction(instruction, ltoken);
	} else {
		std::string c_string = std::bitset<32>(c).to_string();
		std::string instruction = c_string.substr(16, 16) + "000" + "0" + std::bitset<4>(dst).to_string();
		addInstruction(instruction, ltoken);
		location_counter -= 4;//becasue instr size for ldc returns 8 and it's divided in two halfs
		instruction = c_string.substr(0, 16) + "000" + "1" + std::bitset<4>(dst).to_string();
		addInstruction(instruction, ltoken);
		location_counter -= 4;//becasue instr size for ldc returns 8 and it's divided in two halfs
	}
}

void SectionContentTableBuilder::resolveMoveInstruction(Token* token)
{
	MoveInstructionAsmToken* mtoken = static_cast<MoveInstructionAsmToken*>(token);
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
	IOInstructionAsmToken* iotoken = static_cast<IOInstructionAsmToken*>(token);
	char dst = getRegister(iotoken->getDst());
	if (dst == -1 || dst >= 16) {
		throw MyException("Undefined destination register for Instruction: ", iotoken->getLineNumber(), iotoken->getPosition(), iotoken->getName());
	}
	char src = getRegister(iotoken->getSrc());
	if (src == -1) {
		throw MyException("Undefined source register for Instruction: ", iotoken->getLineNumber(), iotoken->getPosition(), iotoken->getName());
	}
	std::string instruction = iotoken->getIO() + std::bitset<4>(src).to_string() + std::bitset<4>(dst).to_string();
	addInstruction(instruction, iotoken);
}

void SectionContentTableBuilder::resolveCallInstruction(Token* token)
{
	CallInstructionAsmToken* ctoken = static_cast<CallInstructionAsmToken*>(token);
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
	StackInstructionAsmToken* stoken = static_cast<StackInstructionAsmToken*>(token);
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
	LogicalInstructionAsmToken* ltoken = static_cast<LogicalInstructionAsmToken*>(token);
	char dst = getRegister(ltoken->getDst());
	if (dst == -1) {
		throw MyException("Undefined destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	if (dst == 16 || dst == 17 || dst == 19 ) {
		throw MyException("Register not allowed as destination register for Instruction: ", ltoken->getLineNumber(), ltoken->getPosition(), ltoken->getName());
	}
	char src = getRegister(ltoken->getSrc());
	if (src == -1) {
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
	TermInstructionAsmToken* ttoken = static_cast<TermInstructionAsmToken*>(token);
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
	InterruptInstructionAsmToken* itoken = static_cast<InterruptInstructionAsmToken*>(token);
	char src = itoken->getSrc();
	std::string instruction = std::bitset<4>(src).to_string();
	addInstruction(instruction, itoken);
}

void SectionContentTableBuilder::resolveAlignDirective(Token* token)
{
	AlignDirectiveAsmToken* atoken = static_cast<AlignDirectiveAsmToken*>(token);
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
	SkipDirectiveAsmToken* stoken = static_cast<SkipDirectiveAsmToken*>(token);
	std::string bytes;
	for (int i = 0; i < stoken->getSize(); i++)
		bytes += valueToString<char>(stoken->getFill());
	section_data.push_back(new SectionData(location_counter, bytes));
	location_counter += stoken->getSize();
}

void SectionContentTableBuilder::resolveTypeDirective(Token* token)
{
	TypeDirectiveAsmToken* ttoken = static_cast<TypeDirectiveAsmToken*>(token);
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
	for (std::vector<Data*>::const_iterator it = symbols->begin(); it != symbols->end(); ++it) {
		SymbolData* symbol = static_cast<SymbolData*>(*it);
		if (symbol->label[0] != '.' && symbol->section != name) {
			symbol_table.add_constant(symbol->label, 0);
		} else if (symbol->label[0] != '.' && symbol->section == name) {
			symbol_table.add_constant(symbol->label, symbol->offset - (location_counter + 4));
		}
	}

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	parser.dec().collect_variables() = true;
	if (!parser.compile(expression_string, expression)) {
		AsmToken* asmtoken = static_cast<AsmToken*>(token);
		throw MyException("Expression error: ", asmtoken->getLineNumber(), asmtoken->getPosition(), parser.error());
	}

	std::deque<symbol_t> symbol_list;
	parser.dec().symbols(symbol_list);
	for (std::size_t i = 0; i < symbol_list.size(); ++i) {
		symbol_t& symbol = symbol_list[i];
		switch (symbol.second) {
		case parser_t::e_st_variable :
			resolveExpressionSymbol(symbol.first, token);
			break;
		default:
			break;
		}
	}

	if (std::isnan(expression.value())) {
		//string type in expression -> hex/octal
		expression_string.erase(0, 1);
		expression_string.erase(expression_string.size() - 1);
		return std::stoi(expression_string, 0, 0);
	}

	return expression.value();
}

void SectionContentTableBuilder::resolveExpressionSymbol(std::string expr_symbol, Token* token)
{
	int token_type = token->getType();
	if (token_type >= interrupt_instruction && token_type != stack_instruction && token_type != call_instruction ) { //instruction expression
		InstructionAsmToken* asmtoken = static_cast<InstructionAsmToken*>(token);
		throw MyException("Symbols are not allowed in this instruction: ", asmtoken->getLineNumber(), asmtoken->getPosition(), asmtoken->getName());
	}
	for (std::vector<Data*>::const_iterator it = symbols->begin(); it != symbols->end(); ++it) {
		SymbolData* symbol = static_cast<SymbolData*>(*it);
		if (symbol->label == expr_symbol && (token_type == stack_instruction || token_type == call_instruction) && symbol->section != name) {
			InstructionAsmToken* asmtoken = static_cast<InstructionAsmToken*>(token);
			throw MyException("Symbols from other sections are not allowed in this instruction: ", asmtoken->getLineNumber(), asmtoken->getPosition(), asmtoken->getName());

		}
		if (symbol->section != name && symbol->label == expr_symbol) {
			relocations.push_back(new ReloactionData(location_counter, it - symbols->begin()));
		}
	}
}

static inline unsigned short __builtin_bswap16(short int a)
{
	return (a << 8) | (a >> 8);
}

template <typename T>
std::string SectionContentTableBuilder::valueToString(T value)
{
	// if (typeid(T) == typeid(int) || typeid(T) == typeid(unsigned))
	//  value =  __builtin_bswap32(value);
	// else if (typeid(T) == typeid(short int))
	//  value =  __builtin_bswap16(value);
	std::stringstream stream;
	stream << std::setfill ('0') << std::setw(2 * sizeof(T))
	       << std::hex << (int)value;
	return stream.str();
}

void SectionContentTableBuilder::addInstruction(std::string & instruction, InstructionAsmToken * token)
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

std::vector<Data*>& SectionContentTableBuilder::getSymbols()
{
	return *symbols;
}

SectionInfo::SectionInfo(std::vector<Data*>& section_data, std::vector<Data*>& relocations, std::string name, int location_counter)
{
	this->section_data = section_data;
	this->relocations = relocations;
	this->name = name;
	this->size = location_counter;
}

SectionInfo::~SectionInfo()
{
	for (std::vector<Data*>::iterator it = section_data.begin(); it != section_data.end(); ++it) {
		delete *it;
	}
	for (std::vector<Data*>::iterator it = relocations.begin(); it != relocations.end(); ++it) {
		delete *it;
	}
}
