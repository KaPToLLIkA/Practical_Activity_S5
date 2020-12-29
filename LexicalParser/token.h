#pragma once
#include <string>

enum token_type {
	key_word,
	identifier,
	terminal,
	literal
};


struct token {
	token_type type;
	std::string data;
};