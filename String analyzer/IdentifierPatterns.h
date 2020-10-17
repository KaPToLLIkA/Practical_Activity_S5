#pragma once
#include "PatternBase.h"
class IdentifierPatterns :
	public PatternBase
{
private:
	static IdentifierPatterns* instance;

	IdentifierPatterns() {
		this->patterns = {
			std::regex("^(\\s)*[_,a-z,A-Z][_,A-Z,a-z,0-9]*[_,A-Z,a-z,0-9]"),
		};
	}

	IdentifierPatterns(const IdentifierPatterns&) = delete;
	IdentifierPatterns& operator=(IdentifierPatterns&) = delete;
public:
	static IdentifierPatterns* getInst() {
		if (instance == nullptr) {
			instance = new IdentifierPatterns();
		}
		return instance;
	}
};

