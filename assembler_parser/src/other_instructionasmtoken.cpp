#include "instructionasmtoken.h"

/**
 * InterruptInstructionAsmToken
 * TermInstructionAsmToken
 * LogicalInstructionAsmToken
 * StackInstructionAsmToken
 * CallInstructionAsmToken
 * IOInstructionAsmToken
 * MoveInstructionAsmToken
 * LoadInstructionAsmToken
 */

InterruptInstructionAsmToken::InterruptInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	flag = "0";
	src = (char)std::stoi(content[2].str(), 0, 0);
	if (src < 0 || src > 15) {
		throw MyException("Interrupt must be a number between 0 and 15 for Instruction: ", line_number, content.position(), content.str());
	}
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 1) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
}

std::string InterruptInstructionAsmToken::getOC() const
{
	return "0000";
}

TermInstructionAsmToken::TermInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	if (!name.compare(0, 3, "mul")) {
		flag = "0";
	} else if (!name.compare(0, 3, "div")) {
		flag = "0";
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		throw MyException("Too much parametars for Instruction: ", line_number, content.position(), content.str());
	}
	src_imm = params[0];
}

std::string TermInstructionAsmToken::getOC() const
{
	if (!name.compare(0, 3, "add")) {
		return "0001";
	} else if (!name.compare(0, 3, "sub")) {
		return "0010";
	} else if (!name.compare(0, 3, "mul")) {
		return "0011";
	} else if (!name.compare(0, 3, "div")) {
		return "0100";
	} else if (!name.compare(0, 3, "cmp")) {
		return "0101";
	}
	return "";
}

LogicalInstructionAsmToken::LogicalInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	src = params[0];
}

std::string LogicalInstructionAsmToken::getOC() const
{
	if (!name.compare(0, 3, "and")) {
		return "0110";
	} else if (!name.compare(0, 2, "or")) {
		return "0111";
	} else if (!name.compare(0, 3, "not")) {
		return "1000";
	} else if (!name.compare(0, 4, "test")) {
		return "1001";
	}
	return "";
}

StackInstructionAsmToken::StackInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	flag = "0";
	r = content[2].str();
	f = "000";
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 3) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	a = params[0];
	ls = !name.compare(0, 3, "str") ? "0" : "1";
	if (!name.compare(3, 5, "ia")) {
		f = "010";
	} else if (!name.compare(3, 5, "ib")) {
		f = "100";
	} else if (!name.compare(3, 5, "da")) {
		f = "011";
	} else if (!name.compare(3, 5, "db")) {
		f = "101";
	}
	if (a == "psw")
		f = "000";
	imm = params.size() == 2 ? params[1] : "0";
}

std::string StackInstructionAsmToken::getOC() const
{
	return "1010";
}

CallInstructionAsmToken::CallInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	flag = "0";
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 2) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	imm = params.size() == 1 ? params[0] : "0";
}

std::string CallInstructionAsmToken::getOC() const
{
	return "1100";
}

IOInstructionAsmToken::IOInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	flag = "0";
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	src = params[0];
	io = !name.compare(0, 2, "in") ? "1" : "0";
}

std::string IOInstructionAsmToken::getOC() const
{
	return "1101";
}

MoveInstructionAsmToken::MoveInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 3) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	src = params[0];
	imm = params.size() == 2 ? params[1] : "0";
	lr = !name.compare(0, 3, "shr") ? "0" : "1";
}

std::string MoveInstructionAsmToken::getOC() const
{
	return "1110";
}

LoadInstructionAsmToken::LoadInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionAsmToken(line_number, content)
{
	flag = "0";
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		throw MyException("Wrong number of parametars for Instruction: ", line_number, content.position(), content.str());
	}
	c = params[0];
	hl = !name.compare(0, 4, "ldch") ? "1" : "0";
}

std::string LoadInstructionAsmToken::getOC() const
{
	return "1111";
}

int LoadInstructionAsmToken::getInstrSize() const
{
	if (!name.compare(0, 3, "ldc")) {
		return INSTRUCTION_SIZE_BYTES*2;
	}
	return INSTRUCTION_SIZE_BYTES;
}
