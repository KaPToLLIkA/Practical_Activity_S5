#pragma once
#include <regex>

class PatternBase
{
protected:
	std::vector<std::regex> patterns;

public:
	std::vector<std::regex> const& getPatterns() const;
};

