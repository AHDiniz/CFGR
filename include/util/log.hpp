#pragma once

#define LOG_INFO(format, ...) \
	LogInfo(format, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(format, ...) \
	LogWarning(format, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(format, ...) \
	LogError(format, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(format, ...) \
	LogFatal(format, __FILE__, __LINE__, __VA_ARGS__)

namespace CFGR
{
	int LogInfo(const char *format, const char *file, const int line, ...);
	int LogWarning(const char *format, const char *file, const int line, ...);
	int LogError(const char *format, const char *file, const int line, ...);
	int LogFatal(const char *format, const char *file, const int line, ...);
}
