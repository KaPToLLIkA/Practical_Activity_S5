#pragma once
#include <string>
#include <regex>

struct Token {
	size_t start = 0, end = 0;
	std::string str;
	std::string::const_iterator istart, iend;

	Token() {};

	Token(std::string& str) 
	{
		this->str = std::regex_replace(str, std::regex("\\s*"), "");
	};

	Token(std::string& str, size_t start, size_t end)
		: start(start), end(end) 
	{
		this->str = std::regex_replace(str, std::regex("\\s*"), "");
	};

	Token(std::string::const_iterator& istart, std::string::const_iterator& iend)
		: istart(istart), iend(iend) 
	{
		std::string tmpStr(istart, iend);
		this->str = std::regex_replace(tmpStr, std::regex("\\s*"), "");
	};

	Token(size_t start, size_t end, std::string::const_iterator& istart, std::string::const_iterator& iend)
		: start(start), end(end), istart(istart), iend(iend) 
	{
		std::string tmpStr(istart, iend);
		this->str = std::regex_replace(tmpStr, std::regex("\\s*"), "");
	};
};

