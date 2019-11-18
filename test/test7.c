#include <stdio.h>
#include "file.h"

int main()
{
	char buf[15];
	file_t *f;
	f = open_file("test7.txt", "wt");
	if(get_errori_file(f) != FILE_ERROR_OKAY)
		return 1;
	writef_file(f, "Hello world!\n");
	f = reopen_file(f, "rt");
	if(get_errori_file(f) != FILE_ERROR_OKAY)
		return 1;
	read_file(f, buf, 1, 13);
	buf[13] = 0;
	printf("Data: %s", buf);
	close_file(f);
	return 0;
}
