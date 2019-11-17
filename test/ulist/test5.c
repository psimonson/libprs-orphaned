#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ulist.h"

void strip_newline(char *s, int size)
{
	int i;
	for(i = 0; i < size; i++) {
		if(s[i] == '\n' || s[i] == '\r') {
			while(i < size-1) {
				s[i] = 0;
				i++;
			}
			break;
		}
	}
	s[i] = 0;
}

static void *create_data(const char *s)
{
	char *data;
	data = (char*)malloc(strlen(s)+1);
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		return NULL;
	}
	strcpy(data, s);
	return data;
}

static void destroy_data(void *data)
{
	if(data != NULL) free(data);
}

int main()
{
	ulist_t *head, *tmp;
	int i;
	head = create_ulist();
	for(i = 0; i < 5; i++) {
		char buf[64];
		sprintf(buf, "test%d", i+1);
		if(get_name_ulist(head) == NULL)
			set_attr_ulist(head, buf, create_data("hello world"));
		else
			add_ulist(head, buf, create_data("hello world"));
	}
	/* print list */
	for(tmp = head; tmp != NULL; tmp = get_next_ulist(tmp)) {
		printf("%s: %s\n", get_name_ulist(tmp),
			(char*)get_data_ulist(tmp));
	}
	/* del at */
	del_by_name_ulist(&head, "test3", destroy_data);
	/* print list */
	for(tmp = head; tmp != NULL; tmp = get_next_ulist(tmp)) {
		printf("%s: %s\n", get_name_ulist(tmp),
			(char*)get_data_ulist(tmp));
	}
	destroy_ulist(&head, destroy_data, 1);
	return 0;
}
