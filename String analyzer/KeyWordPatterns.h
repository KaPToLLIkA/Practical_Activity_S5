#pragma once
#include "PatternBase.h"
class KeyWordPatterns :
	public PatternBase
{
private:
	static KeyWordPatterns* instance;

	KeyWordPatterns() {
		this->patterns = {
			std::regex("\\s*while"),
			std::regex("\\s*do"),
			std::regex("\\s*if"),
			std::regex("\\s*else"),
			std::regex("\\s*return"),
		};
	}

	KeyWordPatterns(const KeyWordPatterns&) = delete;
	KeyWordPatterns& operator=(KeyWordPatterns&) = delete;

public:
	static KeyWordPatterns* getInst() {
		if (instance == nullptr) {
			instance = new KeyWordPatterns();
		}
		return instance;
	}
};

