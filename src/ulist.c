/**
 * @file ulist.c
 * @author Philip R. Simonson
 * @date 11/10/2019
 * @details
 *
 * Simple universal dynamic linked list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ulist.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ulist {
	void *data;
	char *name;
	ulist_t *next;
};

int _errno_ulist;

char *pstrdup(const char *s)
{
	char *r;
	if(s == NULL) return NULL;
	r = (char*)malloc(strlen(s)+1);
	if(r == NULL) return NULL;
	strcpy(r, s);
	return r;
}

PRS_EXPORT ulist_t *create_ulist(void)
{
	ulist_t *head;
	head = malloc(sizeof(ulist_t));
	if(head == NULL) {
		_errno_ulist = ULIST_ERROR_ALLOC;
		return NULL;
	}
	head->name = NULL;
	head->data = NULL;
	head->next = NULL;
	_errno_ulist = ULIST_ERROR_OKAY;
	return head;
}

PRS_EXPORT void set_attr_ulist(ulist_t *node, const char *name, void *data)
{
	if(node == NULL || name == NULL || data == NULL) return;
	node->name = (char*)malloc(strlen(name)+1);
	if(node->name == NULL) {
		_errno_ulist = ULIST_ERROR_NAME;
		return;
	}
	strcpy(node->name, name);
	node->data = data;
}

PRS_EXPORT void destroy_ulist(ulist_t **head, void (*func)(void *data),
	unsigned char bdata)
{
	ulist_t *tmp;
	for(tmp = *head; tmp->next != NULL; ) {
		ulist_t *next = tmp->next;
		if((*func) != NULL && bdata) (*func)(tmp->data);
		if(tmp->name != NULL) free(tmp->name);
		if(tmp != NULL) free(tmp);
		tmp = next;
	}
	*head = NULL;
}

static void rebuild_ulist(ulist_t **head, void (*func)(void *data))
{
	ulist_t *new;
	new = create_ulist();
	if(new != NULL) {
		ulist_t *tmp;
		for(tmp = *head; tmp != NULL; tmp = tmp->next) {
			if(new->name == NULL) {
				set_attr_ulist(new, pstrdup(tmp->name),
					tmp->data);
			} else {
				if(tmp != NULL) {
					add_ulist(new, pstrdup(tmp->name),
						tmp->data);
				}
			}
		}
		destroy_ulist(head, func, 0);
		*head = new;
	}
}

PRS_EXPORT ulist_t *search_ulist(const ulist_t *head, const char *name)
{
	const ulist_t *tmp;
	if(head == NULL || name == NULL) return NULL;
	for(tmp = head; tmp != NULL; tmp = tmp->next) {
		if(strcmp(tmp->name, name) == 0)
			return (ulist_t*)tmp;
	}
	_errno_ulist = ULIST_ERROR_SEARCH;
	return NULL;
}

PRS_EXPORT void add_ulist(ulist_t *head, const char *name, void *data)
{
	ulist_t *tmp;
	if((head == NULL) | (name == NULL) | (data == NULL)) return;
	for(tmp = head; tmp->next != NULL; tmp = tmp->next);
	tmp->next = create_ulist();
	if(tmp->next == NULL) {
		_errno_ulist = ULIST_ERROR_ALLOC;
		return;
	}
	set_attr_ulist(tmp->next, name, data);
	tmp->next->next = NULL;
	_errno_ulist = ULIST_ERROR_OKAY;
}

PRS_EXPORT void del_ulist(ulist_t **head, void (*func)(void *data))
{
	ulist_t *tmp;
	if((*head)->next != NULL) {
		ulist_t *prev;
		for(tmp = *head; tmp->next != NULL; tmp = tmp->next);
		prev = get_prev_ulist(*head, tmp);
		if((*func) != NULL) (*func)(tmp->data);
		free(tmp->name);
		free(tmp);
		prev->next = NULL;
	} else if((*head) != NULL) {
		if((*func) != NULL) (*func)((*head)->data);
		free((*head)->name);
		free((*head));
		*head = NULL;
	} else {
		_errno_ulist = ULIST_ERROR_EMPTY;
		*head = NULL;
	}
}

PRS_EXPORT void del_by_name_ulist(ulist_t **head, const char *name,
	void (*func)(void *data))
{
	ulist_t *tmp;
	if(head == NULL || name == NULL || (*func) == NULL) return;
	tmp = search_ulist(*head, name);
	if(tmp != NULL) {
		ulist_t *prev, *next;
		prev = get_prev_ulist(*head, tmp);
		next = get_next_ulist(tmp);
		if(prev != NULL) {
			if((*func) != NULL) (*func)(tmp->data);
			free(tmp->name);
			free(tmp);
			prev->next = next;
			rebuild_ulist(head, func);
		} else {
			_errno_ulist = ULIST_ERROR_DELAT;
		}
	}
}

PRS_EXPORT void *get_data_ulist(const ulist_t *elem)
{
	return (void*)elem->data;
}

PRS_EXPORT const char *get_name_ulist(const ulist_t *elem)
{
	return (const char*)elem->name;
}

PRS_EXPORT ulist_t *get_prev_ulist(const ulist_t *head, const ulist_t *node)
{
	const ulist_t *tmp;
	if(head == NULL || node == NULL) return NULL;
	for(tmp = head; tmp->next != NULL; tmp = tmp->next)
		if(tmp->next == node && tmp != NULL)
			return (ulist_t*)(tmp == NULL ? NULL : tmp);
	return NULL;
}

PRS_EXPORT ulist_t *get_next_ulist(ulist_t *node)
{
	return (ulist_t*)(node == NULL ? NULL : node->next);
}

PRS_EXPORT const char *strerror_ulist(int errno)
{
	const char *errors[] = {
		"No error.",
		"Memory allocation error.",
		"Cannot allocate memory for name.",
		"No results found for search.",
		"List is empty or NULL pointer given.",
		"List empty (blank list)."
	};
	return errors[errno];
}

PRS_EXPORT int get_error_ulist(void)
{
	int errno = _errno_ulist;
	_errno_ulist = 0;
	return errno;
}

#ifdef __cplusplus
}
#endif
