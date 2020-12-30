#include "syntax_analyzer.h"

size_t syntax_analyzer::get_close_bracket(
	size_t start,
	size_t end,
	std::string open_bracket, 
	std::string close_bracket)
{
	int balance = 1;
	for (size_t i = start; i <= end; ++i) {
		if (tokens[i].data == open_bracket) {
			++balance;
		}
		else if (tokens[i].data == close_bracket) {
			--balance;
			if (balance == 0) {
				return i;
			}
		}
	}
	return std::string::npos;
}

size_t syntax_analyzer::get_first(
	size_t start,
	size_t end,
	std::string target)
{
	for (size_t i = start; i <= end; ++i) {
		if (tokens[i].data == target) {
			return i;
		}
	}
	return std::string::npos;
}

std::string syntax_analyzer::tokens_to_str(size_t start, size_t end)
{
	std::string str;
	for (size_t i = start; i <= end; ++i) {
		str += tokens[i].data;
	}
	return str;
}

void syntax_analyzer::add_error(std::string text, size_t start, size_t end)
{
	errors.push_back(text
		+ tokens_to_str(start, end));
}

void syntax_analyzer::statement(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		add_error("no statement", start, end);
		return;
	}
	
	std::string& token = tokens[start].data;

	if (token == "{") {
		size_t pos = get_close_bracket(start + 1, end, "{", "}");
		if (pos > end) {
			add_error("can't find close \"}\" bracket", start, end);
		}
		else {
			current_node.childs.push_back(tree_node("{"));
			current_node.childs.push_back(tree_node("statements"));
			statements(current_node.childs.back(), start + 1, pos - 1);
			current_node.childs.push_back(tree_node("}"));

		}
	}
	else if (token == "if") {
		if (tokens[start + 1].data == "(") {
			size_t pos = get_close_bracket(start + 2, end, "(", ")");

			if (pos > end) {
				add_error("can't find close bracket \")\" in if", start, end);
			}
			else {
				current_node.childs.push_back(tree_node("if"));
				current_node.childs.push_back(tree_node("("));
				current_node.childs.push_back(tree_node("expression"));
				expression(current_node.childs.back(), start + 2, pos - 1);
				current_node.childs.push_back(tree_node(")"));


				start = pos + 1;
				current_node.childs.push_back(tree_node("statement"));

				size_t else_p = get_first(start, end, "else");
				size_t next_if = get_first(start, end, "if");

				if (else_p > end || else_p > next_if) {
					statement(current_node.childs.back(), start, end);
				}
				else {
					statement(current_node.childs.back(), start, else_p - 1);
					current_node.childs.push_back(tree_node("else"));
					current_node.childs.push_back(tree_node("statement"));
					statement(current_node.childs.back(), else_p + 1, end);
				}
			}
		}
		else {
			add_error("can't find open bracket \"(\" after if", start, end);
		}
	}
	else if (token == "for") {
		if (tokens[start + 1].data == "(") {
			size_t pos = get_close_bracket(start + 2, end, "(", ")");

			if (pos > end) {
				add_error("can't find open bracket \")\" in for", start, end);
			}
			else {
				current_node.childs.push_back(tree_node("for"));
				current_node.childs.push_back(tree_node("("));
				start += 1;

				size_t sep_pos = start;
				bool loop_success = true;

				for (size_t i = 0; i < 2; ++i) {
					start = sep_pos + 1;
					sep_pos = get_first(start, pos - 1, ";");

					if (sep_pos > pos - 1) {
						add_error("can't find separator at ", start, pos);
						loop_success = false;
						break;
					}
					else {
						current_node.childs.push_back(tree_node("optional_expression"));
						optional_expression(current_node.childs.back(), start, sep_pos - 1);
						current_node.childs.push_back(tree_node(";"));
					}
				}

				if (loop_success) {
					current_node.childs.push_back(tree_node("optional_expression"));
					optional_expression(current_node.childs.back(), sep_pos + 1, pos - 1);
					current_node.childs.push_back(tree_node(")"));
					current_node.childs.push_back(tree_node("statement"));
					statement(current_node.childs.back(), pos + 1, end);
				}
			}
		}
		else {
			add_error("can't find open bracket \"(\" after for", start, end);
		}


	}
	else if (token == "while") {
		if (tokens[start + 1].data == "(") {
			size_t pos = get_close_bracket(start + 2, end, "(", ")");

			if (pos > end) {
				add_error("can't find close \")\" bracket after while", start, end);
			}
			else {
				current_node.childs.push_back(tree_node("while"));
				current_node.childs.push_back(tree_node("("));
				current_node.childs.push_back(tree_node("expression"));
				expression(current_node.childs.back(), start + 2, pos - 1);
				current_node.childs.push_back(tree_node(")"));
				current_node.childs.push_back(tree_node("statement"));
				statement(current_node.childs.back(), pos + 1, end);
			}
		}
		else {
			add_error("can't find open \"(\" bracket after while", start, end);
		}
	}
	else if (token == "do") {
		size_t w_pos = get_first(start + 1, end, "while");
		current_node.childs.push_back(tree_node("do"));
		current_node.childs.push_back(tree_node("statement"));
		statement(current_node.childs.back(), start + 1, w_pos - 1);
		current_node.childs.push_back(tree_node("while"));

		start = w_pos + 1;

		if (tokens[start].data == "(") {
			size_t pos = get_close_bracket(start + 1, end, "(", ")");

			if (pos > end) {
				add_error("can't find close \")\" bracket after while", start, end);
			}
			else {
				current_node.childs.push_back(tree_node("("));
				current_node.childs.push_back(tree_node("expression"));
				expression(current_node.childs.back(), start + 1, pos - 1);
				current_node.childs.push_back(tree_node(")"));
			}
		}
		else {
			add_error("can't find open \"(\" bracket after while", start, end);
		}
	}
	else {
		size_t sep_pos = get_first(start, end, ";");
		current_node.childs.push_back(tree_node("expression"));
		expression(current_node.childs.back(), start, sep_pos - 1);
		current_node.childs.push_back(tree_node(";"));
	}
}

