#include "code_loader.h"

code_loader::code_loader(std::string fname):
	fname(fname)
{
}

std::string code_loader::load_code()
{
	std::ifstream in(fname);

	std::string data;

	while (!in.eof()) {
		std::string tmp;
		std::getline(in, tmp, '\n');
		data += tmp + "\n";
	}

	std::string filtered_data;

	for (char c : data) {
		if (c != '\t' && c != '\n') {
			filtered_data += c;
		}
	}

	in.close();
	return filtered_data;
}
