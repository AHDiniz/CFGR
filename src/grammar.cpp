#include <algorithm>

#include "grammar.hpp"

#include "util/log.hpp"
#include "util/vector_utils.hpp"

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
		std::vector<char> nullable;
		/* Determining if the null word (0 in this case) belongs to the generated language */ {

			LOG_INFO("%s\n", "Determining if the null word (0 in this case) belongs to the generated language.");

			std::vector<char> previous;

			for (Rule rule : rules)
				if (CONTAINS(rule.end, '0'))
					nullable.push_back(rule.start);

			while (previous.size() != nullable.size())
			{
				for (Rule rule : rules)
				{
					for (char i = 'a'; i <= 'z'; ++i)
						if (CONTAINS(rule.end, i))
							continue;

					bool canPush = true;
					for (char n : nullable)
						if (!CONTAINS(rule.end, n))
							canPush = false;
					if (canPush) nullable.push_back(rule.start);
				}

				previous = nullable;
			}
		}

		/* Removing the sentence element recursion if it's nullable */
		if (CONTAINS(nullable, sentenceElem))
		{
			LOG_INFO("%s\n", "Removing the sentence element recursion.");
			char nSentenceElem = '$';

			Rule nRule1 = {nSentenceElem, {sentenceElem}};
			Rule nRule2 = {nSentenceElem, {'0'}};

			rules.push_back(nRule1);
			rules.push_back(nRule2);

			sentenceElem = nSentenceElem;
			variables.push_back(nSentenceElem);
		}

		/* Removing the null word rules from the grammar while mantaining the resultant language */ {

			LOG_INFO("%s\n", "Removing the null word rules from the grammar while mantaining the resultant language.");

			std::vector<Rule> nullRules;
			for (Rule rule : rules)
			{
				if (rule.start != '$' && rule.end[0] == '0')
				{
					nullRules.push_back(rule);
					REMOVE(rules, rule);
				}
			}

			for (Rule rule : rules)
			{
				if (rule.start == '$' && rule.end[0] == '0') continue;

				for (Rule nullRule : nullRules)
				{
					if (CONTAINS(rule.end, nullRule.start))
					{
						Rule nRule;
						nRule.start = rule.start;
						for (char v : rule.end)
							if (v != nullRule.start)
								nRule.end.push_back(v);
						rules.push_back(nRule);
					}
				}
			}
		}

		/* Removing chain rules while mantaining the resultant language */ {

			LOG_INFO("%s\n", "Removing chain rules while mantaining the resultant language.");

			std::vector<Rule> chainRules;
			for (Rule rule : rules)
			{
				if (rule.end.size() == 1 && CONTAINS(variables, rule.end[0]))
				{
					chainRules.push_back(rule);
					REMOVE(rules, rule);
				}
			}

			std::vector<Rule> nRules;
			for (Rule chainRule : chainRules)
			{
				for (Rule rule : rules)
				{
					if (rule.start == chainRule.end[0])
					{
						Rule nRule;
						nRule.start = chainRule.start;
						nRule.end = rule.end;
						nRules.push_back(nRule);
					}
				}
			}

			for (Rule nRule : nRules)
				rules.push_back(nRule);
		}

		/* Removing the variables that don't generate terminal strings */ {

			LOG_INFO("%s\n", "Removing the variables that don't generate terminal strings.");

			std::vector<char> previous;
			std::vector<char> generators;
			generators.push_back(sentenceElem);

			while (previous.size() != generators.size())
			{
				for (Rule rule : rules)
				{
					if (CONTAINS(generators, rule.start))
					{
						for (char v : rule.end)
						{
							if (v >= 'a' && v <= 'z')
								generators.push_back(v);
						}
					}
				}

				previous = generators;
			}

			for (Rule rule : rules)
			{
				if (!CONTAINS(generators, rule.start))
					REMOVE(rules, rule);
			}
		}

		/* Removing the unreachable variables through the sentence element */ {

			LOG_INFO("%s\n", "Removing the unreachable variables through the sentencial element.");

			std::vector<char> previous;
			std::vector<char> reachable;
			reachable.push_back(sentenceElem);

			while (previous.size() != reachable.size())
			{
				for (Rule rule : rules)
				{
					if (CONTAINS(reachable, rule.start))
					{
						for (char v : rule.end)
						{
							if (v >= 'A' && v <= 'Z')
								reachable.push_back(v);
						}
					}
				}

				previous = reachable;
			}

			for (Rule rule : rules)
			{
				if (!CONTAINS(reachable, rule.start))
					REMOVE(rules, rule);
			}
		}
	}
}