void syntax_analyzer::statements(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		return;
	}

	std::string token = tokens[start].data;
	if (token == "{" || token == "if" || token == "for" || token == "while" || token == "do") {
		current_node.childs.push_back(tree_node("statement"));
		statement(current_node.childs.back(), start, end);
		
	}
	else {
		size_t sep_pos = get_first(start, end, ";");

		if (sep_pos > end) {
			add_error("can't find separator \";\"", start, end);
		}
		else {
			current_node.childs.push_back(tree_node("expression"));
			expression(current_node.childs.back(), start, sep_pos - 1);
			current_node.childs.push_back(tree_node(";"));
			current_node.childs.push_back(tree_node("statements"));
			statements(current_node.childs.back(), sep_pos + 1, end);
		}
	}
}

void syntax_analyzer::expression(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		return;
	}

	if (std::regex_match(tokens[start].data, r_identifier)
		&& !std::regex_match(tokens[start].data, r_types)
		&& !std::regex_match(tokens[start].data, r_modificators)
		&& !std::regex_match(tokens[start].data, r_opt_mod)) {
		current_node.childs.push_back(tree_node("identifier"));
		current_node.childs.back().childs.push_back(tokens[start].data);

		if (start + 1 <= end && tokens[start + 1].data == "=") {
			current_node.childs.push_back(tree_node("="));
			start += 1;
			if (start + 1 <= end) {
				if (tokens[start + 1].type == token_type::literal) {
					current_node.childs.push_back(tree_node("literal"));
					current_node.childs.back().childs.push_back(tree_node(tokens[start + 1].data));
				}
				else {
					if (get_first(start, end, "+") != std::string::npos
						|| get_first(start, end, "-") != std::string::npos
						|| get_first(start, end, "/") != std::string::npos
						|| get_first(start, end, "*") != std::string::npos
						|| get_first(start, end, "%") != std::string::npos
						) {
						current_node.childs.push_back(tree_node("math_expr"));
						math_expr(current_node.childs.back(), start + 1, end);
					}
					else {
						current_node.childs.push_back(tree_node("expression"));
						expression(current_node.childs.back(), start + 1, end);
					}
				}
			}
			else {
				add_error("has no expression ", start - 1, end);
			}
		}
	}
	else if (get_first(start, end, "int") != std::string::npos
		|| get_first(start, end, "bool") != std::string::npos
		|| get_first(start, end, "char") != std::string::npos
		|| get_first(start, end, "double") != std::string::npos
		|| get_first(start, end, "float") != std::string::npos
		) {
		current_node.childs.push_back(tree_node("definition"));
		definition(current_node.childs.back(), start, end);
		if (current_node.childs.back().childs.empty()) {
			current_node.childs.pop_back();
		}
	}
	else if (get_first(start, end, "+") != std::string::npos
		|| get_first(start, end, "-") != std::string::npos
		|| get_first(start, end, "/") != std::string::npos
		|| get_first(start, end, "*") != std::string::npos
		|| get_first(start, end, "%") != std::string::npos
		) {
		current_node.childs.push_back(tree_node("math_expr"));
		math_expr(current_node.childs.back(), start, end);
		if (current_node.childs.back().childs.empty()) {
			current_node.childs.pop_back();
		}
	}
	else if (get_first(start, end, "==") != std::string::npos
		|| get_first(start, end, "!=") != std::string::npos
		|| get_first(start, end, "<=") != std::string::npos
		|| get_first(start, end, ">=") != std::string::npos
		|| get_first(start, end, "<") != std::string::npos
		|| get_first(start, end, ">") != std::string::npos
		) {
		current_node.childs.push_back(tree_node("logic_expr"));
		logic_expr(current_node.childs.back(), start, end);
		if (current_node.childs.back().childs.empty()) {
			current_node.childs.pop_back();
		}
	}
}

