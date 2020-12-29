#include "analyzer.h"

std::regex analyzer::logic_and = std::regex("(\s*∧)|(\s*and)|(\\s*^)");
std::regex analyzer::logic_or = std::regex("(\s*or)|(\s*∨)|(\\s*V)");
std::regex analyzer::unary_operations = std::regex("(\\s*!)|(\\s*~)");
std::regex analyzer::open_bracket = std::regex("(\\s*\\()");
//std::regex analyzer::open_bracket = std::regex("(\\s*\\()|(\\s*\\[)|(\\s*\\{)");
std::regex analyzer::close_bracket = std::regex("(\\s*\\))");
//std::regex analyzer::close_bracket = std::regex("(\\s*\\))|(\\s*\\])|(\\s*\\})");
std::regex analyzer::identifiers = std::regex("^(\\s)*[_,a-z,A-Z][_,A-Z,a-z,0-9]*[_,A-Z,a-z,0-9]");

void analyzer::analyze(std::string& expr)
{


}
