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

static int _errno_file;

struct file {
    FILE *fp;
    char name[MAX_PATH];
    long size;
    long lines;
};

static const char *_prs_file_errors[] = {
    "File is okay (no error).",
    "File cannot be opened.",
    "Cannot get file size.",
    "Cannot get lines count from file.",
    "File was unable to be read.",
    "File was unable to be written to.",
    "File was unable to seek.",
    "Cannot tell size of file."
};

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------- standard functions ------------------------ */

/* Open a file by (path, mode).
 */
PRS_EXPORT file_t *open_file(const char *filename, const char *mode)
{
    file_t *file;
    file = (file_t*)malloc(sizeof(file_t));
    if(file == NULL)
	return NULL;
    file->fp = NULL;
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    _errno_file = FILE_ERROR_OKAY;
    if((file->fp = fopen(filename, mode)) == NULL) {
        _errno_file = FILE_ERROR_OPEN;
        return file;
    }
    strcpy(file->name, filename);
    if(strchr(mode, 'w') == NULL) {
        file->size = get_size_file(file);
        if(file->size < 0) {
            _errno_file = FILE_ERROR_SIZE;
        }
        if(strchr(mode, 'b') == NULL) {
            file->lines = get_lines_file(file);
            if(file->lines < 0) {
                _errno_file = FILE_ERROR_LINE;
            }
	}
	return file;
    } else {
        file->size = 0;
        file->lines = 0;
    }
    _errno_file = FILE_ERROR_OKAY;
    fseek(file->fp, 0, SEEK_SET);
    return file;
}
/* Reopen file with different mode.
 */
PRS_EXPORT file_t *reopen_file(file_t *file, const char *mode)
{
    if(file != NULL &&
             (file->fp = freopen(file->name, mode, file->fp)) == NULL) {
        _errno_file = FILE_ERROR_OPEN;
	close_file(file);
	return NULL;
    }
    file->size = -1;
    file->lines = -1;
    _errno_file = FILE_ERROR_OKAY;
    if(strchr(mode, 'w') == NULL) {
        file->size = get_size_file(file);
        if(file->size < 0) {
            _errno_file = FILE_ERROR_SIZE;
        }
        if(strchr(mode, 'b') == NULL) {
            file->lines = get_lines_file(file);
            if(file->lines < 0) {
                _errno_file = FILE_ERROR_LINE;
            }
        }
	return file;
    } else {
        file->size = 0;
        file->lines = 0;
    }
    _errno_file = FILE_ERROR_OKAY;
    fseek(file->fp, 0, SEEK_SET);
    return file;
}
/* Gets the error string associated with error code.
 */
PRS_EXPORT const char *strerror_file(int err)
{
    return _prs_file_errors[err];
}
/* Gets the error code from file.
 */
PRS_EXPORT int get_error_file(void)
{
    int error = _errno_file;
    _errno_file = FILE_ERROR_OKAY;
    return error;
}
/* Uninitialize the file structure; closing file.
 */
PRS_EXPORT void close_file(file_t *file)
{
    if(file->fp != NULL) fclose(file->fp);
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    _errno_file = FILE_ERROR_OKAY;
    free(file);
}

/* ------------------------- handling functions ------------------------ */

/* Read file into buf; of size
 */
PRS_EXPORT int read_file(file_t *file, void *buf, size_t nmem, size_t size)
{
    int bytes;
    if((bytes = fread(buf, nmem, size, file->fp)) < 0)
        _errno_file = FILE_ERROR_READ;
    return bytes;
}
/* Write into file from buf; of size
 */
PRS_EXPORT int write_file(file_t *file, const void *buf, size_t nmem,
	size_t size)
{
    int bytes;
    if((bytes = fwrite(buf, nmem, size, file->fp)) < 0)
        _errno_file = FILE_ERROR_WRITE;
    return bytes;
}
/* Write formatted into file
 */
