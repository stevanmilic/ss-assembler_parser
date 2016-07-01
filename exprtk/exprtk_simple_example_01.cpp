/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * Simple Example 1                                           *
 * Author: Arash Partow (1999-2016)                           *
 * URL: http://www.partow.net/programming/exprtk/index.html   *
 *                                                            *
 * Copyright notice:                                          *
 * Free use of the Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the  *
 * most current version of the Common Public License.         *
 * http://www.opensource.org/licenses/cpl1.0.php              *
 *                                                            *
 **************************************************************
*/


#include <iostream>
#include <string>
#include "exprtk.hpp"


void test_function()
{
	typedef exprtk::symbol_table<double> symbol_table_t;
	typedef exprtk::expression<double>     expression_t;
	typedef exprtk::parser<double>             parser_t;

	typedef typename parser_t::
	dependent_entity_collector::symbol_t symbol_t;

	std::string expression_string =
	    "z + g + b";

	double x;

	parser_t parser;
	symbol_table_t symbol_table;

	// symbol_table.add_variable("z", r1);
	// symbol_table.add_variable("g", r2);

	symbol_table.add_constant("z", 5);
	symbol_table.add_constant("g", 10);
	symbol_table.add_constant("b", 20);

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	//Collect only variable
	parser.dec().collect_variables() = true;
	parser.next_token();


	if (!parser.compile(expression_string, expression)) {
		// error....
		std::cout << parser.error() << std::endl;
		exit(1);
	}

	std::deque<symbol_t> symbol_list;

	parser.dec().symbols(symbol_list);

	for (std::size_t i = 0; i < symbol_list.size(); ++i) {
		symbol_t& symbol = symbol_list[i];

		switch (symbol.second) {
		case parser_t::e_st_variable :
			std::cout << "Var: " << symbol.first << std::endl;
			break;
		case parser_t::e_st_vector   :
			std::cout << "Vector: " << symbol.first << std::endl;
			break;
		case parser_t::e_st_string   :
			std::cout << "String: " << symbol.first << std::endl;
			break;
		case parser_t::e_st_function :
			std::cout << "Function: " << symbol.first << std::endl;
			break;
		case 0:
			std::cout << "Unknown: " << symbol.first << std::endl;
			break;
		default:
			break;
		}
	}
	x = 2;
	std::cout << expression.value() << std::endl;
}

int main()
{
	test_function();
	return 0;
}
