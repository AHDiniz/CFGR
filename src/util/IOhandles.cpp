#include "pch.h"

#include "util/IOhandles.hpp"
#include "util/log.hpp"


namespace CFGR
{
	Grammar actualRead(FILE *f);

	Grammar Read()
	{
		LOG_INFO("%s\n", "Reading the grammar from standard input.");

		return actualRead(stdin);
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

		return actualRead(f);
	}

	Grammar actualRead(FILE *f)
	{
		char sentenceElem;
		int ruleCount;
		std::vector<char> variables;
		std::vector<char> terminals;

		fscanf(f, "%d %c\n", &ruleCount, &sentenceElem);

		char variable;
		do
		{
			variable = fgetc(f);
			if (variable >= 'A' && variable <= 'Z')
			{
				variables.push_back(variable);
			}
		} while (variable != '\n');

		char terminal;
		do
		{
			terminal = fgetc(f);
			if (terminal >= 'a' && terminal <= 'z')
			{
				terminals.push_back(terminal);
			}
		} while (terminal != '\n');

		Grammar g(sentenceElem, variables, terminals);

		for (int i = 0; i < ruleCount; ++i)
		{
			Rule rule;
			char buffer[40];
			fgets(buffer, 40, f);

			if (buffer[0] < 'A' || buffer[0] > 'Z')
			{
				LOG_ERROR("The rule #%d has an invalid variable (%c) and will be ignored.\n", i + 1, buffer[0]);
				continue;
			}

			rule.start = buffer[0];

			for (int j = 1; j < 40; ++j)
			{
				if (buffer[j] == ' ' || buffer[j] == '-' || buffer[j] == '>') continue;
				else if (buffer[j] == '\n') break;

				if ((buffer[j] == '0') || (buffer[j] >= 'a' && buffer[j] <= 'z') || (buffer[j] >= 'A' && buffer[j] <= 'Z'))
					rule.end.push_back(buffer[j]);
			}

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
			std::cout << "\t(" << r.start << " -> ";
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
			fprintf(f, "\t(%c -> ", r.start);
			for (char e : r.end)
				fprintf(f, "%c", e);
			fprintf(f, "),\n");
		}
		fprintf(f, "}\n");
	}
}