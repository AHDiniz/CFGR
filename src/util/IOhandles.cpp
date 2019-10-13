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

		std::cin >> vCount >> tCount >> rCount;

		for (int i = 0; i < vCount; ++i)
		{
			char variable;
			scanf("%c%*[ \t\r\n\f]", &variable);
			variables.push_back(variable);
		}

		for (int i = 0; i < tCount; ++i)
		{
			char terminal;
			scanf("%c%*[ \t\r\n\f]", &terminal);
			terminals.push_back(terminal);
		}

		Grammar g(sentenceElem, variables, terminals);

		for (int i = 0; i < rCount; ++i)
		{
			char start;
			
		}

		return g;
	}
}
