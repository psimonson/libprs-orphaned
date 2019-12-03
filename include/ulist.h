/**
 * @file ulist.h
 * @author Philip R. Simonson
 * @date 11/10/2019
 * @brief Simple universal dynamic linked list.
 ********************************************************************
 * @details
 *
 * Simple universal dynamic linked list.
 */

#ifndef _ULIST_H_
#define _ULIST_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ULIST Errors - Simple status errors.
 */
enum ULIST_ERROR {
	ULIST_ERROR_OKAY,	/**< No error. */
	ULIST_ERROR_ALLOC,	/**< Memory allocation error. */
	ULIST_ERROR_NAME,	/**< Memory alloc error for name in attr. */
	ULIST_ERROR_SEARCH,	/**< Error searching (not found). */
	ULIST_ERROR_DELAT,	/**< Delete at (list empty or invalid). */
	ULIST_ERROR_EMPTY	/**< List is empty. */
};

/**
 * @brief ulist_t type definition.
 */
typedef struct ulist ulist_t;

/** @brief Create a new ulist_t pointer. */
ulist_t *create_ulist(void);
/** @brief Set attributes for new node. */
void set_attr_ulist(ulist_t *node, const char *name, void *data);
/** @brief Destroys a ulist_t pointer. */
void destroy_ulist(ulist_t **list, void (*func)(void *data),
	unsigned char bdata);

/** @brief Searches for the given list item name. */
ulist_t *search_ulist(const ulist_t *head, const char *name);
/** @brief Add a new item to the list. */
void add_ulist(ulist_t *head, const char *name, void *data);
/** @brief Delete an item from the list. */
void del_ulist(ulist_t **head, void (*func)(void *data));
/** @brief Delete an item by it's name. */
void del_by_name_ulist(ulist_t **head, const char *name,
	void (*func)(void *data));

/** @brief Gets data from the given ulist_t item. */
void *get_data_ulist(const ulist_t *node);
/** @brief Gets the name from a given ulist_t item. */
const char *get_name_ulist(const ulist_t *node);
/** @brief Gets the previous item in the list. */
ulist_t *get_prev_ulist(const ulist_t *head, const ulist_t *node);
/** @brief Gets the next item in the list, if any. */
ulist_t *get_next_ulist(ulist_t *node);

/** @brief Gets the error string associated with the ulist error code. */
const char *strerror_ulist(int errno);
/** @brief Get the error code from the last list function ran. */
int get_error_ulist(void);

#define get_error_number ((int)get_error_ulist()) /**< get error number */

/** @brief Print error with file name and line number. */
#define perror_ulist() fprintf(stderr, "[%s:%d] ULIST: %s\n", \
		__FILE__, __LINE__, strerror(get_error_number));

#ifdef __cplusplus
}
#endif

#endif
