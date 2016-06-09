#include "asmparserfirstpass.h"

/**
 * AsmParserFirstPass implementation
 */

AsmParserFirstPass::AsmParserFirstPass()
{
	current_token = 0;
}

void AsmParserFirstPass::parse(std::ifstream& program)
{
	std::string line;
	int line_number = 1;
	while (std::getline(program, line)) {
		if (!parseLine(line, line_number++))
			break;//.end directive
	}
}

bool AsmParserFirstPass::parseLine(std::string& line, int line_number)
{
	try {
		lex(line, line_number);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	std::vector<Token*>::iterator token;
	for (token = tokens.begin() + current_token; token != tokens.end(); ++token) {
		try {
			if (!symbol_table.resolveToken(*token))
				return false;
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	current_token = token - tokens.begin();
	return true;
}

void AsmParserFirstPass::lex(std::string& line, int line_number)
{
	LineManipulation::trim(line);
	LineManipulation::whiteToOne(line);
	unsigned start_number_of_tokens = tokens.size();
	getTokens(line, line_number, label);
	getTokens(line, line_number, directive);
	getTokens(line, line_number, instruction);
	if ( line.length() && start_number_of_tokens == tokens.size()) {
		throw (MyException("Not declared in this scope:", line_number, 0, line));
	}
}

void AsmParserFirstPass::getTokens(std::string& curr_line, int line_number, token_type type)
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

void AsmParserFirstPass::printTables() const
{
	std::cout << symbol_table << endl;
}

std::vector<SymbolData*> AsmParserFirstPass::getSymbols()
{
	return symbol_table.getSymbols();
}
