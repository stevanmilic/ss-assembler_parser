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


#include <cstdio>
#include <string>
#include "exprtk.hpp"


template <typename T>
void trig_function()
{
	typedef exprtk::symbol_table<T> symbol_table_t;
	typedef exprtk::expression<T>     expression_t;
	typedef exprtk::parser<T>             parser_t;

	std::string expression_string = "main + 2 + gsda";

	symbol_table_t symbol_table;

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	if (!parser.compile(expression_string, expression)) {

		printf("Error: %s\n", parser.error().c_str());
		symbol_table.add_constant("main",3);
		parser.compile(expression_string, expression);
		printf("Error: %s\n", parser.error().c_str());

	}
	double result = expression.value();

	printf("Result: %10.5f\n", result);
}

int main()
{
	trig_function<double>();
	return 0;
}
