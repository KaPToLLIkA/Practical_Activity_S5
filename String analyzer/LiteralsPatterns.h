#pragma once
#include "PatternBase.h"
class LiteralsPatterns :
	public PatternBase
{
private:
	static LiteralsPatterns* instance;

	LiteralsPatterns() {
		this->patterns = {
			std::regex("^\s*\d*\s*"), //integer
			std::regex("(^\s*\"(\S*|\s*)*\"\s*)|(^\s*\'(\S*|\s*)*\'\s*)"), //string
		};
	}

	LiteralsPatterns(const LiteralsPatterns&) = delete;
	LiteralsPatterns& operator=(LiteralsPatterns&) = delete;
public:
	static LiteralsPatterns* getInst() {
		if (instance == nullptr) {
			instance = new LiteralsPatterns();
		}
		return instance;
	}
};

