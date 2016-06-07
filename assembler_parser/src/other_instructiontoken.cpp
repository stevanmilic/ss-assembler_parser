#include "instructiontoken.h"

/**
 * InterruptInstructionToken
 * TermInstructionToken
 * LogicalInstructionToken
 * StackInstructionToken
 * CallInstructionToken
 * IOInstructionToken
 * MoveInstructionToken
 * LoadInstructionToken
 */

InterruptInstructionToken::InterruptInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error -> not enough params
	}
	src = (char)std::stoi(content[2].str(), 0, 0);
	if(src < 0 || src > 15){
		return;//throw error interrupt must be number between 0 and 15
	}
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 1) {
		return;//throw error wrong number of parametars
	}
}

std::string InterruptInstructionToken::getFlag() const
{
	return "0";
}

std::string InterruptInstructionToken::getOC() const
{
	return "0000";
}

TermInstructionToken::TermInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error -> not enough params
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() <= 1 || params.size() >= 3) {
		return;//throw error wrong number of parametars
	}
	bit = params[0];
	if (bit == "0") {
		src = params[1];
	} else if (bit == "1") {
		imm = params[1];
	} else {
		return;//throw error -> must be 0 or 1
	}
}

std::string TermInstructionToken::getFlag() const
{
	return "1";
}

std::string TermInstructionToken::getOC() const
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

LogicalInstructionToken::LogicalInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		return;//throw error wrong number of params
	}
	src = params[0];
}

std::string LogicalInstructionToken::getFlag() const
{
	return "1";
}

std::string LogicalInstructionToken::getOC() const
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

StackInstructionToken::StackInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	a = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		return;//throw error wrong number of params
	}
	r = params[0];
	imm = params.size() == 2 ? params[1] : 0;
	ls = !name.compare(0, 3, "str") ? "0" : "1";
}

std::string StackInstructionToken::getFlag() const
{
	return "0";
}

std::string StackInstructionToken::getOC() const
{
	return "1010";
}

CallInstructionToken::CallInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		return;//throw error wrong number of params
	}
	imm = params[0];
}

std::string CallInstructionToken::getFlag() const
{
	return "0";
}

std::string CallInstructionToken::getOC() const
{
	return "1100";
}

IOInstructionToken::IOInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 2) {
		return;//throw error wrong number of params
	}
	src = params[0];
	io = !name.compare(0, 2, "in") ? "1" : 0;
}

std::string IOInstructionToken::getFlag() const
{
	return "0";
}

std::string IOInstructionToken::getOC() const
{
	return "1101";
}

MoveInstructionToken::MoveInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() == 0 || params.size() >= 3) {
		return;//throw error wrong number of params
	}
	src = params[0];
	imm = params.size() == 2 ? params[1] : 0;
	lr = !name.compare(0, 3, "shr") ? "0" : "1";
}

std::string MoveInstructionToken::getFlag() const
{
	return "1";
}

std::string MoveInstructionToken::getOC() const
{
	return "1110";
}

LoadInstructionToken::LoadInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : InstructionToken(line_number, content)
{
	if (content[2].str().empty()) {
		return;//throw error: no arguments
	}
	dst = content[2].str();
	std::vector<std::string> params = LineManipulation::split(content[3].str(), ',');
	if (params.size() <= 1 || params.size() >= 3) {
		return;//throw error wrong number of params
	}
	dst = params[0];
	c = params[1];
	hl = !name.compare(0, 4, "ldch") ? "1" : "0";
}

std::string LoadInstructionToken::getFlag() const
{
	return "0";
}

std::string LoadInstructionToken::getOC() const
{
	return "1111";
}
