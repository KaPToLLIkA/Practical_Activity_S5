#pragma once
#include <fstream>
#include <string>

class code_loader
{
	std::string fname;

public:
	explicit code_loader(std::string);

	std::string load_code();
};

