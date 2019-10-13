#pragma once

#include "grammar.hpp"

namespace CFGR
{
	Grammar Read();
	Grammar Read(const char *path);
	void Print(const Grammar g);
	void Print(const Grammar g, const char *path);
}
