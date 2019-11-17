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

static void *create_data(void)
{
	char *data;
	data = (char*)malloc(BUFSIZ);
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		return NULL;
	}
	return data;
}

static void destroy_data(void *data)
{
	free(data);
}

int main()
{
	ulist_t *head, *tmp;
	char *data;
	data = create_data();
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		return 1;
	}
	strcpy(data, "Hello world.");
	head = create_ulist();
	if(head == NULL) {
		perror_ulist();
		free(data);
		return 1;
	}
	set_attr_ulist(head, "test1", data);
	data = create_data();
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		destroy_ulist(&head, destroy_data, 1);
		free(data);
		return 1;
	}
	strcpy(data, "Hello again.");
	add_ulist(head, "test2", data);
	if(get_error_ulist() != ULIST_ERROR_OKAY) {
		perror_ulist();
		destroy_ulist(&head, destroy_data, 1);
		return 1;
	}
	for(tmp = head; tmp != NULL; tmp = get_next_ulist(tmp)) {
		printf("%s: %s\n",
			get_name_ulist(tmp),
			(char*)get_data_ulist(tmp));
	}
	tmp = search_ulist(head, "test1");
	if(tmp == NULL) {
		perror_ulist();
	} else {
		printf("%s: %s\n",
			get_name_ulist(tmp),
			(char*)get_data_ulist(tmp));
	}
	destroy_ulist(&head, destroy_data, 1);
	return 0;
}
