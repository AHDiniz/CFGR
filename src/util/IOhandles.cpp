#include "pch.h"

#include "util/IOhandles.hpp"
#include "util/log.hpp"

namespace CFGR
{
	Grammar Read()
	{
		LOG_INFO("%s\n", "Reading the grammar from standard input.");

		char sentenceElem;
		int vCount, tCount, rCount;
		std::vector<char> variables;
		std::vector<char> terminals;

		std::cin >> vCount >> tCount >> rCount >> sentenceElem;

		for (int i = 0; i < vCount; ++i)
		{
			char variable;
			variable = getc(stdin);
			if (variable >= 'A' && variable <= 'Z')
				variables.push_back(variable);
		}

		for (int i = 0; i < tCount; ++i)
		{
			char terminal;
			terminal = getc(stdin);
			if (terminal >= 'A' && terminal <= 'Z')
				terminals.push_back(terminal);
		}

		Grammar g(sentenceElem, variables, terminals);

		for (int i = 0; i < rCount; ++i)
		{
			char start;
			char end[32];
			int endSize = 0;
			scanf("%c -> ", &start);

			char c;
			do
			{
				c = getc(stdin);
				if (c != '\n') end[endSize++] = c;
			} while (c != '\n');

			if (start < 'A' || start > 'Z')
			{
				LOG_ERROR("Rule #%d is invalid because it uses the invalid variable character %c in the rule start. This rule is being ignored.\n", i + 1, start);
				continue;
			}

			for (int j = 0; j < endSize; ++j)
			{
				if ((end[j] != '0') && ((end[j] < 'A') || (end[j] > 'Z' && end[j] < 'a') || (end[j] > 'z')))
				{
					LOG_ERROR("Rule #%d is invalid because it uses the invalid variable character %c in the rule end. This rule is being ignored.\n", i + 1, end[j]);
					continue;
				}
			}

			Rule rule;

			rule.start = start;
			for (int i = 0; i < endSize; ++i)
				rule.end.push_back(end[i]);
			
			g.AddRule(rule);
		}

		return g;
	}

	Grammar Read(const char *path)
	{
		FILE *f = fopen(path, "r");

		if (!f)
		{
			LOG_FATAL("Could not open the following file: %s", path);
			exit(EXIT_FAILURE);
		}

		LOG_INFO("Reading the grammar from the file %s.\n", path);

		char sentenceElem;
		int vCount, tCount, rCount;
		std::vector<char> variables;
		std::vector<char> terminals;

		std::cin >> vCount >> tCount >> rCount >> sentenceElem;

		for (int i = 0; i < vCount; ++i)
		{
			char variable;
			fscanf(f, "%c\n", &variable);
			variables.push_back(variable);
		}

		for (int i = 0; i < tCount; ++i)
		{
			char terminal;
			fscanf(f, "%c\n", &terminal);
			terminals.push_back(terminal);
		}

		Grammar g(sentenceElem, variables, terminals);

		for (int i = 0; i < rCount; ++i)
		{
			char start;
			char end[32];
			int endSize = 0;
			fscanf(f, "%c -> ", &start);

			char c;
			do
			{
				c = fgetc(f);
				end[endSize++] = c;
			} while (c != '\n');

			if (start < 'A' || start > 'Z')
			{
				LOG_ERROR("Rule #%d is invalid because it uses the invalid variable character %c in the rule start. This rule is being ignored.", i + 1, start);
				continue;
			}

			for (int j = 0; j < endSize; ++j)
			{
				if ((end[j] < 'A') || (end[j] > 'Z' && end[j] < 'a') || (end[j] > 'z'))
				{
					LOG_ERROR("Rule #%d is invalid because it uses the invalid variable character %c in the rule end. This rule is being ignored.", i + 1, end[j]);
					continue;
				}
			}

			Rule rule;

			rule.start = start;
			for (int i = 0; i < endSize; ++i)
				rule.end.push_back(end[i]);
			
			g.AddRule(rule);
		}

		return g;
	}

	void Print(const Grammar g)
	{
		LOG_INFO("%s\n", "Printing the grammar in the standard output.");

		std::cout << "Variables = { ";
		for (char c : g.GetVariables())
			std::cout << "(" << c << "), ";
		std::cout << "}" << std::endl;

		std::cout << "Terminals = { ";
		for (char t : g.GetTerminals())
			std::cout << "(" << t << "), ";
		std::cout << "}" << std::endl;

		std::cout << "Rules = {" << std::endl;
		for (const Rule r : g.GetRules())
		{
			std::cout << "(" << r.start << " -> ";
			for (char e : r.end)
				std::cout << e;
			std::cout << ")," << std::endl;
		}
		std::cout << "}" << std::endl;
	}

	void Print(const Grammar g, const char *path)
	{
		FILE *f = fopen(path, "w");

		if (!f)
		{
			LOG_FATAL("Could not open the following file: %s\n", path);
			exit(EXIT_FAILURE);
		}

		LOG_INFO("Printing the grammar to the file %s.\n", path);

		fprintf(f, "Variables = { ");
		for (char c : g.GetVariables())
			fprintf(f, "(%c), ", c);
		fprintf(f, "}\n");

		fprintf(f, "Terminals = { ");
		for (char t : g.GetTerminals())
			fprintf(f, "(%c), ", t);
		fprintf(f, "}\n");

		fprintf(f, "Rules = {\n");
		for (const Rule r : g.GetRules())
		{
			fprintf(f, "(%c -> ", r.start);
			for (char e : r.end)
				fprintf(f, "%c", e);
			fprintf(f, "),\n");
		}
		fprintf(f, "}\n");
	}
}
