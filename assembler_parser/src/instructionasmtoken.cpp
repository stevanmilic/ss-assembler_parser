#include "instructionasmtoken.h"

/**
 * InstructionAsmToken implementation
 */

//Documentation for exprtk expression:
//  (01) Basic operators: +, -, *, /, %, ^
//
//  (02) Assignment:      :=, +=, -=, *=, /=, %=
//
//  (03) Equalities &
//       Inequalities:    =, ==, <>, !=, <, <=, >, >=
//
//  (04) Boolean logic:   and, mand, mor, nand, nor, not, or, shl, shr,
//                        xnor, xor, true, false

const boost::regex InstructionAsmToken::pattern("^(?:[a-zA-Z0-9]+:[ ]?)?([^.][a-zA-Z]+) ([0-9a-zA-Z ]+)((?:,[a-zA-Z0-9 /'%<>=*+-]+)*)$");

InstructionAsmToken::InstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : AsmToken(line_number, content)
{
	name = content[1].str();
	flag = "0";
	if (name.back() == 's') {
		name.erase(name.size() - 1);
		flag = "1";
	}
}

std::string InstructionAsmToken::getCondition() const
{
	if (!name.compare(name.length() - 2, 2, "eq")) {
		return "000";
	} else if (!name.compare(name.length() - 2, 2, "ne")) {
		return "001";
	} else if (!name.compare(name.length() - 2, 2, "gt")) {
		return "010";
	} else if (!name.compare(name.length() - 2, 2, "ge")) {
		return "011";
	} else if (!name.compare(name.length() - 2, 2, "lt")) {
		return "100";
	} else if (!name.compare(name.length() - 2, 2, "le")) {
		return "101";
	} else if (!name.compare(name.length() - 2, 2, "al")) {
		return "111";
	} else {
		return "110";
	}
}

std::string InstructionAsmToken::getFlag() const
{
	return flag;
}

std::string InstructionAsmToken::getName() const
{
	return name;
}

int InstructionAsmToken::getInstrSize() const
{
	//all instructions are the size of 4 bytes
	return INSTRUCTION_SIZE_BYTES;
}

InstructionAsmToken* InstructionAsmToken::getInstruction(int line_number, const boost::match_results<std::string::const_iterator>& content)
{
	std::string instr_name = content[1].str();
	if (!instr_name.compare(0, 3, "int")) {
		return new InterruptInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 3, "add") || !instr_name.compare(0, 3, "sub") || !instr_name.compare(0, 3, "mul") || !instr_name.compare(0, 3, "div") || !instr_name.compare(0, 3, "cmp")) {
		return new TermInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 3, "and") || !instr_name.compare(0, 2, "or") || !instr_name.compare(0, 3, "not") || !instr_name.compare(0, 4, "test")) {
		return new LogicalInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 3, "ldr") || !instr_name.compare(0, 3, "str")) {
		return new StackInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 4, "call")) {
		return new CallInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 2, "in") || !instr_name.compare(0, 3, "out")) {
		return new IOInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 3, "mov") || !instr_name.compare(0, 3, "shr") || !instr_name.compare(0, 3, "shl")) {
		return new MoveInstructionAsmToken(line_number, content);
	} else if (!instr_name.compare(0, 4, "ldch") || !instr_name.compare(0, 4, "ldcl") || !instr_name.compare(0, 3, "ldc")) {
		return new LoadInstructionAsmToken(line_number, content);
	} else {
		throw MyException("Undefined Instruction: ", line_number, content.position(), content.str());
		return 0;
	}
}