void syntax_analyzer::optional_expression(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		return;
	}

	current_node.childs.push_back(tree_node("expression"));
	expression(current_node.childs.back(), start, end);
}

void syntax_analyzer::definition(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		return;
	}

	if (std::regex_match(tokens[start].data, r_opt_mod)) {
		current_node.childs.push_back(tree_node(tokens[start].data));
		start += 1;
	}

	if (start <= end && std::regex_match(tokens[start].data, r_modificators)) {
		current_node.childs.push_back(tree_node(tokens[start].data));
		start += 1;
	}

	if (start <= end && std::regex_match(tokens[start].data, r_types)) {
		current_node.childs.push_back(tree_node(tokens[start].data));
		start += 1;
	}
	else {
		add_error("illegal definition", start, end);
		return;
	}

	if (start <= end && tokens[start].type == token_type::identifier) {
		current_node.childs.push_back(tree_node("identifier"));
		current_node.childs.back().childs.push_back(tokens[start].data);
		
		start += 1;
		if (start <= end && tokens[start].data == "[") {
			current_node.childs.push_back(tree_node("["));

			start += 1;
			if (start <= end && tokens[start].type == token_type::literal) {
				current_node.childs.push_back(tree_node("literal"));
				current_node.childs.back().childs.push_back(tokens[start].data);
				start += 1;
				if (start <= end && tokens[start].data == "]") {
					current_node.childs.push_back(tree_node("]"));
					start += 1;
				}
				else {
					add_error("illegal array definition ", start, end);
					return;
				}
			}
			else {
				add_error("illegal array definition ", start, end);
				return;
			}
		}
		
		if (start <= end && tokens[start].data == "=") {
			current_node.childs.push_back(tree_node("="));
			start += 1;
			if ((start <= end && tokens[start].type == token_type::literal)
				|| (start <= end && tokens[start].type == token_type::identifier)) {
				if (tokens[start].type == token_type::identifier) {
					current_node.childs.push_back(tree_node("identifier"));
				}
				else {
					current_node.childs.push_back(tree_node("literal"));
				}
				current_node.childs.back().childs.push_back(tokens[start].data);
			}
			else if (start <= end && tokens[start].data == "{") {
				size_t pos = get_close_bracket(start + 1, end, "{", "}");

				if (pos != end) {
					add_error("can't find close bracket in array values definition ", start, end);
				}
				else {
					current_node.childs.push_back(tree_node("{"));
					current_node.childs.push_back(tree_node("array_vals"));
					array_values(current_node.childs.back(), start + 1, pos - 1);
					current_node.childs.push_back(tree_node("}"));
				}
			}
		}

	}
}

