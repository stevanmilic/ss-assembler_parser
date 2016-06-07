#include "linemanipulation.h"

std::vector<std::string>& LineManipulation::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if(!item.empty())
			elems.push_back(LineManipulation::trim(item));
	}
	return elems;
}


std::vector<std::string> LineManipulation::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string& LineManipulation::ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
std::string& LineManipulation::rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string& LineManipulation::trim(std::string &s)
{
	return ltrim(rtrim(s));
}


std::string& LineManipulation::whiteToOne(std::string &s)
{
	std::string::iterator new_end = std::unique(s.begin(), s.end(), BothAreSpaces);
	s.erase(new_end, s.end());
	return s;
}
