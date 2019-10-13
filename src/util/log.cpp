#include "util/log.hpp"

#include "pch.h"

enum TextColor
{
	EE_TEXT_BLUE = 1, // Info
	EE_TEXT_RED = 4, // Fatal
	EE_TEXT_LIGHTRED = 12, // Error
	EE_TEXT_YELLOW = 14, // Warning
	EE_TEXT_WHITE = 15 // Normal printing
};

static void setTextColor(FILE *stream, TextColor color);

namespace CFGR
{
	int LogInfo(const char *format, const char *file, const int line, ...)
	{
		setTextColor(stderr, EE_TEXT_BLUE);
		va_list args;
		int result;

		fprintf(stderr, "EE Info (File: %s, Line: %d): ", file, line);

		va_start(args, format);
		result = vfprintf(stderr, format, args);
		va_end(args);

		setTextColor(stderr, EE_TEXT_WHITE);
		return result;
	}

	int LogWarning(const char *format, const char *file, const int line, ...)
	{
		setTextColor(stderr, EE_TEXT_YELLOW);
		va_list args;
		int result;

		fprintf(stderr, "EE Warning (File: %s, Line: %d): ", file, line);

		va_start(args, format);
		result = vfprintf(stderr, format, args);
		va_end(args);

		setTextColor(stderr, EE_TEXT_WHITE);
		return result;
	}

	int LogError(const char *format, const char *file, const int line, ...)
	{
		setTextColor(stderr, EE_TEXT_LIGHTRED);
		va_list args;
		int result;

		fprintf(stderr, "EE Error (File: %s, Line: %d): ", file, line);

		va_start(args, format);
		result = vfprintf(stderr, format, args);
		va_end(args);

		setTextColor(stderr, EE_TEXT_WHITE);
		return result;
	}

	int LogFatal(const char *format, const char *file, const int line, ...)
	{
		setTextColor(stderr, EE_TEXT_RED);
		va_list args;
		int result;

		fprintf(stderr, "EE Fatal (File: %s, Line: %d): ", file, line);

		va_start(args, format);
		result = vfprintf(stderr, format, args);
		va_end(args);

		setTextColor(stderr, EE_TEXT_WHITE);
		return result;
	}
}

#ifdef _WIN32

	// Windows implementation of the set color function:
	static void setTextColor(FILE *stream, TextColor color)
	{
		int outfd = fileno(stream);
		HANDLE out = (HANDLE)_get_osfhandle(outfd);
		DWORD outType = GetFileType(out);
		DWORD mode;
		if (outType == FILE_TYPE_CHAR && GetConsoleMode(out, &mode))
			SetConsoleTextAttribute(out, color);
	}

#else

	// Implementing the set color function to other systems:

	// ANSI color sequences definition:
	static const char *ansiColorSequences[] =
	{
		"\x1B[0;30m",
		"\x1B[0;34m",
		"\x1B[0;32m",
		"\x1B[0;36m",
		"\x1B[0;31m",
		"\x1B[0;35m",
		"\x1B[0;33m",
		"\x1B[0;37m",
		"\x1B[1;30m",
		"\x1B[1;34m",
		"\x1B[1;32m",
		"\x1B[1;36m",
		"\x1B[1;31m",
		"\x1B[1;35m",
		"\x1B[1;33m",
		"\x1B[1;37m"
	};

	// ANSI color terms:
	static const char *ansiColorTerms[] =
	{
		"xterm",
		"rxvt",
		"vt100",
		"linux",
		"screen",
		0
	};

	/**
	 * Function that checks if the current terminal is in the list of ANSI terms
	*/
	static int isAnsiColorTerm(void)
	{
		char *term = getenv("TERM"); // Getting the current environment
		// Coparing the current term to the list of terms:
		for (const char **ansiTerm = &ansiColorTerms[0]; *ansiTerm; ++ansiTerm)
		{
			const char *t = term;
			const char *a = *ansiTerm;
			while (*a && *t)
				if (*a++ != *t++)
					return 1;
		}
		return 0;
	}

	// setTextColor implementation:
	static void setTextColor(FILE *stream, TextColor color)
	{
		int outfd = fileno(stream);
		if (isatty(outfd) && isAnsiColorTerm())
			fputs(ansiColorSequences[color], stream);
	}

#endif
