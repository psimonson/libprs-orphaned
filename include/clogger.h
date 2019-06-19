/*
 * c_logger.h - My C logger header library.
 *
 * Author : Philip R. Simonson
 * Date   : 03/13/2019
 ******************************************************************************
 */

#define PRS_CLOGGER_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

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

PRS_EXPORT void init_logger ();
PRS_EXPORT void open_log (int logNum, const char *name);
PRS_EXPORT int read_log (int logNum, char *buf, int size);
PRS_EXPORT void write_log (int logNum, const char *data, ...);
PRS_EXPORT void close_log (int logNum);
PRS_EXPORT void print_status (int logNum);
PRS_EXPORT int get_status (int logNum);
PRS_EXPORT const char* get_log_name (int logNum);

#ifdef __cplusplus
}
#endif
