#ifndef EEPCH_H_

#define EEPCH_H_

#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdarg>
#include <iostream>

/**
 * Windows definitions
*/
#ifdef _WIN32

#include <Windows.h>
#include <io.h>

#ifdef _WIN64
#else
#endif

#endif

/**
 * Apple definitions
*/
#ifdef __APPLE__

#include "TargetConditionals.h"

#if TARGET_IPHONE_SIMULATOR
#elif TARGET_OS_IPHONE
#elif TARGET_OS_MAC
#else

#error "Unknown Apple platform"

#endif

#endif

/**
 * Linux definitions
*/
#ifdef __linux__

#include <unistd.h>

#endif

#endif