void syntax_analyzer::math_expr(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		add_error("illegal math expression", start, end);
		return;
	}
	if (start == end) {
		current_node.childs.push_back(tree_node(tokens[start].data));
		return;
	}

	int bal = 0;
	size_t sign = -1;
	for (size_t i = start; i <= end; ++i) {
		if (tokens[i].data == "(") {
			++bal;
		} 
		else if (tokens[i].data == ")") {
			--bal;
		}
		else if (tokens[i].data == "+" || tokens[i].data == "-") {
			if (bal == 0) {
				sign = i;
			}
		}
	}

	if (sign == end || sign == start) {
		add_error("illegal math expression ", start, end);
	}
	else {
		if (sign > end) {
			current_node.childs.push_back(tree_node("term"));
			term(current_node.childs.back(), start, end);
		}
		else {
			current_node.childs.push_back(tree_node("math_expr"));
			math_expr(current_node.childs.back(), start, sign - 1);
			current_node.childs.push_back(tree_node(tokens[sign].data));
			current_node.childs.push_back(tree_node("term"));
			term(current_node.childs.back(), sign + 1, end);
		}
	}
}

void syntax_analyzer::logic_expr(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		add_error("has no logic expression ", start, end);
		return;
	}
	

	if (end - start > 2) {
		add_error("illegal logic expression ", start, end);
		return;
	}

	if (start + 1 <= end
		&& std::regex_match(tokens[start + 1].data, r_logic_op)) {
		
		current_node.childs.push_back(tree_node("a"));
		a(current_node.childs.back(), start, start);
		current_node.childs.push_back(tree_node(tokens[start + 1].data));
		
		if (start + 2 <= end) {
			current_node.childs.push_back(tree_node("b"));
			a(current_node.childs.back(), start + 2, start + 2);
		}
		else {
			add_error("illegar logic expression ", start, end);
		}

	}
	else {
		add_error("illegar logic expression ", start, end);
	}
}

void syntax_analyzer::term(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		add_error("illegal math expression", start, end);
		return;
	}

	if (start == end) {
		current_node.childs.push_back(tree_node(tokens[start].data));
		return;
	}

	int bal = 0;
	size_t sign = -1;
	for (size_t i = start; i <= end; ++i) {
		if (tokens[i].data == "(") {
			++bal;
		}
		else if (tokens[i].data == ")") {
			--bal;
		}
		else if (tokens[i].data == "%" || tokens[i].data == "/" || tokens[i].data == "*") {
			if (bal == 0) {
				sign = i;
			}
		}
	}

	if (sign == end || sign == start) {
		add_error("illegal math expression ", start, end);
	}
	else {
		if (sign > end) {
			current_node.childs.push_back(tree_node("factor"));
			factor(current_node.childs.back(), start, end);
		}
		else {
			current_node.childs.push_back(tree_node("term"));
			term(current_node.childs.back(), start, sign - 1);
			current_node.childs.push_back(tree_node(tokens[sign].data));
			current_node.childs.push_back(tree_node("factor"));
			factor(current_node.childs.back(), sign + 1, end);
		}
	}
}

