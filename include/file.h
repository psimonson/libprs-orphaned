/**
 * @file file.h
 * @author Philip R. Simonson
 * @date   2019/05/11
 ********************************************************************
 * @details
 *
 * File handling library, open, read/write, and close files in a
 * simple manner.
 ********************************************************************
 */

#ifndef PRS_CFILE_H
#define PRS_CFILE_H

#include "export.h"

#ifndef va_list
#include <stdarg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_PATH
#define MAX_PATH 260                /* needed for unix/linux */
#endif

/** @brief File error enumeration. */
enum {
    FILE_ERROR_OKAY,
    FILE_ERROR_OPEN,
    FILE_ERROR_SIZE,
    FILE_ERROR_LINE,
    FILE_ERROR_READ,
    FILE_ERROR_WRITE,
    FILE_ERROR_SEEK,
    FILE_ERROR_TELL,
    FILE_ERROR_CLOSED
};

/** @brief File structure, for handling files in this library. */
typedef struct file {
    FILE *fp;
    char name[MAX_PATH];
    long size;
    long lines;
    int error;
} file_t;

/** @brief Initialise the file structure. */
PRS_EXPORT void
init_file (file_t* file);
/** @brief Open a file with open mode. */
PRS_EXPORT void
open_file (file_t* file, const char* filename, const char* mode);
/** @brief Reopen a file with open mode. */
PRS_EXPORT void
reopen_file (file_t* file, const char* filename, const char* mode);
/** @brief Get error for file structure (const char *). */
PRS_EXPORT const char*
get_error_file (file_t* file);
/** @brief Get error for file structure (int). */
PRS_EXPORT int
get_errori_file (file_t* file);
/** @brief Close a file structure. */
PRS_EXPORT void
close_file (file_t* file);

/** @brief Read from a file, this is like fread(). */
PRS_EXPORT int
read_file (file_t* file, void* buf, size_t nmem, size_t size);
/** @brief Write to a file, this is like fwrite(). */
PRS_EXPORT int
write_file (file_t* file, const void* buf, size_t nmem, size_t size);
/** @brief Write formatted text to file. */
PRS_EXPORT int
writef_file (file_t* file, const char* buf, ...);
/** @brief Read formatted text from file. */
PRS_EXPORT int
readf_file (file_t* file, const char* buf, ...);
/** @brief Like vprintf() but, for files. */
PRS_EXPORT int
vwritef_file (file_t* file, const char* buf, va_list ap);
/** @brief Get line of text from file. */
PRS_EXPORT char*
gets_file (file_t* file, char* buf, long size);
/** @brief Put line of text to file. */
PRS_EXPORT int
puts_file (file_t* file, const char* buf);
/** @brief Get character from file. */
PRS_EXPORT int
getc_file (file_t* file);
/** @brief Put character to file. */
PRS_EXPORT void
putc_file (file_t* file, int c);
/** @brief Put character back on buffer. */
PRS_EXPORT void
ungetc_file (file_t* file, int c);
/** @brief Seek through file. */
PRS_EXPORT int
seek_file (file_t* file, long bytes, int seek);
/** @brief Rewind file, start from beginning. */
PRS_EXPORT void
rewind_file (file_t* file);
/** @brief Tell file, where are we in the file. */
PRS_EXPORT long
tell_file (file_t* file);
/** @brief Flush file. */
PRS_EXPORT int
flush_file (file_t* file);

/** @brief Get the name of the file. */
PRS_EXPORT const char*
get_name_file (file_t* file);
/** @brief Get the size of the file in bytes. */
PRS_EXPORT long
get_size_file (file_t* file);
/** @brief Get the number of lines in the file. */
PRS_EXPORT int
get_lines_file (file_t* file);
/** @brief Get the handle to the file. */
PRS_EXPORT FILE*
get_handle_file (file_t* file);

#ifdef __cplusplus
}
#endif

#endif
