#pragma once
#include <string>

struct Token {
	unsigned start = 0, end = 0;
	std::string str;
	std::string::iterator istart, iend;
};

