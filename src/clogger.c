/*
 * c_logger.h - My C logger header library.
 *
 * Author : Philip R. Simonson
 * Date   : 03/13/2019
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "file.h"
#include "clogger.h"

struct CLOG {
	file_t file;
	int status;
	long read_pos;
	long write_pos;
};

struct CLOG _logs[MAX_LOGS];
char init_var;

void close_log(int);
int get_status(int);

void
_logger_exit_func ()
{
	int i;

	for(i=0; i<MAX_LOGS; i++)
		if (get_status(i))
			close_log(i);
}

void
init_logger ()
{
	if(init_var == 0) {
		int i;

		init_var = 1;
		for(i=0; i<MAX_LOGS; i++) {
			memset(&_logs[i], 0, sizeof(struct CLOG));
			init_file(&_logs[i].file);
		}
		atexit(_logger_exit_func);
	} else {
		printf("C_Logger is already initialized!\n");
	}
}

void
open_log (int logNum, const char *name)
{
	if(init_var) {
		if(!get_status(logNum)) {
			open_file(&_logs[logNum].file, name, "a+t");
			if(get_errori_file(&_logs[logNum].file)
				!= FILE_ERROR_OKAY)
				return;
			_logs[logNum].status = 1;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}

int
read_log (int logNum, char *buf, int size)
{
	if(init_var) {
		if(get_status(logNum)) {
			int c, pos;

			seek_file(&_logs[logNum].file,
				_logs[logNum].read_pos, SEEK_SET);
			if(get_errori_file(&_logs[logNum].file)
				!= FILE_ERROR_OKAY) {
				printf("Error: %s\n",
					get_error_file(&_logs[logNum].file));
				return -1;
			}
			pos = 0;
			memset(buf, 0, size);
			while(pos < size &&
				((c = getc_file(&_logs[logNum].file))
				!= '\n'))
				buf[pos++] = c;
			_logs[logNum].read_pos =
				tell_file(&_logs[logNum].file);
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

void
write_log (int logNum, const char *data, ...)
{
	if(init_var) {
		if(get_status(logNum)) {
			va_list ap;
			seek_file(&_logs[logNum].file,
				_logs[logNum].write_pos,
				SEEK_SET);
			va_start(ap, data);
			vfprintf(_logs[logNum].file.fp, data, ap);
			va_end(ap);
			_logs[logNum].write_pos =
				tell_file(&_logs[logNum].file);
			flush_file(&_logs[logNum].file);
			return;
		}
		printf("Warning: Not writing, log CLOG%d not open.\n", logNum);
		return;
	}
	printf("Please use init_logger() first.\n");
}

void
close_log (int logNum)
{
	if(init_var) {
		if(get_status(logNum)) {
			close_file(&_logs[logNum].file);
			_logs[logNum].status = 0;
			_logs[logNum].read_pos = 0;
			_logs[logNum].write_pos = 0;
			return;
		}
		return;
	}
	printf("Please use init_logger() first.\n");
}

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

int
get_status (int logNum)
{
	if(init_var)
		return _logs[logNum].status;
	printf("Please use init_logger() first.\n");
	return 0;
}

const char*
get_log_name (int logNum)
{
	if(init_var)
		return _logs[logNum].file.name;
	printf("Please use init_logger() first.\n");
	return 0;
}