void syntax_analyzer::factor(tree_node& current_node, size_t start, size_t end)
{
	if (tokens[start].type == token_type::literal && start == end) {
		current_node.childs.push_back(tree_node("literal"));
		current_node.childs.back().childs.push_back(tree_node(tokens[start].data));
	}
	else if (tokens[start].data == "(") {
		size_t pos = get_close_bracket(start + 1, end, "(", ")");

		if (pos != end) {
			add_error("illegal brackets in math expression ", start, end);
		}
		else {
			current_node.childs.push_back(tree_node("math_expr"));
			math_expr(current_node.childs.back(), start + 1, pos - 1);
		}
	}
	else {
		add_error("illegal math expression ", start, end);
	}
}

void syntax_analyzer::a(tree_node& current_node, size_t start, size_t end)
{
	if (tokens[start].type == token_type::identifier) {
		current_node.childs.push_back(tree_node("identifier"));
		current_node.childs.back().childs.push_back(tree_node(tokens[start].data));
	}
	else if (tokens[start].type == token_type::literal) {
		current_node.childs.push_back(tree_node("literal"));
		current_node.childs.back().childs.push_back(tree_node(tokens[start].data));
	}
	else {
		add_error("illegal logic expression operand ", start, end);
	}
}

void syntax_analyzer::array_values(tree_node& current_node, size_t start, size_t end)
{
	if (start > end) {
		return;
	}

	if (tokens[start].data == ",") {
		array_values(current_node, start + 1, end);
		return;
	}

	if (tokens[start].type == token_type::identifier
		|| tokens[start].type == token_type::literal) {
		if (tokens[start].type == token_type::identifier) {
			current_node.childs.push_back(tree_node("identifier"));
		}
		else {
			current_node.childs.push_back(tree_node("literal"));
		}
		current_node.childs.back().childs.push_back(tree_node(tokens[start].data));

		if (start + 1 <= end && tokens[start + 1].data == ",") {
			current_node.childs.push_back(tree_node(","));
			array_values(current_node, start + 2, end);
		}
	}
	else {
		add_error("illegal array value ", start, end);
	}
}

void syntax_analyzer::rec_print(
	std::ostream& out,
	tree_node& current,
	std::vector<bool>& levels,
	int32_t depth,
	uint32_t l,
	char spacer,
	char down,
	char cross
)
{

	for (int32_t i = 0; i < depth; ++i) {
		if (i != depth - 1) {
			if (levels[i]) {
				out << down;
			}
			else {
				out << " ";
			}
			for (uint32_t x = 0; x < l; ++x) {
				out << " ";
			}
		}
		else {
			out << cross;
		}
	}

	if (depth) {
		for (uint32_t i = 0; i < l; ++i) {
			out << spacer;
		}
	}
	
	out << current.t << std::endl;

	levels.push_back(true);
	auto it = current.childs.begin();
	while (it != current.childs.end()) {
		if (it == --(current.childs.end())) {
			levels[levels.size() - 1] = false;
		}
		rec_print(out, *it, levels, depth + 1, l, spacer, down, cross);
		++it;
	}
	levels.erase(--levels.end());
}


syntax_analyzer::syntax_analyzer(std::vector<token>& tokens):
	tokens(tokens)
{
}

void syntax_analyzer::analyze()
{
	errors.clear();

	root.childs.clear();

	statement(root, 0, tokens.size() - 1);

	has_error = !errors.empty();
}

tree_node& syntax_analyzer::get_root()
{
	return this->root;
}

bool syntax_analyzer::has_errors()
{
	return has_error;
}

std::vector<std::string> syntax_analyzer::get_errors()
{
	return errors;
}

void syntax_analyzer::print(std::ostream& out)
{
	std::vector<bool> levels;
	rec_print(out, root, levels, 0, 3, '-', '|', '+');
}
