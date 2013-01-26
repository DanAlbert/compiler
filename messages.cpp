#include "messages.h"

#include <stdarg.h>

int log_level = 3;

int get_log_level(void)
{
	return log_level;
}

void set_log_level(int level)
{
	assert(level >= LOGLEVEL_CRITICAL);
	assert(level <= LOGLEVEL_DEBUG);

	log_level = level;
}

void message(int level, const char* format, ...)
{
	assert(format);
	assert(level >= LOGLEVEL_CRITICAL);
	assert(level <= LOGLEVEL_DEBUG);

	if (level > log_level)
		return;

	va_list args;
	va_start(args, format);
	
	vfprintf(stderr, format, args);
	putc('\n', stderr);

	va_end(args);
}

void error_message(int level,
                   const char* file,
                   int line,
                   const char* format, ...)
{
	assert(format);
	assert(level >= LOGLEVEL_CRITICAL);
	assert(level <= LOGLEVEL_DEBUG);

	if (level > log_level)
		return;

	va_list args;
	va_start(args, format);
	
	fprintf(stderr, "%s:%d:", file, line);
	vfprintf(stderr, format, args);
	putc('\n', stderr);

	va_end(args);
}

