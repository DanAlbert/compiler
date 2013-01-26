/**
 * Defines interfaces for logging messages at the appropriate error levels.
 *
 * Author: Dan Albert <dan@gingerhq.net>
 */
#ifndef MESSAGES_H
#define MESSAGES_H

#include <assert.h>
#include <stdio.h>

#define LOGLEVEL_CRITICAL	1
#define LOGLEVEL_ERROR		2
#define LOGLEVEL_WARNING	3
#define LOGLEVEL_INFO		4
#define LOGLEVEL_DEBUG		5

#define LOGLEVEL_MIN LOGLEVEL_CRITICAL
#define LOGLEVEL_MAX LOGLEVEL_DEBUG

int get_log_level(void);
void set_log_level(int level);
void message(int level, const char* format, ...);
void error_message(int level,
                   const char* file,
                   int line,
                   const char* format, ...);

#define CRITICAL(format, ...) \
	error_message(LOGLEVEL_CRITICAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define ERROR(format, ...) \
	error_message(LOGLEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define WARNING(format, ...) \
	error_message(LOGLEVEL_WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define INFO(format, ...) message(LOGLEVEL_INFO, format, ##__VA_ARGS__)
#define DEBUG(format, ...) message(LOGLEVEL_DEBUG, format, ##__VA_ARGS__)

#endif // MESSAGES_H

