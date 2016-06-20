#include "directivetoken.h"

/**
 * DirectiveToken implementation
 */

const boost::regex DirectiveToken::pattern("^(?:[a-zA-Z0-9]+:[ ]?)?\\.([a-zA-Z]+)(?|\\.([a-zA-Z]+)| ([0-9a-zA-Z '%<>=/*+-]+)((?:,[a-zA-Z0-9 '%<>=/*+-]+)*))?$");

DirectiveToken::DirectiveToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : Token(line_number, content)
{
	section = content[1].str();
}

std::string DirectiveToken::getSection() const
{
	return section;
}

DirectiveToken* DirectiveToken::getDirective(int line_number, const boost::match_results<std::string::const_iterator>& content)
{
	std::string section = content[1].str();
	if (section == "text" || section == "data" || section == "bss") {
		return new SectionDirectiveToken(line_number, content);
	} else if (section == "public" || section == "extern") {
		return new IODirectiveToken(line_number, content);
	} else if (section == "long" || section == "char" || section == "word") {
		return new TypeDirectiveToken(line_number, content);
	} else if (section ==  "align") {
		return new AlignDirectiveToken(line_number, content);
	} else if (section == "skip") {
		return new SkipDirectiveToken(line_number, content);
	} else if (section == "end") {
		return new EndDirectiveToken(line_number, content);
	} else {
		throw MyException("Undefined Directive: ", line_number, content.position(), content.str());
		return 0;
	}
}
