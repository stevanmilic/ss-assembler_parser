#include "asmparser.h"

/**
 * AsmParser implementation
 */

void AsmParser::parse(std::ifstream& program)
{
	std::string line;
	int line_number = 1;
	while (std::getline(program, line)) {
		if (!parseLine(line, line_number++))
			break;//.end directive
	}
}

void AsmParser::getTokens(std::string& curr_line, int line_number, token_type type)
{
	boost::sregex_iterator line_end;
	boost::sregex_iterator* first_token = 0;
	switch (type) {
	case label:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), LabelToken::pattern);
		break;
	case directive:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), DirectiveToken::pattern);
		break;
	case instruction:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), InstructionToken::pattern);
		break;
	default:
		break;
	}
	for (boost::sregex_iterator token = *first_token; token != line_end; ++token) {
		switch (type) {
		case label:
			tokens.push_back(new LabelToken(line_number, *token));
			break;
		case directive:
			tokens.push_back(DirectiveToken::getDirective(line_number, *token));
			break;
		case instruction:
			tokens.push_back(InstructionToken::getInstruction(line_number, *token));
			break;
		default:
			break;
		}
	}
	delete first_token;
}
