/*
 * Copyright (C) 2001 Sistina Software (UK) Limited.
 *
 * This file is released under the GPL.
 */

#ifndef _LVM_LOG_H
#define _LVM_LOG_H

/*
 * printf()-style macros to use for messages:
 *
 *   log_error   - always print to stderr.
 *   log_print   - always print to stdout.  Use this instead of printf.
 *   log_verbose - print to stdout if verbose is set (-v)
 *   log_very_verbose - print to stdout if verbose is set twice (-vv)
 *   log_debug   - print to stdout if verbose is set three times (-vvv)
 *
 * In addition, messages will be logged to file or syslog if they
 * are more serious than the log level specified with the log/debug_level
 * parameter in the configuration file.  These messages get the file
 * and line number prepended.  'stack' (without arguments) can be used 
 * to log this information at debug level.
 *
 * log_sys_error and log_sys_very_verbose are for errors from system calls
 * e.g. log_sys_error("stat", filename);
 *      /dev/fd/7: stat failed: No such file or directory
 *
 */

#include <stdio.h>		/* FILE */
#include <string.h>		/* strerror() */
#include <errno.h>

#define _LOG_DEBUG 7
#define _LOG_INFO 6
#define _LOG_NOTICE 5
#define _LOG_WARN 4
#define _LOG_ERR 3
#define _LOG_FATAL 2

#define VERBOSE_BASE_LEVEL _LOG_WARN

void init_log_file(const char *log_file, int append);
void init_log_direct(const char *log_file, int append);
void init_log_while_suspended(int log_while_suspended);
void fin_log(void);
void release_log_memory(void);

void init_syslog(int facility);
void fin_syslog(void);

void init_verbose(int level);
void init_test(int level);
void init_partial(int level);
void init_pvmove(int level);
void init_debug(int level);
void init_cmd_name(int status);
void init_msg_prefix(const char *prefix);
void init_indent(int indent);
void init_ignorelockingfailure(int level);

void set_cmd_name(const char *cmd_name);

int test_mode(void);
int partial_mode(void);
int pvmove_mode(void);
int debug_level(void);
int ignorelockingfailure(void);

/* Suppress messages to stdout/stderr */
void log_suppress(int suppress);

/* Suppress messages to syslog */
void syslog_suppress(int suppress);

void print_log(int level, const char *file, int line, const char *format, ...)
    __attribute__ ((format(printf, 4, 5)));

#define plog(l, x...) print_log(l, __FILE__, __LINE__ , ## x)

#define log_debug(x...) plog(_LOG_DEBUG, x)
#define log_info(x...) plog(_LOG_INFO, x)
#define log_notice(x...) plog(_LOG_NOTICE, x)
#define log_warn(x...) plog(_LOG_WARN, x)
#define log_err(x...) plog(_LOG_ERR, x)
#define log_fatal(x...) plog(_LOG_FATAL, x)

#define stack log_debug("<backtrace>")	/* Backtrace on error */

#define log_error(args...) log_err(args)
#define log_print(args...) log_warn(args)
#define log_verbose(args...) log_notice(args)
#define log_very_verbose(args...) log_info(args)

/* Two System call equivalents */
#define log_sys_error(x, y) \
		log_err("%s: %s failed: %s", y, x, strerror(errno))
#define log_sys_very_verbose(x, y) \
		log_info("%s: %s failed: %s", y, x, strerror(errno))
#define log_sys_debug(x, y) \
		log_debug("%s: %s failed: %s", y, x, strerror(errno))

#endif
