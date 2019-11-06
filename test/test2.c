#include <stdio.h>
#include <string.h>
#include "file.h"

int
main ()
{
    char buf[32];
    file_t *file1;
    file_t *file2;
    int bytes;
    
    /* open files */
    file1 = open_file("test.c", "rt");
    if(file1 == NULL)
        return 1;
    file2 = open_file("test.txt", "wt");
    if(file2 == NULL) {
        close_file(file1);
        return 1;
    }
    
    /* copy file */
    while((bytes = read_file(file1, buf, 1, 32)) > 0) {
        write_file(file2, buf, 1, bytes);
    }
    if(get_errori_file(file1) != FILE_ERROR_OKAY) {
        printf("Error: %s\n", get_error_file(file1));
        close_file(file1);
        return 1;
    }
    if(get_errori_file(file2) != FILE_ERROR_OKAY) {
        printf("Error: %s\n", get_error_file(file2));
        close_file(file2);
        return 1;
    }
    remove(get_name_file(file2));
    close_file(file1);
    close_file(file2);
    printf("File was written successfully.\n");
    return 0;
}
