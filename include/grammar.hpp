#pragma once

#include "pch.h"

namespace CFGR
{
	struct Rule
	{
		char start;
		std::vector<char> end;
	};

	class Grammar
	{
	public:
		Grammar(char sentenceElem, const std::vector<char> variables, const std::vector<char> terminals);
		void AddRule(const Rule rule);
		void Refine(void);

	private:
		char sentenceElem;
		std::vector<char> variables;
		std::vector<char> terminals;
		std::vector<const Rule> rules;
	};
}
