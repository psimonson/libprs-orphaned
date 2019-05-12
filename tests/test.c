#include <stdio.h>
#include "file.h"

int
main ()
{
    file_t file;
    int c;
    
    init_file(&file);
    open_file(&file, "test.c", "rt");
    if(get_errori_file(&file) != FILE_ERROR_OKAY)
        goto error;
    while((c = getc_file(&file)) != EOF)
        putchar(c);
    if(get_errori_file(&file) != FILE_ERROR_OKAY)
        goto error;
    printf("Lines: %ld\nSize: %ld\n", file.lines, file.size);
    close_file(&file);
    return 0;

error:
    printf("Error: %s\n", get_error_file(&file));
    close_file(&file);
    return 1;
}
