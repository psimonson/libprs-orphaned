#include <stdio.h>
#include "file.h"
#include "clogger.h"

int
main ()
{
    file_t *file;
    int c, error;

    init_logger();
    open_log(CLOG0, "test.log");
    file = open_file("test.c", "rt");
    if(file == NULL) {
        close_log(CLOG0);
        return 1;
    }
    write_log(CLOG0, "Opened CLOG0\n");
    while((c = getc_file(file)) != EOF)
        putchar(c);
    if((error = get_error_file()) != FILE_ERROR_OKAY)
        goto error;
    write_log(CLOG0, "File: %s\nLines: %ld\nSize: %ld\n",
        get_name_log(CLOG0), get_lines_file(file), get_size_file(file));
    close_file(file);
    close_log(CLOG0);
    return 0;

error:
    printf("Error: %s\n", strerror_file(error));
    close_file(file);
    close_log(CLOG0);
    return 1;
}
