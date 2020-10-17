
#include <iostream>
#include <regex>

#include "IdentifierPatterns.h"
#include "BinaryOperatorPatterns.h"

IdentifierPatterns* IdentifierPatterns::instance = nullptr;
BinaryOperatorPatterns* BinaryOperatorPatterns::instance = nullptr;

int main() {
	

	std::smatch matches;
	std::string str = "initial213 _13231dsad = dsaa 123dasdaf 13241241 _sdasflsd dasasjd _------dsa21 dasda";
	

	for (auto pattern : IdentifierPatterns::getInst()->getPatterns()) {
		bool hasData = std::regex_search(str, matches, pattern);
		str = matches.suffix();
		hasData = std::regex_search(str, matches, pattern);
		str = matches.suffix();
		hasData = std::regex_search(str, matches, pattern);
		str = matches.suffix();
		hasData = std::regex_search(str, matches, pattern);
		str = matches.suffix();
		hasData = std::regex_search(str, matches, pattern);
		str = matches.suffix();
		hasData = std::regex_search(str, matches, pattern);
	}

	
	return 0;
}