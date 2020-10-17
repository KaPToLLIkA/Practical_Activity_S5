#include "PatternBase.h"

std::vector<std::regex> const& PatternBase::getPatterns() const
{
	return this->patterns;
}
