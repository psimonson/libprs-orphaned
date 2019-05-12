/*
 * file.c - main source file for file operations.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/11
 ********************************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "file.h"

static const char *_prs_file_errors[] = {
    "File is okay (no error).",
    "File cannot be opened.",
    "Cannot get file size.",
    "Cannot get lines count from file.",
    "File was unable to be read.",
    "File was unable to be written to.",
    "Cannot uninitialize file structure.",
    "Cannot initialize file structure.",
    "File was closed."
};

/* --------------------------- helper funtions ------------------------- */

/* get_size_file:  gets the size of the current file */
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

/* get_lines_file:  gets the line count of the current file */
long
get_lines_file (file_t* file)
{
    long nl;
    int c;
    fseek(file->fp, 0, SEEK_SET);
    for(nl=0; (c = fgetc(file->fp)) != EOF;)
        if(c == '\n')
            nl++;
    fseek(file->fp, 0, SEEK_SET);
    return nl;
}

/* ------------------------- standard functions ------------------------ */

/* init_file:  initialise the file structure */
void
init_file (file_t* file)
{
    file->fp = NULL;
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    file->error = FILE_ERROR_OKAY;
}

/* open_file:  open a file by (path, mode) */
void
open_file (file_t* file, const char* filename, const char* mode)
{
    if((file->fp = fopen(filename, mode)) == NULL) {
        file->error = FILE_ERROR_OPEN;
        return;
    }
    strncpy(file->name, filename, strlen(filename));
    if(strchr(mode, 'w') == NULL) {
        file->size = get_size_file(file);
        if(file->size < 0) {
            close_file(file);
            file->error = FILE_ERROR_SIZE;
            return;
        }
        if(strchr(mode, 'b') == NULL) {
            file->lines = get_lines_file(file);
            if(file->lines < 0) {
                close_file(file);
                file->error = FILE_ERROR_LINE;
                return;
            }
        }
    } else {
        file->size = 0;
        file->lines = 0;
    }
    file->error = FILE_ERROR_OKAY;
    fseek(file->fp, 0, SEEK_SET);
}

/* get_error_file:  gets the error string associated with error code */
const char*
get_error_file (file_t* file)
{
    return _prs_file_errors[file->error];
}

/* get_errori_file:  gets the error code from file */
int
get_errori_file (file_t* file)
{
    return file->error;
}

/* close_file:  uninitialize the file structure; closing file */
void
close_file (file_t* file)
{
    fclose(file->fp);
    memset(file->name, 0, MAX_PATH);
    file->size = -1;
    file->lines = -1;
    file->error = FILE_ERROR_CLOSED;
}

/* ------------------------- handling functions ------------------------ */

/* read_file:  read file into buf; of size */
int
read_file (file_t* file, char* buf, int size)
{
    int bytes;
    if((bytes = fread(buf, 1, size, file->fp)) < 0)
        file->error = FILE_ERROR_READ;
    return bytes;
}

/* write_file:  write into file from buf; of size */
int
write_file (file_t* file, const char* buf, int size)
{
    int bytes;
    if((bytes = fwrite(buf, 1, size, file->fp)) < 0)
        file->error = FILE_ERROR_WRITE;
    return bytes;
}

/* writef_file:  write formatted into file */
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

/* readf_file:  read formatted from file */
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

/* getc_file:  gets one byte from the file */
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

/* putc_file:  puts one byte into the file */
void
putc_file (file_t* file, int c)
{
    errno = 0;
    fputc(c, file->fp);
    if(errno != 0)
        file->error = FILE_ERROR_WRITE;
}
