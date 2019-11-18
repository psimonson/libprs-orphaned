/**
 * @file file.c
 * @author Philip R. Simonson
 * @date 11 May 2019
 * @brief Simple file operation library.
 * @details
 *
 * This file library does make a bunch of calls into
 * not so many. With just a few functions you can open
 * a file and close. Or open two file, copy one to the
 * other and close both with about 10-15 lines of code.
 * Easier file handling and error checking.
 */

#if defined(__linux) || defined(__UNIX__)
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "file.h"

struct file {
    FILE *fp;
    char name[MAX_PATH];
    long size;
    long lines;
    int error;
};

static const char *_prs_file_errors[] = {
    "File is okay (no error).",
    "File cannot be opened.",
    "Cannot get file size.",
    "Cannot get lines count from file.",
    "File was unable to be read.",
    "File was unable to be written to.",
    "File was unable to seek.",
    "Cannot tell size of file.",
    "File was closed."
};

/* ------------------------- standard functions ------------------------ */

/**
 * @brief Open a file by (path, mode).
 */
file_t*
open_file(const char* filename, const char* mode)
{
    file_t *file;
    file = (file_t*)malloc(sizeof(file_t));
    if(file == NULL)
	return NULL;
    file->fp = NULL;
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    file->error = FILE_ERROR_OKAY;
    if((file->fp = fopen(filename, mode)) == NULL) {
        file->error = FILE_ERROR_OPEN;
        return file;
    }
    strcpy(file->name, filename);
    if(strchr(mode, 'w') == NULL) {
        file->size = get_size_file(file);
        if(file->size < 0) {
            file->error = FILE_ERROR_SIZE;
        }
        if(strchr(mode, 'b') == NULL) {
            file->lines = get_lines_file(file);
            if(file->lines < 0) {
                file->error = FILE_ERROR_LINE;
            }
	}
	return file;
    } else {
        file->size = 0;
        file->lines = 0;
    }
    file->error = FILE_ERROR_OKAY;
    fseek(file->fp, 0, SEEK_SET);
    return file;
}

/**
 * @brief Reopen file with different mode.
 */
file_t*
reopen_file(file_t *file, const char* mode)
{
    if(file != NULL &&
             (file->fp = freopen(file->name, mode, file->fp)) == NULL) {
        file->error = FILE_ERROR_OPEN;
	return file;
    }
    file->size = -1;
    file->lines = -1;
    file->error = FILE_ERROR_OKAY;
    if(strchr(mode, 'w') == NULL) {
        file->size = get_size_file(file);
        if(file->size < 0) {
            file->error = FILE_ERROR_SIZE;
        }
        if(strchr(mode, 'b') == NULL) {
            file->lines = get_lines_file(file);
            if(file->lines < 0) {
                file->error = FILE_ERROR_LINE;
            }
        }
	return file;
    } else {
        file->size = 0;
        file->lines = 0;
    }
    file->error = FILE_ERROR_OKAY;
    fseek(file->fp, 0, SEEK_SET);
    return file;
}

/**
 * @brief Gets the error string associated with error code.
 */
const char*
get_error_file (file_t* file)
{
    return _prs_file_errors[file->error];
}

/**
 * @brief Gets the error code from file.
 */
int
get_errori_file (file_t* file)
{
    return file->error;
}

/**
 * @brief Uninitialize the file structure; closing file.
 */
void
close_file(file_t* file)
{
    if(file->fp != NULL) fclose(file->fp);
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    file->error = FILE_ERROR_CLOSED;
    free(file);
}

/* ------------------------- handling functions ------------------------ */

/**
 * @brief Read file into buf; of size
 */
int
read_file (file_t* file, void* buf, size_t nmem, size_t size)
{
    int bytes;
    if((bytes = fread(buf, nmem, size, file->fp)) < 0)
        file->error = FILE_ERROR_READ;
    return bytes;
}
/**
 * @brief Write into file from buf; of size
 */
