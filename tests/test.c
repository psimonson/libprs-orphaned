#include <stdio.h>
#include "prslib.h"
#include PRS_CLOGGER
#include PRS_CFILE

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
    write_log(CLOG0, "File: %s\nLines: %ld\nSize: %ld\n",
        get_log_name(CLOG0), file.lines, file.size);
    close_file(&file);
    close_log(CLOG0);
    return 0;

error:
    printf("Error: %s\n", get_error_file(&file));
    close_file(&file);
    close_log(CLOG0);
    return 1;
}
