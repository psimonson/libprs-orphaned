#include <stdio.h>
#include <string.h>
#include "file.h"

int
main ()
{
    char buf[32];
    file_t file1;
    file_t file2;
    int bytes;
    
    init_file(&file1);
    init_file(&file2);
    
    /* open files */
    open_file(&file1, "test.c", "rt");
    if(get_errori_file(&file1) != FILE_ERROR_OKAY)
        return 1;
    open_file(&file2, "test.txt", "wt");
    if(get_errori_file(&file2) != FILE_ERROR_OKAY) {
        close_file(&file1);
        return 1;
    }
    
    /* copy file */
    while((bytes = read_file(&file1, buf, 1, 32)) > 0) {
        write_file(&file2, buf, 1, bytes);
    }
    if(get_errori_file(&file1) != FILE_ERROR_OKAY) {
        printf("Error: %s\n", get_error_file(&file1));
        close_file(&file1);
        return 1;
    }
    if(get_errori_file(&file2) != FILE_ERROR_OKAY) {
        printf("Error: %s\n", get_error_file(&file2));
        close_file(&file2);
        return 1;
    }
    remove(file2.name);
    close_file(&file1);
    close_file(&file2);
    printf("File was written successfully.\n");
    return 0;
}
