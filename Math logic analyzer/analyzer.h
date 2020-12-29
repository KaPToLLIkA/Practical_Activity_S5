#pragma once
#include <regex>


class analyzer
{
private:
	static std::regex
		unary_operations,
		logic_and,
		logic_or,
		open_bracket,
		close_bracket,
		identifiers;

	std::vector<std::string> tokens;


	void find_term(std::string& expr);
	void find_kon(std::string& expr);
	void find_fal(std::string& expr);

	

public:

	explicit analyzer() {}
	
	void analyze(std::string& expr);
	std::vector<std::string>& const get_tokens();

};

