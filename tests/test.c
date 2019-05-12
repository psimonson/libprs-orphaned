#include <stdio.h>

#include "file.h"

int
main ()
{
    char buf[BUFSIZ];
    file_t file;
    int bytes;
    
    init_file(&file);
    open_file(&file, "test.c", "rt");
    if(get_errori_file(&file) != FILE_ERROR_OKAY)
        goto error;
    while((bytes = read_file(&file, buf, sizeof(buf))) > 0) {
        buf[bytes] = 0;
        puts(buf);
        fflush(stdout);
    }
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
