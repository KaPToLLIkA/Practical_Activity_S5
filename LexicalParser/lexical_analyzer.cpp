#include "lexical_analyzer.h"

std::regex lexical_analyzer::r_key_word =
	std::regex("(^\\s*const)|(^\\s*int)|(^\\s*char)|(^\\s*double)|"
		"(^\\s*float)|(^\\s*bool)|(^\\s*unsigned)|(^\\s*signed)|"
		"(^\\s*short)|(^\\s*long)|(^\\s*if)|(^\\s*for)|(^\\s*do)|"
		"(^\\s*while)|(^\\s*else)");


std::regex lexical_analyzer::r_identifier =
	std::regex("^\\s*[_a-zA-Z][_A-Za-z0-9]*");


std::regex lexical_analyzer::r_literal =
std::regex("(^\\s*[+-]?\\d+\\.?\\d*)"
	"|(^\\s*\\\"[^\\\"]*\\\")"
	"|(^\\s*\\'[^\\']*\\')");


std::regex lexical_analyzer::r_terminal =
std::regex("(^\\s*==)|(^\\s*=)|(^\\s*\\()|(^\\s*!=)"
	"|(^\\s*<=)|(^\\s*>=)|(^\\s*<)|(^\\s*>)|(^\\s*\\))"
	"|(^\\s*\\{)|(^\\s*\\})|(^\\s*;)|(^\\s*\\[)|(^\\s*\\])"
	"|(^\\s*\\*)|(^\\s*\\+)|(^\\s*\\-)|(^\\s*\\/)|(^\\s*%)|(^\\s*,)");


lexical_analyzer::lexical_analyzer(std::string data):
	data(data)
{
}

std::vector<token> lexical_analyzer::analyze()
{
	std::vector<token> tokens;

	trim(this->data);
	
	while (!this->data.empty()) {
		std::smatch matches;
		token t;

		if (std::regex_search(this->data, matches, r_key_word)) {
			std::string key_word = matches[0].str();

			std::regex_search(this->data, matches, r_identifier);

			if (key_word.size() == matches[0].length()) {
				t.type = token_type::key_word;
				t.data = trim_copy(key_word);
			}
			else {
				t.type = token_type::identifier;
				t.data = trim_copy(matches[0].str());
			}
			tokens.push_back(t);
			this->data = matches.suffix();
			continue;
		}

		if (std::regex_search(this->data, matches, r_identifier)) {
			t.type = token_type::identifier;
			t.data = trim_copy(matches[0].str());
			
			tokens.push_back(t);
			this->data = matches.suffix();
			continue;
		}

		if (std::regex_search(this->data, matches, r_terminal)) {
			t.type = token_type::terminal;
			t.data = trim_copy(matches[0].str());

			tokens.push_back(t);
			this->data = matches.suffix();
			continue;
		}

		if (std::regex_search(this->data, matches, r_literal)) {
			t.type = token_type::literal;
			t.data = trim_copy(matches[0].str());

			tokens.push_back(t);
			this->data = matches.suffix();
			continue;
		}

		throw std::exception((this->data + "lexical error at >>> ").c_str());
	}

	return tokens;
}
