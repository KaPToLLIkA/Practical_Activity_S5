
#include <iostream>
#include <regex>

#include "IdentifierPatterns.h"
#include "BinaryOperatorPatterns.h"
#include "InstructionsSeparatorPatterns.h"
#include "KeyWordPatterns.h"
#include "LiteralsPatterns.h"

#include "Token.h"

IdentifierPatterns*				IdentifierPatterns::instance			= nullptr;
BinaryOperatorPatterns*			BinaryOperatorPatterns::instance		= nullptr;
InstructionsSeparatorPatterns*	InstructionsSeparatorPatterns::instance = nullptr;
KeyWordPatterns*				KeyWordPatterns::instance				= nullptr;
LiteralsPatterns*				LiteralsPatterns::instance				= nullptr;


struct FindResult {
	Token token;
	bool success = false;

	FindResult() {};

	FindResult(Token token, bool success)
		: token(token), success(success) {};
};

FindResult findToken(std::string& str, const std::vector<std::regex>& patterns);



int main() {
	
	FindResult result;
	std::vector<Token> tokens;
	std::string str;
	
	while (true) {
		std::cout << "Print expressions: ";
		std::getline(std::cin, str, '\n');

		tokens.clear();

		while (!str.empty()) {
			result = findToken(str, IdentifierPatterns::getInst()->getPatterns());
			if (result.success) {
				tokens.push_back(result.token);
			}
			else {
				std::cout << "Can't find identifier at start of " << str << std::endl;
				break;
			}


			result = findToken(str, BinaryOperatorPatterns::getInst()->getPatterns());
			if (result.success) {
				tokens.push_back(result.token);
			}
			else {
				std::cout << "Can't find binary operator at start of " << str << std::endl;
				break;
			}

			result = findToken(str, LiteralsPatterns::getInst()->getPatterns());
			if (result.success) {
				tokens.push_back(result.token);
			}
			else {
				std::cout << "Can't find literal at start of " << str << std::endl;
				break;
			}

			result = findToken(str, InstructionsSeparatorPatterns::getInst()->getPatterns());
			if (result.success) {
				tokens.push_back(result.token);
			}
			else {
				std::cout << "Can't find instructions separator at start of " << str << std::endl;
				break;
			}

			while (true) {
				result = findToken(str, KeyWordPatterns::getInst()->getPatterns());
				if (result.success) {
					tokens.push_back(result.token);
				}
				else {
					std::cout << "Can't find key word at start of " << str << std::endl;
					break;
				}

				result = findToken(str, InstructionsSeparatorPatterns::getInst()->getPatterns());
				if (result.success) {
					tokens.push_back(result.token);
				}
				else {
					std::cout << "Can't find instructions separator at start of " << str << std::endl;
					break;
				}
			}
		}

		std::cout << "Result set: \n";

		for (auto token : tokens) {
			std::cout << token.str << "\n";
		}

	}

	

	
	return 0;
}

FindResult findToken(std::string& str, const std::vector<std::regex>& patterns)
{
	std::smatch matches;
	FindResult result;
	for (auto pattern : patterns) {
		result.success = std::regex_search(str, matches, pattern);

		if (result.success) {
			result.token = Token((std::string&)matches[0].str());
			str = matches.suffix();
			break;
		}
	}

	return result;
}