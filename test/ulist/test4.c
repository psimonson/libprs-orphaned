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

struct data {
	char data[BUFSIZ];
	int len;
};

static void *create_data(void)
{
	struct data *data;
	data = (struct data*)malloc(sizeof(struct data));
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		return NULL;
	}
	memset(data->data, 0, sizeof(char)*BUFSIZ);
	data->len = 0;
	return data;
}

static void destroy_data(void *data)
{
	free(data);
}

static void set_data(struct data *data, const char *s)
{
	memcpy(data->data, s, strlen(s)+1);
	data->len = strlen(s);
}

int main()
{
	ulist_t *head, *tmp;
	struct data *data;
	data = create_data();
	if(data == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory.\n");
		return 1;
	}
	set_data(data, "This is test one element");
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
	set_data(data, "This is test two element");
	add_ulist(head, "test2", data);
	if(get_error_ulist() != ULIST_ERROR_OKAY) {
		perror_ulist();
		destroy_ulist(&head, destroy_data, 1);
		return 1;
	}
	for(tmp = head; tmp != NULL; tmp = get_next_ulist(tmp)) {
		printf("[Name/Length] %s:%d = %s\n",
			get_name_ulist(tmp),
			((struct data*)get_data_ulist(tmp))->len,
			((struct data*)get_data_ulist(tmp))->data);
	}
	tmp = search_ulist(head, "test2");
	if(tmp == NULL) {
		perror_ulist();
	} else {
		tmp = get_prev_ulist(head, tmp);
		if(tmp != NULL) {
			printf("(Previous Node) : [Name/Length] %s:%d = %s\n",
				get_name_ulist(tmp),
				((struct data*)get_data_ulist(tmp))->len,
				((struct data*)get_data_ulist(tmp))->data);
		}
	}
	destroy_ulist(&head, destroy_data, 1);
	return 0;
}
