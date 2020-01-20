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
#include <errno.h>

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

struct CLOG _logs[MAX_LOGS];  /**< Global variable for storing log info */
char init_var;                /**< Global variable for logger initialization */

#ifdef __cplusplus
extern "C" {
#endif

/* Close a log file.
 */
PRS_EXPORT void close_log(int);

/* Exit function, clean up log files.
 */
static void _logger_exit_func(void)
{
	int i;

	for(i=0; i<MAX_LOGS; i++)
		if (get_status_log(i))
			close_log(i);
}
/* Initialize logger system.
 */
PRS_EXPORT void init_logger(void)
{
	if(init_var == 0) {
		int i;

		init_var = 1;
		for(i=0; i<MAX_LOGS; i++) {
			_logs[i].file = NULL;
			_logs[i].status = CLOGERR_CLOSE;
			_logs[i].write_pos = 0;
			_logs[i].read_pos = 0;
		}
		atexit(_logger_exit_func);
	} else {
		printf("C_Logger is already initialized!\n");
	}
}
/* Opens a log file of name.
 */
PRS_EXPORT void open_log(int logNum, const char *name)
{
	if(init_var) {
		if(get_status_log(logNum) == CLOGERR_CLOSE) {
			_logs[logNum].file = open_file(name, "a+t");
			if(get_error_file() != FILE_ERROR_OKAY) {
				_logs[logNum].status = CLOGERR_OPEN;
				return;
			}
			_logs[logNum].status = CLOGERR_OKAY;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}
/* Reads a log file into buf of size.
 */
PRS_EXPORT int read_log(int logNum, char *buf, int size)
{
	if(init_var) {
		if(get_status_log(logNum) == CLOGERR_OKAY) {
			int c, pos, err;
			seek_file(_logs[logNum].file,
				_logs[logNum].read_pos, SEEK_SET);
			if((err = get_error_file()) != FILE_ERROR_OKAY) {
				printf("Error: %s\n", strerror_file(err));
				return -1;
			}
			c = pos = 0;
			memset(buf, 0, size);
			while(pos < size &&
				(c = getc_file(_logs[logNum].file)
				!= EOF && c != '\n'))
				buf[pos++] = c;
			_logs[logNum].read_pos =
				tell_file(_logs[logNum].file);
			buf[pos] = '\0';
			if(c != EOF && errno != 0) {
				_logs[logNum].status = CLOGERR_READ;
				errno = 0;
			}
			return c;
		}
		printf("Warning: Could not read, log CLOG%d not open.\n",
			logNum);
		return -1;
	}
	printf("Please use init_logger() first.\n");
	return -1;
}
/* Formatted output (writing to log file).
 */
PRS_EXPORT void write_log(int logNum, const char *data, ...)
{
	if(init_var) {
		if(get_status_log(logNum) == CLOGERR_OKAY) {
			va_list ap;
			int res;
			seek_file(_logs[logNum].file,
				_logs[logNum].write_pos,
				SEEK_SET);
			va_start(ap, data);
			res = vwritef_file(_logs[logNum].file, data, ap);
			va_end(ap);
			_logs[logNum].write_pos =
				tell_file(_logs[logNum].file);
			flush_file(_logs[logNum].file);
			if(res < 0 && errno != 0) {
				_logs[logNum].status = CLOGERR_WRITE;
				errno = 0;
			}
			return;
		}
		printf("Warning: Not writing, log CLOG%d not open.\n", logNum);
		return;
	}
	printf("Please use init_logger() first.\n");
}
/* Close a log file.
 */
PRS_EXPORT void close_log(int logNum)
{
	if(init_var) {
		if(get_status_log(logNum) == CLOGERR_OKAY) {
			close_file(_logs[logNum].file);
			_logs[logNum].status = CLOGERR_CLOSE;
			_logs[logNum].read_pos = 0;
			_logs[logNum].write_pos = 0;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}
/* Print status of log file.
 */
PRS_EXPORT void print_status_log(int logNum)
{
	if(init_var) {
		const char *_err_codes[] = {
			"OKAY", "OPEN ERROR",
			"CLOSED", "WRITE ERROR",
			"READ ERROR"
		};
		printf("CLOG%d: Log status\t\t- [%s]\n", logNum,
			_err_codes[get_status_log(logNum)]);
		return;
	}
	printf("Please use init_logger() first.\n");
}
/* Gets the status number from log file.
 */
PRS_EXPORT int get_status_log(int logNum)
{
	if(init_var)
		return _logs[logNum].status;
	printf("Please use init_logger() first.\n");
	return 0;
}
/* Gets the file name from given log file.
 */
PRS_EXPORT const char* get_name_log(int logNum)
{
	if(init_var)
		return get_name_file(_logs[logNum].file);
	printf("Please use init_logger() first.\n");
	return 0;
}
#ifdef __cplusplus
}
#endif
