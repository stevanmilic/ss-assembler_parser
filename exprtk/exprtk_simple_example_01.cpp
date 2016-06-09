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


template <typename T>
void trig_function(std::string expression_string)
{
	typedef exprtk::symbol_table<T> symbol_table_t;
	typedef exprtk::expression<T>     expression_t;
	typedef exprtk::parser<T>             parser_t;
	typedef typename parser_t::
	dependent_entity_collector::symbol_t symbol_t;

	symbol_table_t symbol_table;

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;

	if (!parser.compile(expression_string, expression)) {

		printf("Error: %s\n", parser.error().c_str());
		// symbol_table.add_constant("main", 3);
		// parser.compile(expression_string, expression);

	}
	std::deque<symbol_t> symbol_list;

	double result = expression.value();

	if (isnan(result)) {
		expression_string.erase(0,1);
		expression_string.erase(expression_string.size() - 1);
		int test = std::stoi(expression_string, 0, 0);
		std::cout << test;
	}
}

int main(int argc, char* argv[])
{
	std::string expression = argv[1];
	trig_function<double>(expression);
	return 0;
}
