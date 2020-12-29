#pragma once
#include <vector>
#include <regex>
#include <string>
#include "token.h"

class lexical_analyzer
{
	static std::regex r_key_word;
	static std::regex r_identifier;
	static std::regex r_terminal;
	static std::regex r_literal;

	std::string data;

public:
	explicit lexical_analyzer(std::string data);

	std::vector<token> analyze();
};

static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}