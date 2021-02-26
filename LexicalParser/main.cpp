#include <iostream>
#include "code_loader.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"

int main() {
	code_loader loader("code2.c");

	auto code = loader.load_code();

	std::cout << code << "\n\n";

	lexical_analyzer lexical(code);

	auto tokens = lexical.analyze();

	for (auto t : tokens) {
		if (token_type::identifier == t.type) std::cout << "ident: ";
		if (token_type::key_word == t.type) std::cout << "key_w: ";
		if (token_type::literal == t.type) std::cout << "liter: ";
		if (token_type::terminal == t.type) std::cout << "termi: ";

		std::cout << t.data << std::endl;
	}

	for (auto t : tokens) {
		std::cout << t.data;
	}

	syntax_analyzer syntax(tokens);

	syntax.analyze();
	syntax.print(std::cout);

	return 0;
}