int
write_file (file_t* file, const void* buf, size_t nmem, size_t size)
{
    int bytes;
    if((bytes = fwrite(buf, nmem, size, file->fp)) < 0)
        file->error = FILE_ERROR_WRITE;
    return bytes;
}
/**
 * @brief Write formatted into file
 */
int
writef_file (file_t* file, const char* buf, ...)
{
    int res;
    va_list ap;
    va_start(ap, buf);
    res = vfprintf(file->fp, buf, ap);
    va_end(ap);
    if(res < 0)
        file->error = FILE_ERROR_WRITE;
    return res;
}
/**
 * @brief Write formatted into file; using va_list
 */
int
vwritef_file (file_t* file, const char* buf, va_list ap)
{
    return vfprintf(file->fp, buf, ap);
}
/**
 * @brief Read a line of text from file
 */
char*
gets_file (file_t* file, char* buf, long size)
{
	return fgets(buf, size, file->fp);
}
/**
 * @brief Put a line of text to file
 */
int
puts_file (file_t* file, const char* buf)
{
	return fputs(buf, file->fp);
}
/**
 * @brief Read formatted from file
 */
int
readf_file (file_t* file, const char* buf, ...)
{
    int res;
    va_list ap;
    va_start(ap, buf);
    res = vfscanf(file->fp, buf, ap);
    va_end(ap);
    if(res < 0)
        file->error = FILE_ERROR_READ;
    return res;
}
/**
 * @brief Gets one byte from the file
 */
int
getc_file (file_t* file)
{
    int c;
    
    errno = 0;
    c = fgetc(file->fp);
    if(errno != 0)
        file->error = FILE_ERROR_READ;
    return c;
}
/**
 * @brief Puts one byte into the file
 */
void
putc_file (file_t* file, int c)
{
    errno = 0;
    fputc(c, file->fp);
    if(errno != 0)
        file->error = FILE_ERROR_WRITE;
}
/**
 * @brief Puts one byte back onto file stream.
 */
void
ungetc_file (file_t* file, int c)
{
    errno = 0;
    ungetc(c, file->fp);
    if(errno != 0)
        file->error = FILE_ERROR_WRITE;
}
/**
 * @brief Seek through file by bytes.
 */
int
seek_file (file_t* file, long bytes, int seek)
{
    int res;
    errno = 0;
    res = fseek(file->fp, bytes, seek);
    if(errno != 0)
        file->error = FILE_ERROR_SEEK;
    return res;
}
/**
 * @brief Rewind file back to start.
 */
void
rewind_file (file_t* file)
{
	rewind(file->fp);
}
/**
 * @brief Tell size of file; returns size in bytes.
 */
long
tell_file (file_t* file)
{
    long size;
    errno = 0;
    size = ftell(file->fp);
    if(errno != 0)
        file->error = FILE_ERROR_TELL;
    return size;
}
/**
 * @brief Flushs a file.
 */
int
flush_file (file_t* file)
{
    return fflush(file->fp);
}

/* --------------------------- helper funtions ------------------------- */

/**
 * @brief Gets the handle for a given file; returns FILE pointer.
 */
FILE*
get_handle_file (file_t* file)
{
    return (file->fp == NULL) ? NULL : file->fp;
}
/**
 * @brief Gets the name of the file passed in.
 */
const char*
get_name_file (file_t* file)
{
    return file->name;
}
/**
 * @brief Gets the size of the current file.
 */
long
get_size_file (file_t* file)
{
    long size,cur_pos;
    cur_pos = ftell(file->fp);
    fseek(file->fp, 0, SEEK_END);
    size = ftell(file->fp);
    fseek(file->fp, cur_pos, SEEK_SET);
    if(size < 0)
        file->error = FILE_ERROR_SIZE;
    return size;
}
/**
 * @brief Gets the line count of the current file.
 */
int
get_lines_file (file_t* file)
{
    int nl,c;
    fseek(file->fp, 0, SEEK_SET);
    for(nl=0; (c = fgetc(file->fp)) != EOF;)
        if(c == '\n')
            nl++;
    fseek(file->fp, 0, SEEK_SET);
    return nl;
}
