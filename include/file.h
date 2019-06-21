/* file.h - header file for my file handling library.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/11
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

typedef struct file {
    FILE *fp;
    char name[MAX_PATH];
    long size;
    long lines;
    int error;
} file_t;

/* standard file funtions */
PRS_EXPORT void
init_file (file_t* file);
PRS_EXPORT void
open_file (file_t* file, const char* filename, const char* mode);
PRS_EXPORT void
reopen_file (file_t* file, const char* filename, const char* mode);
PRS_EXPORT const char*
get_error_file (file_t* file);
PRS_EXPORT int
get_errori_file (file_t* file);
PRS_EXPORT void
close_file (file_t* file);

/* standard handling functions */
PRS_EXPORT int
read_file (file_t* file, void* buf, size_t nmem, size_t size);
PRS_EXPORT int
write_file (file_t* file, const void* buf, size_t nmem, size_t size);
PRS_EXPORT int
writef_file (file_t* file, const char* buf, ...);
PRS_EXPORT int
readf_file (file_t* file, const char* buf, ...);
PRS_EXPORT int
vwritef_file (file_t* file, const char* buf, va_list ap);
PRS_EXPORT char*
gets_file (file_t* file, char* buf, long size);
PRS_EXPORT int
getc_file (file_t* file);
PRS_EXPORT void
putc_file (file_t* file, int c);
PRS_EXPORT void
ungetc_file (file_t* file, int c);
PRS_EXPORT int
seek_file (file_t* file, long bytes, int seek);
PRS_EXPORT long
tell_file (file_t* file);
PRS_EXPORT int
flush_file (file_t* file);

/* get functions */
PRS_EXPORT const char*
get_name_file (file_t* file);
PRS_EXPORT long
get_size_file (file_t* file);
PRS_EXPORT int
get_lines_file (file_t* file);
PRS_EXPORT FILE*
get_handle_file (file_t* file);

#ifdef __cplusplus
}
#endif

#endif
