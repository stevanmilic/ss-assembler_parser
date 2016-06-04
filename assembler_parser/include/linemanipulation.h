//line_manupulation.h
#ifndef _line_manipulation_h_
#define _line_manipulation_h_
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;

class LineManipulation
{
public:
	static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);

	static std::string& whiteToOne(std::string &s);
	static bool BothAreSpaces(char lhs, char rhs)
	{
		return (lhs == rhs) && (lhs == ' ');
	}

	static std::string &ltrim(std::string &s);
	static std::string &rtrim(std::string &s);
	static std::string &trim(std::string &s);
};
#endif
