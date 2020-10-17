#pragma once
#include "PatternBase.h"

class InstructionsSeparatorPatterns :
	public PatternBase
{
private:
	static InstructionsSeparatorPatterns* instance;

	InstructionsSeparatorPatterns() {
		this->patterns = {
			std::regex("^\\s*;\\s*"),
		};
	}

	InstructionsSeparatorPatterns(const InstructionsSeparatorPatterns&) = delete;
	InstructionsSeparatorPatterns& operator=(InstructionsSeparatorPatterns&) = delete;
public:
	static InstructionsSeparatorPatterns* getInst() {
		if (instance == nullptr) {
			instance = new InstructionsSeparatorPatterns();
		}
		return instance;
	}
};
