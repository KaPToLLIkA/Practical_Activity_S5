#pragma once
#include "PatternBase.h"
class BinaryOperatorPatterns :
	public PatternBase
{
private:
	static BinaryOperatorPatterns* instance;

	BinaryOperatorPatterns() {
		this->patterns = {
			std::regex("^\\s*=\\s*"),
		};
	}

	BinaryOperatorPatterns(const BinaryOperatorPatterns&) = delete;
	BinaryOperatorPatterns& operator=(BinaryOperatorPatterns&) = delete;
public:
	static BinaryOperatorPatterns* getInst() {
		if (instance == nullptr) {
			instance = new BinaryOperatorPatterns();
		}
		return instance;
	}
};

