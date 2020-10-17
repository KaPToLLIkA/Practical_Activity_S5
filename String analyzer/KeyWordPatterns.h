#pragma once
#include "PatternBase.h"
class KeyWordPatterns :
	public PatternBase
{
private:
	static KeyWordPatterns* instance;

	KeyWordPatterns() {
		this->patterns = {
			std::regex(""),
			std::regex(""),
			std::regex(""),
			std::regex(""),
			std::regex(""),
		};
	}

public:
	static KeyWordPatterns* getInst() {
		if (instance == nullptr) {
			instance = new KeyWordPatterns();
		}
		return instance;
	}
};

