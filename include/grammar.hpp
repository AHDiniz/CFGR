#pragma once

#include "pch.h"

namespace CFGR
{
	struct Rule
	{
		char start;
		std::vector<char> end;
	};

	bool operator==(Rule r1, Rule r2);

	class Grammar
	{
	public:
		Grammar(char sentenceElem, const std::vector<char> variables, const std::vector<char> terminals);
		void AddRule(const Rule rule);
		void Refine(void);

		const std::vector<char> GetVariables() const;
		const std::vector<char> GetTerminals() const;
		const std::vector<Rule> GetRules() const;

	private:
		char sentenceElem;
		std::vector<char> variables;
		std::vector<char> terminals;
		std::vector<Rule> rules;
	};
}
