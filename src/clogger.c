/**
 * @file clogger.c
 * @author Philip R. Simonson
 * @date 13 Mar 2019
 * @brief Simple logging system that I made.
 * @details
 *
 * Just a logging system, very simple and straight
 * forward to use.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "file.h"
#include "clogger.h"

/**
 * @brief Structure used for handling log files.
 */
struct CLOG {
	file_t *file;    /**< File structure for log file storage. */
	int status;      /**< Current status of log file number. */
	long read_pos;   /**< Current read position */
	long write_pos;  /**< Current write position */
};

struct CLOG _logs[MAX_LOGS]; /**< Global variable for storing log info */
char init_var;               /**< Global variable for logger initialization */

void close_log(int);
int get_status(int);

static void
_logger_exit_func ()
{
	int i;

	for(i=0; i<MAX_LOGS; i++)
		if (get_status(i))
			close_log(i);
}

/**
 * @brief Initialize logger system.
 *
 * Returns: void
 */
void
init_logger ()
{
	if(init_var == 0) {
		int i;

		init_var = 1;
		for(i=0; i<MAX_LOGS; i++) {
			memset(&_logs[i], 0, sizeof(struct CLOG));
		}
		atexit(_logger_exit_func);
	} else {
		printf("C_Logger is already initialized!\n");
	}
}

/**
 * @brief Opens a log file of name.
 *
 * Returns: void
 */
void
open_log (int logNum, const char *name)
{
	if(init_var) {
		if(!get_status(logNum)) {
			_logs[logNum].file = open_file(name, "a+t");
			if(get_errori_file(_logs[logNum].file)
				!= FILE_ERROR_OKAY)
				return;
			_logs[logNum].status = 1;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}

/**
 * @brief Reads a log file into buf of size.
 *
 * Returns: int
 */
int
read_log (int logNum, char *buf, int size)
{
	if(init_var) {
		if(get_status(logNum)) {
			int c, pos;

			seek_file(_logs[logNum].file,
				_logs[logNum].read_pos, SEEK_SET);
			if(get_errori_file(_logs[logNum].file)
				!= FILE_ERROR_OKAY) {
				printf("Error: %s\n",
					get_error_file(_logs[logNum].file));
				return -1;
			}
			c = pos = 0;
			memset(buf, 0, size);
			while(pos < size &&
				((c = getc_file(_logs[logNum].file))
				!= '\n'))
				buf[pos++] = c;
			_logs[logNum].read_pos =
				tell_file(_logs[logNum].file);
			buf[pos] = '\0';
			return c;
		}
		printf("Warning: Could not read, log CLOG%d not open.\n",
			logNum);
		return -1;
	}
	printf("Please use init_logger() first.\n");
	return -1;
}

/**
 * @brief Formatted output (writing to log file).
 *
 * Returns: void
 */
void
write_log (int logNum, const char *data, ...)
{
	if(init_var) {
		if(get_status(logNum)) {
			va_list ap;
			seek_file(_logs[logNum].file,
				_logs[logNum].write_pos,
				SEEK_SET);
			va_start(ap, data);
			vwritef_file(_logs[logNum].file, data, ap);
			va_end(ap);
			_logs[logNum].write_pos =
				tell_file(_logs[logNum].file);
			flush_file(_logs[logNum].file);
			return;
		}
		printf("Warning: Not writing, log CLOG%d not open.\n", logNum);
		return;
	}
	printf("Please use init_logger() first.\n");
}

/**
 * @brief Close a log file.
 *
 * Returns: void
 */
void
close_log (int logNum)
{
	if(init_var) {
		if(get_status(logNum)) {
			close_file(_logs[logNum].file);
			_logs[logNum].status = 0;
			_logs[logNum].read_pos = 0;
			_logs[logNum].write_pos = 0;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}

/**
 * @brief Print status of log file.
 *
 * Returns: void
 */
void
print_status (int logNum)
{
	if(init_var) {
		printf("CLOG%d: Log %s\n", logNum,
			(get_status(logNum) == 1) ? "open" : "closed");
		return;
	}
	printf("Please use init_logger() first.\n");
}

/**
 * @brief Gets the status number from log file.
 *
 * Returns: int (status)
 */
int
get_status (int logNum)
{
	if(init_var)
		return _logs[logNum].status;
	printf("Please use init_logger() first.\n");
	return 0;
}

/**
 * @brief Gets the file name from given log file.
 *
 * Returns: const char*
 */
const char*
get_log_name (int logNum)
{
	if(init_var)
		return get_name_file(_logs[logNum].file);
	printf("Please use init_logger() first.\n");
	return 0;
}
