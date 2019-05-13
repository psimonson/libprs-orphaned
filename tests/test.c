#include <stdio.h>
#include "file.h"
#include "clogger.h"

int
main ()
{
    file_t file;
    int c;
    
    init_logger();
    init_file(&file);
    open_log(CLOG0, "test.log");
    open_file(&file, "test.c", "rt");
    if(get_errori_file(&file) != FILE_ERROR_OKAY)
        goto error;
    write_log(CLOG0, "Opened CLOG0\n");
    while((c = getc_file(&file)) != EOF)
        putchar(c);
    if(get_errori_file(&file) != FILE_ERROR_OKAY)
        goto error;
    write_log(CLOG0, "Lines: %ld\nSize: %ld\n", file.lines, file.size);
    close_file(&file);
    close_log(CLOG0);
    return 0;

error:
    printf("Error: %s\n", get_error_file(&file));
    close_file(&file);
    close_log(CLOG0);
    return 1;
}
