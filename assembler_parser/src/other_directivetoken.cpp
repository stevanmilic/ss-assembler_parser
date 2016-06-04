#include "directivetoken.h"

/**
 * SectionDirectiveToken implementation
 * IODirectiveToken
 * TypeDirectiveToken
 * AlignDirectiveToken
 * SkipDirectiveToken
 * EndDirectiveToken
 */

SectionDirectiveToken::SectionDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
	sub_section = content[2];
}

IODirectiveToken::IODirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
	if (content[2].str().empty()) {
		;//throw errror -> no symbols
	}
	symbols.push_back(content[2].str());
	LineManipulation::split(content[3].str(), ',', symbols);
}


TypeDirectiveToken::TypeDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
	expressions.push_back(content[2].str());
	LineManipulation::split(content[3].str(), ',', expressions);
}


AlignDirectiveToken::AlignDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
	if (content[2].str().empty()) {
		;//throw error -> undefined alignment
	}
	//catch : 'std::invalid_argument'
	alignment = std::stoi(content[2].str());
	//default values
	fill = 0;
	max = 32;

	std::vector<std::string>params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 3) {
		;//throw error -> too much params
	}
	if (params.size() == 1) {
		fill = std::stoi(params[0], 0, 0);
	} else if (params.size() == 2) {
		fill = std::stoi(params[0], 0, 0);
		max = std::stoi(params[1], 0, 0);
	}
}

SkipDirectiveToken::SkipDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
	if (content[2].str().empty()) {
		;//throw error -> undefined alignment
	}
	// catch: 'std::invalid_argument'
	size = std::stoi(content[2].str(), 0, 0);
	//default value
	fill = 0;

	std::vector<std::string>params = LineManipulation::split(content[3].str(), ',');
	if (params.size() >= 2) {
		;//throw error too much arguments
	}
	if (params.size() == 1) {
		fill = std::stoi(params[0], 0, 0);
	}
}


EndDirectiveToken::EndDirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : DirectiveToken(line_number, content)
{
}