PRS_EXPORT int writef_file(file_t *file, const char *buf, ...)
{
    int res;
    va_list ap;
    va_start(ap, buf);
    res = vfprintf(file->fp, buf, ap);
    va_end(ap);
    if(res < 0)
        _errno_file = FILE_ERROR_WRITE;
    return res;
}
/* Write formatted into file; using va_list
 */
PRS_EXPORT int vwritef_file(file_t *file, const char *buf, va_list ap)
{
    return vfprintf(file->fp, buf, ap);
}
/* Read a line of text from file
 */
PRS_EXPORT char *gets_file(file_t *file, char *buf, long size)
{
	return fgets(buf, size, file->fp);
}
/* Put a line of text to file
 */
PRS_EXPORT int puts_file(file_t *file, const char *buf)
{
	return fputs(buf, file->fp);
}
/* Read formatted from file
 */
PRS_EXPORT int readf_file(file_t *file, const char *buf, ...)
{
    int res;
    va_list ap;
    va_start(ap, buf);
    res = vfscanf(file->fp, buf, ap);
    va_end(ap);
    if(res < 0)
        _errno_file = FILE_ERROR_READ;
    return res;
}
/* Gets one byte from the file
 */
PRS_EXPORT int getc_file(file_t *file)
{
    int c;
    
    errno = 0;
    c = fgetc(file->fp);
    if(errno != 0)
        _errno_file = FILE_ERROR_READ;
    return c;
}
/* Puts one byte into the file.
 */
PRS_EXPORT void putc_file(file_t *file, int c)
{
    errno = 0;
    fputc(c, file->fp);
    if(errno != 0)
        _errno_file = FILE_ERROR_WRITE;
}
/* Puts one byte back onto file stream.
 */
PRS_EXPORT void ungetc_file(file_t *file, int c)
{
    errno = 0;
    ungetc(c, file->fp);
    if(errno != 0)
        _errno_file = FILE_ERROR_WRITE;
}
/* Seek through file by bytes.
 */
PRS_EXPORT int seek_file(file_t *file, long bytes, int seek)
{
    int res;
    errno = 0;
    res = fseek(file->fp, bytes, seek);
    if(errno != 0)
        _errno_file = FILE_ERROR_SEEK;
    return res;
}
/* Rewind file back to start.
 */
PRS_EXPORT void rewind_file(file_t *file)
{
	rewind(file->fp);
}
/* Tell size of file; returns size in bytes.
 */
PRS_EXPORT long tell_file(file_t *file)
{
    long size;
    errno = 0;
    size = ftell(file->fp);
    if(errno != 0)
        _errno_file = FILE_ERROR_TELL;
    return size;
}
/* Flush the file stream.
 */
PRS_EXPORT int flush_file(file_t *file)
{
    return fflush(file->fp);
}

/* --------------------------- helper funtions ------------------------- */

/* Gets the handle for a given file; returns FILE pointer.
 */
PRS_EXPORT FILE *get_handle_file(file_t *file)
{
    return (file->fp == NULL) ? NULL : file->fp;
}
/* Gets the name of the file passed in.
 */
PRS_EXPORT const char *get_name_file(file_t *file)
{
    return file->name;
}
/* Gets the size of the current file.
 */
PRS_EXPORT long get_size_file(file_t *file)
{
    long size, cur_pos;
    cur_pos = ftell(file->fp);
    fseek(file->fp, 0, SEEK_END);
    errno = 0;
    size = ftell(file->fp);
    if(errno != 0)
        _errno_file = FILE_ERROR_SIZE;
    fseek(file->fp, cur_pos, SEEK_SET);
    return size;
}
/* Gets the line count of the current file.
 */
PRS_EXPORT int get_lines_file(file_t *file)
{
    int nl,c;
    fseek(file->fp, 0, SEEK_SET);
    for(nl=0; (c = fgetc(file->fp)) != EOF;)
        if(c == '\n')
            nl++;
    fseek(file->fp, 0, SEEK_SET);
    return nl;
}
#ifdef __cplusplus
}
#endif
