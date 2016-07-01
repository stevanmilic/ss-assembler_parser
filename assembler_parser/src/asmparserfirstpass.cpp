#include "asmparserfirstpass.h"
#include "symboltablebuilder.h"
#include "labelasmtoken.h"
#include "directiveasmtoken.h"
#include "instructionasmtoken.h"

/**
 * AsmParserFirstPass implementation
 */

AsmParserFirstPass::AsmParserFirstPass(std::string program_path)
{
	current_token = 0;
	program.open(program_path);
	table_builder = new SymbolTableBuilder();
}

void AsmParserFirstPass::parse()
{
	std::string line;
	int line_number = 1;
	while (std::getline(program, line)) {
		if (!parseLine(line, line_number++))
			break;//.end directive
	}
	if (error) {
		throw MyException("There was an error in Parsing while passing through code First time!", 0);
	}
}

bool AsmParserFirstPass::parseLine(std::string& line, int line_number)
{
	try {
		lex(line, line_number);
	} catch (MyException& e) {
		std::cout << e.what() << std::endl;
		error = true;
	}
	std::vector<Token*>::iterator token;
	for (token = tokens.begin() + current_token; token != tokens.end(); ++token) {
		try {
			if (!table_builder->resolveToken(*token))
				return false;
		} catch (MyException& e) {
			std::cout << e.what() << std::endl;
			error = true;
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
	getAsmTokens(line, line_number, label);
	getAsmTokens(line, line_number, directive);
	getAsmTokens(line, line_number, instruction);
	if ( line.length() && start_number_of_tokens == tokens.size()) {
		throw (MyException("Not declared in this scope:", line_number, 0, line));
	}
}

void AsmParserFirstPass::getAsmTokens(std::string& curr_line, int line_number, token_type type)
{
	boost::sregex_iterator line_end;
	boost::sregex_iterator* first_token = 0;
	switch (type) {
	case label:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), LabelAsmToken::pattern);
		break;
	case directive:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), DirectiveAsmToken::pattern);
		break;
	case instruction:
		first_token = new boost::sregex_iterator(curr_line.begin(), curr_line.end(), InstructionAsmToken::pattern);
		break;
	default:
		break;
	}
	for (boost::sregex_iterator token = *first_token; token != line_end; ++token) {
		switch (type) {
		case label:
			tokens.push_back(new LabelAsmToken(line_number, *token));
			break;
		case directive:
			tokens.push_back(DirectiveAsmToken::getDirective(line_number, *token));
			break;
		case instruction:
			tokens.push_back(InstructionAsmToken::getInstruction(line_number, *token));
			break;
		default:
			break;
		}
	}
	delete first_token;
}
