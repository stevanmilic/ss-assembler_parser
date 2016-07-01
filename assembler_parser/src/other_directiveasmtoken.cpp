#include "directiveasmtoken.h"

/**
 * SectionDirectiveAsmToken implementation
 * IODirectiveAsmToken
 * TypeDirectiveAsmToken
 * AlignDirectiveAsmToken
 * SkipDirectiveAsmToken
 * EndDirectiveAsmToken
 */

SectionDirectiveAsmToken::SectionDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
	if (!content[2].str().empty()) {
		sub_section = LineManipulation::split(content[2].str(), '.')[0];
	}
}

IODirectiveAsmToken::IODirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
	symbols.push_back(content[2].str());
	LineManipulation::split(content[3].str(), ',', symbols);
}


TypeDirectiveAsmToken::TypeDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
	expressions.push_back(content[2].str());
	LineManipulation::split(content[3].str(), ',', expressions);
}


AlignDirectiveAsmToken::AlignDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
	//catch : 'std::invalid_argument'
	alignment = std::stoi(content[2].str());
	//default values
	fill = 0;
	max = 32;

	std::vector<std::string>params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 3) {
		throw MyException("Too much parametars for Directive: ", line_number, content.position(), content.str());
	}
	if (params.size() == 1) {
		fill = (char)std::stoi(params[0], 0, 0);
	} else if (params.size() == 2) {
		fill = (char)std::stoi(params[0], 0, 0);
		max = std::stoi(params[1], 0, 0);
	}
}

SkipDirectiveAsmToken::SkipDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
	// catch: 'std::invalid_argument'
	size = std::stoi(content[2].str(), 0, 0);
	//default value
	fill = 0;

	std::vector<std::string>params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 2) {
		throw MyException("Too much parametars for Directive: ", line_number, content.position(), content.str());
	}
	if (params.size() == 1) {
		fill = (char)std::stoi(params[0], 0, 0);
	}
}


EndDirectiveAsmToken::EndDirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveAsmToken(line_number, content)
{
}
