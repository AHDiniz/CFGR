#include <algorithm>

#include "grammar.hpp"

#include "util/log.hpp"
#include "util/vector_utils.hpp"

static std::vector<char> NullVariables(CFGR::Grammar g);
static std::vector<CFGR::Rule> ChainRules(CFGR::Grammar g);
static std::vector<char> GeneratorVariables(CFGR::Grammar g);
static std::vector<char> Reachable(CFGR::Grammar g);

namespace CFGR
{
	Grammar::Grammar(char sentenceElem, const std::vector<char> variables, const std::vector<char> terminals)
	{
		this->sentenceElem = sentenceElem;
		this->variables = variables;
		this->terminals = terminals;
	}

	void Grammar::AddRule(const Rule rule)
	{
		rules.push_back(rule);
	}

	void Grammar::Refine(void)
	{
		std::vector<char> nullable = NullVariables(*this);
		for (int i = 0; i < rules.size(); ++i)
		{
			Rule rule = rules[i];
			if (rule.end[0] == '0')
			{
				REMOVE(rules, rule);
				--i;
			}
			if (rule.start == sentenceElem)
			{
				Rule nRule;
				nRule.start = '$';
				nRule.end = rule.end;
				rules.push_back(nRule);
			}
		}

		if (CONTAINS(nullable, sentenceElem))
		{
			Rule nRule1;
			nRule1.start = '$';
			nRule1.end = { sentenceElem };

			Rule nRule2;
			nRule2.start = '$';
			nRule2.end = { '0' };

			rules.push_back(nRule1);
			rules.push_back(nRule2);
		}

		std::vector<Rule> chainRules = ChainRules(*this);
		for (Rule chainRule : chainRules)
			REMOVE(rules, chainRule);

		for (Rule chainRule : chainRules)
		{
			for (Rule rule : rules)
			{
				if (rule.start == chainRule.end[0])
				{
					Rule nRule;
					nRule.start = chainRule.start;
					nRule.end = rule.end;
					rules.push_back(nRule);
				}
			}
		}

		std::vector<char> generators = GeneratorVariables(*this);
		for (int i = 0; i < rules.size(); ++i)
		{
			Rule rule = rules[i];
			for (char e : rule.end)
			{
				if ((e >= 'A' && e <= 'Z') && !CONTAINS(generators, e))
				{
					REMOVE(rules, rule);
					--i;
					break;
				}
			}
		}

		std::vector<char> reachable = Reachable(*this);
		for (int i = 0; i < rules.size(); ++i)
		{
			Rule rule = rules[i];
			if (!CONTAINS(reachable, rule.start))
			{
				REMOVE(rules, rule);
				--i;
				continue;
			}

			for (char e : rule.end)
			{
				if (!CONTAINS(reachable, e))
				{
					REMOVE(rules, rule);
					--i;
					if (CONTAINS(variables, e)) REMOVE(variables, e);
					else if (CONTAINS(terminals, e)) REMOVE(terminals, e);
				}
			}
		}
	}

	const std::vector<char> Grammar::GetVariables() const
	{
		return variables;
	}

	const std::vector<char> Grammar::GetTerminals() const
	{
		return terminals;
	}

	const std::vector<Rule> Grammar::GetRules() const
	{
		return rules;
	}

	bool operator==(Rule r1, Rule r2)
	{
		bool result = true;
		if (r1.start != r2.start) result = false;
		else
		{
			int s = (r1.end.size() < r2.end.size()) ? r1.end.size() : r2.end.size();
			for (int i = 0; i < s; ++i)
			{
				if (r1.end[i] != r2.end[i])
				{
					result = false;
					break;
				}
			}
		}
		return result;
	}
}

static std::vector<char> NullVariables(CFGR::Grammar g)
{
	std::vector<char> previous;
	std::vector<char> nullable;

	for (CFGR::Rule rule : g.GetRules())
	{
		if (rule.end[0] == '0')
			nullable.push_back(rule.start);
	}

	while (previous.size() != nullable.size())
	{
		for (CFGR::Rule rule : g.GetRules())
		{
			for (char e : rule.end)
			{
				if (CONTAINS(nullable, e) && !CONTAINS(nullable, rule.start))
				{
					nullable.push_back(rule.start);
					break;
				}
			}
		}

		previous = nullable;
	}

	return nullable;
}

static std::vector<CFGR::Rule> ChainRules(CFGR::Grammar g)
{
	std::vector<CFGR::Rule> chainRules;

	for (CFGR::Rule rule : g.GetRules())
	{
		if (rule.end.size() == 1 && CONTAINS(g.GetVariables(), rule.end[0]))
		{
			chainRules.push_back(rule);
		}
	}

	return chainRules;
}

static std::vector<char> GeneratorVariables(CFGR::Grammar g)
{
	std::vector<char> previous;
	std::vector<char> generators;

	for (CFGR::Rule rule : g.GetRules())
	{
		if (rule.end.size() == 1 && CONTAINS(g.GetTerminals(), rule.end[0]))
		{
			generators.push_back(rule.start);
		}
	}

	while (previous.size() != generators.size())
	{
		for (CFGR::Rule rule : g.GetRules())
		{
			for (char e : rule.end)
			{
				if (CONTAINS(generators, e) && !CONTAINS(generators, rule.start))
				{
					generators.push_back(rule.start);
					break;
				}
			}
		}

		previous = generators;
	}

	return generators;
}

static std::vector<char> Reachable(CFGR::Grammar g)
{
	std::vector<char> reachable;
	reachable.push_back(g.GetSentenceElem());

	for (CFGR::Rule rule : g.GetRules())
	{
		if (CONTAINS(reachable, rule.start))
		{
			for (char e : rule.end)
				if (!CONTAINS(reachable, e))
					reachable.push_back(e);
		}
	}

	return reachable;
}
