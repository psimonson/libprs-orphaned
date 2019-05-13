/*
 * c_logger.h - My C logger header library.
 *
 * Author : Philip R. Simonson
 * Date   : 03/13/2019
 ******************************************************************************
 */

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define MAX_LOGS 5

enum CLOG_ENUM {
	CLOG0,
	CLOG1,
	CLOG2,
	CLOG3,
	CLOG4
};

extern void
init_logger ();

extern void
open_log (int logNum, const char *name);

extern int
read_log (int logNum, char *buf, int size);

extern void
write_log (int logNum, const char *data, ...);

extern void
close_log (int logNum);

extern void
print_status (int logNum);

extern int
get_status (int logNum);

extern const char*
get_log_name (int logNum);
