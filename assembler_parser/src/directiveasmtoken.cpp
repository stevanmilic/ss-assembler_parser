#include "directiveasmtoken.h"

/**
 * DirectiveAsmToken implementation
 */

const boost::regex DirectiveAsmToken::pattern("^(?:[a-zA-Z0-9]+:[ ]?)?\\.([a-zA-Z]+)(?|\\.([a-zA-Z]+)| ([0-9a-zA-Z '%<>=/*+-]+)((?:,[a-zA-Z0-9 '%<>=/*+-]+)*))?$");

DirectiveAsmToken::DirectiveAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content) : AsmToken(line_number, content)
{
	section = content[1].str();
}

std::string DirectiveAsmToken::getSection() const
{
	return section;
}

DirectiveAsmToken* DirectiveAsmToken::getDirective(int line_number, const boost::match_results<std::string::const_iterator>& content)
{
	std::string section = content[1].str();
	if (section == "text" || section == "data" || section == "bss") {
		return new SectionDirectiveAsmToken(line_number, content);
	} else if (section == "public" || section == "extern") {
		return new IODirectiveAsmToken(line_number, content);
	} else if (section == "long" || section == "char" || section == "word") {
		return new TypeDirectiveAsmToken(line_number, content);
	} else if (section ==  "align") {
		return new AlignDirectiveAsmToken(line_number, content);
	} else if (section == "skip") {
		return new SkipDirectiveAsmToken(line_number, content);
	} else if (section == "end") {
		return new EndDirectiveAsmToken(line_number, content);
	} else {
		throw MyException("Undefined Directive: ", line_number, content.position(), content.str());
		return 0;
	}
}
