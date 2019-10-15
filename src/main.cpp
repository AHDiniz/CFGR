#include "cfgr.hpp"

int main(int argc, char *argv[])
{
	std::cout << "CFGR - Context Free Grammar Refiner - version 0.0.1" << std::endl;
	std::cout << "by Alan Herculano Diniz - MIT License" << std::endl << std::endl;

	if (argc == 1)
	{
		CFGR::Grammar g = CFGR::Read();
		g.Refine();
		CFGR::Print(g);
	}
	else if (argc == 2 && (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "--h") == 0 || strcmp(argv[1], "-h") == 0))
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "Use the -i flag to define the input file: ./CFGR -i input.txt" << std::endl;
		std::cout << "Use the -o flag to define the output file: ./CFGR -o output.txt" << std::endl;
		std::cout << "You can use these flags together too: ./CFGR -i input.txt -o output.txt" << std::endl;
		std::cout << "If nothing is define the program will read/write from standard I/O" << std::endl;
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "-i") == 0)
		{
			CFGR::Grammar g = CFGR::Read(argv[2]);
			g.Refine();
			CFGR::Print(g);
		}
		else if (strcmp(argv[1], "-o") == 0)
		{
			CFGR::Grammar g = CFGR::Read();
			g.Refine();
			CFGR::Print(g, argv[2]);
		}
	}
	else if (argc == 5)
	{
		if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0)
		{
			CFGR::Grammar g = CFGR::Read(argv[2]);
			g.Refine();
			CFGR::Print(g, argv[4]);
		}
		else if (strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-i") == 0)
		{
			CFGR::Grammar g = CFGR::Read(argv[4]);
			g.Refine();
			CFGR::Print(g, argv[2]);
		}
	}
	else
	{
		CFGR::LOG_WARNING("%s\n", "Invalid input :(. Type ./CFGR help for usage.");
	}

	std::cout << std::endl;

	return 0;
}
