#ifndef _SECTIONCONTENTTABLEBUILDER_H
#define _SECTIONCONTENTTABLEBUILDER_H

#include "asmtablebuilder.h"
#include <exprtk.hpp>
//boost::regex included in asmtoken.h

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>     expression_t;
typedef exprtk::parser<double>             parser_t;
typedef typename parser_t::
dependent_entity_collector::symbol_t symbol_t;

class SectionContentTableBuilder: public AsmTableBuilder
{
public:
	SectionContentTableBuilder(std::string, std::vector<Data*>*);
	bool resolveToken(Token*);
	int getLocation() const;
	std::vector<Data*>& getSymbols();
	SectionInfo* getSectionInfo();
	std::string getName() const;
	std::ostream& dump(std::ostream& o) const;
protected:
	void resolveTypeDirective(Token*);
	void resolveSkipDirective(Token*);
	void resolveAlignDirective(Token*);
	void resolveInterruptInstruction(Token*);
	void resolveTermInstruction(Token*);
	void resolveLogicalInstruction(Token*);
	void resolveStackInstruction(Token*);
	void resolveCallInstruction(Token*);
	void resolveIOInstruction(Token*);
	void resolveMoveInstruction(Token*);
	void resolveLoadInstruction(Token*);
private:
	double resolveExpression(std::string, Token*);
	template <typename T>
	std::string valueToString(T value);
	void resolveExpressionSymbol(std:: string, Token*);
	void addInstruction(std::string&, InstructionAsmToken*);
	char getRegister(std::string);
	template <typename T>
	bool checkRange(T, unsigned);
	std::vector<Data*> section_data;
	std::vector<Data*> relocations;
	std::vector<Data*>* symbols;
	std::string name;
	int location_counter;
};

#endif //_SECTIONCONTENTTABLEBUILDER_H
