/**
 * @file clogger.h
 * @author Philip R. Simonson
 * @date   03/13/2019
 ******************************************************************************
 * @details
 *
 * My simple log file library, used for making logging easier. Like to report
 * stuff happening in your program.
 ******************************************************************************
 */

#ifndef PRS_CLOGGER_H
#define PRS_CLOGGER_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_PATH
#define MAX_PATH 260 /**< Max chars for path */
#endif

#define MAX_LOGS 5 /**< Max number of logs */

/**
 * @brief Error codes for clogger.
 */
enum CLOG_ERROR {
	CLOGERR_OKAY,
	CLOGERR_OPEN,
	CLOGERR_CLOSE,
	CLOGERR_WRITE,
	CLOGERR_READ
};

/**
 * @brief Log numbers range from (0-4) total 5.
 */
enum CLOG_ENUM {
	CLOG0,
	CLOG1,
	CLOG2,
	CLOG3,
	CLOG4
};

/** @brief This function must be run first. */
PRS_EXPORT void init_logger();
/** @brief Open a log file for reading writing. */
PRS_EXPORT void open_log(int logNum, const char *name);
/** @brief Read a log file. */
PRS_EXPORT int read_log(int logNum, char *buf, int size);
/** @brief Write to a log file. */
PRS_EXPORT void write_log(int logNum, const char *data, ...);
/** @brief Close an opened log file. */
PRS_EXPORT void close_log(int logNum);
/** @brief Print status of log file. */
PRS_EXPORT void print_status_log(int logNum);
/** @brief Get status from log file. */
PRS_EXPORT int get_status_log(int logNum);
/** @brief Get log file name. */
PRS_EXPORT const char *get_name_log(int logNum);

#ifdef __cplusplus
}
#endif

#endif
