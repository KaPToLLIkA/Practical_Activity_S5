#pragma once
#include <string>
#include <list>
#include <vector>
#include <regex>
#include "token.h"


struct tree_node {
	std::string t;
	std::list<tree_node> childs;

	tree_node() {

	}

	tree_node(std::string t): t(t) {

	}
};


class syntax_analyzer
{
	std::regex r_types
		= std::regex(
			"(int)|(char)"
			"|(double)|(float)|(bool)");

	std::regex r_modificators
		= std::regex("(unsigned)|(signed)|(short)|(long)");

	std::regex r_opt_mod
		= std::regex("const");

	std::regex r_identifier
		= std::regex("^\\s*[_a-zA-Z][_A-Za-z0-9]*");

	std::regex r_logic_op
		= std::regex("(==)|(!=)|(<=)|(>=)|(>)|(<)");

	std::vector<token> tokens;
	std::vector<std::string> errors;
	bool has_error = false;
	tree_node root;

	size_t get_close_bracket(
		size_t start,
		size_t end,
		std::string open_bracket,
		std::string close_bracket
	);

	size_t get_first(
		size_t start,
		size_t end,
		std::string target
	);

	std::string tokens_to_str(size_t start, size_t end);
	void add_error(std::string text, size_t start, size_t end);

	void statement(tree_node& current_node, size_t start, size_t end);
	void statements(tree_node& current_node, size_t start, size_t end);
	void expression(tree_node& current_node, size_t start, size_t end);
	void optional_expression(tree_node& current_node, size_t start, size_t end);
	void definition(tree_node& current_node, size_t start, size_t end);
	void math_expr(tree_node& current_node, size_t start, size_t end);
	void logic_expr(tree_node& current_node, size_t start, size_t end);
	void term(tree_node& current_node, size_t start, size_t end);
	void factor(tree_node& current_node, size_t start, size_t end);
	void a(tree_node& current_node, size_t start, size_t end);
	void array_values(tree_node& current_node, size_t start, size_t end);
	

public:
	explicit syntax_analyzer(std::vector<token>& tokens);

	void analyze();

	tree_node& get_root();
	bool has_errors();
	std::vector<std::string> get_errors();
};

