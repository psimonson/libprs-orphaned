#include <stdio.h>
#include "file.h"

/* program to test file operations and getter functions */
int
main ()
{
    file_t *file;
    int c, err;
    
    file = open_file("test6.c", "rt");
    if(file == NULL) {
	return 1;
    } else if((err = get_error_file()) != FILE_ERROR_OKAY) {
        fprintf(stderr, "Error: %s\n", strerror_file(err));
        return 1;
    }
    printf("File Pointer: %p\n", get_handle_file(file));
    printf("Name of File: %s\n", get_name_file(file));
    printf("File Size: %ld\nNumber of lines in file: %d\n",
            get_size_file(file), get_lines_file(file));
    printf("\t>>> File contents below <<<\n"
            "=============================\n");
    while((c = getc_file(file)) != EOF)
        putchar(c);
    close_file(file);
    return 0;
}
