#ifndef _SECTIONCONTENTTABLEBUILDER_H
#define _SECTIONCONTENTTABLEBUILDER_H

#include "tablebuilder.h"
#include "sectiondata.h"
#include "symboldata.h"
#include "reloactiondata.h"
#include <iomanip>
#include <bitset>
#include <boost/regex.hpp>
#include <exprtk.hpp>
#include <iostream>

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>     expression_t;
typedef exprtk::parser<double>             parser_t;

class SectionContentTableBuilder: public TableBuilder
{
public:
	SectionContentTableBuilder(std::string, std::vector<SymbolData*>*);
	bool resolveToken(Token* t);
	int getLocation() const;
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
	bool resolveExpressionError(std:: string, symbol_table_t&);
	void addInstruction(std::string&, InstructionToken*);
	char getRegister(std::string);
	template <typename T>
	bool checkRange(T, unsigned);
	boost::regex undefined_symbol;
	std::vector<SectionData*> section_data;
	std::list<ReloactionData*> relocations;
	std::vector<SymbolData*>* symbols;
	std::string name;
	int location_counter;
};

#endif //_SECTIONCONTENTTABLEBUILDER_H
