#include "asmparsersecondpass.h"
#include "compositetablebuilder.h"

/**
 * AsmParserSecondPass implementation
 */

AsmParserSecondPass::AsmParserSecondPass(TableBuilder* table_builder)
{
	AsmTableBuilder* asmtable_builder = static_cast<AsmTableBuilder*>(table_builder);
	this->table_builder = new CompositeTableBuilder(asmtable_builder);
}

AsmParserSecondPass::~AsmParserSecondPass()
{
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		delete *token;
	}
	tokens.clear();
}

void AsmParserSecondPass::parse()
{
	for (std::vector<Token*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
		try {
			if (!table_builder->resolveToken(*token))
				break;
		} catch (MyException& e) {
			std::cout << e.what() << std::endl;
			error = true;
		}
	}
	if (error) {
		throw MyException("There was an error in Parsing while passing through code Second time!", 0);
	}
}
