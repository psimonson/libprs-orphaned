/* file.h - header file for my file handling library.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/11
 ********************************************************************
 */

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
    FILE_ERROR_UINIT,
    FILE_ERROR_INIT,
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
void init_file(file_t* file);
void open_file(file_t* file, const char* filename, const char* mode);
const char* get_error_file(file_t* file);
int get_errori_file(file_t* file);
void close_file(file_t* file);

/* standard handling functions */
int read_file(file_t* file, char* buf, int size);
int write_file(file_t* file, const char* buf, int size);
int writef_file(file_t* file, const char* buf, ...);
int readf_file(file_t* file, const char* buf, ...);
int getc_file(file_t* file);
void putc_file(file_t* file, int c);
