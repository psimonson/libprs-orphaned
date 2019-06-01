/* file.h - header file for my file handling library.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/11
 ********************************************************************
 */

#define PRS_CFILE_H

#include <stdarg.h>

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
extern void
init_file (file_t* file);
extern void
open_file (file_t* file, const char* filename, const char* mode);
extern void
reopen_file (file_t* file, const char* filename, const char* mode);
extern const char*
get_error_file (file_t* file);
extern int
get_errori_file (file_t* file);
extern void
close_file (file_t* file);

/* standard handling functions */
extern int
read_file (file_t* file, void* buf, size_t nmem, size_t size);
extern int
write_file (file_t* file, const void* buf, size_t nmem, size_t size);
extern int
writef_file (file_t* file, const char* buf, ...);
extern int
vwritef_file (file_t* file, const char* buf, va_list ap);
extern int
readf_file (file_t* file, const char* buf, ...);
extern int
getc_file (file_t* file);
extern void
putc_file (file_t* file, int c);
extern void
ungetc_file (file_t* file, int c);
extern int
seek_file (file_t* file, long bytes, int seek);
extern long
tell_file (file_t* file);
extern int
flush_file (file_t* file);

/* get functions */
extern const char*
get_name_file (file_t* file);
extern long
get_size_file (file_t* file);
extern int
get_lines_file (file_t* file);
extern FILE*
get_handle_file (file_t* file);
