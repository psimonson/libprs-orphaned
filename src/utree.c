/**
 * @file utree.c
 * @author Philip R. Simonson
 * @date 01/17/2020
 * @brief Simple implementation of a universal tree.
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utree.h"

/* ----------------- Universal Binary Tree Code Below ---------------- */

enum { UTREE_LEFT_LIMB, UTREE_RIGHT_LIMB }; /**< utree limbs */

struct utree {
	int id;			/**< ID for tree limb */
	void *data;		/**< Data for tree limb */
	struct utree *left;	/**< Left limb */
	struct utree *right;	/**< Right limb */
};
#ifdef __cplusplus
extern "C" {
#endif
/* Add a limb to the universal tree.
 */
PRS_EXPORT void add_utree(utree_t **leaf, int id, const void *data)
{
	if(*leaf == NULL) {
		*leaf = (utree_t*)malloc(sizeof(utree_t));
		if(!(*leaf)) return;
		(*leaf)->id = id;
		(*leaf)->data = (void*)data;
		(*leaf)->left = NULL;
		(*leaf)->right = NULL;
	} else if(id < (*leaf)->id) {
		add_utree(&(*leaf)->left, id, data);
	} else if(id > (*leaf)->id) {
		add_utree(&(*leaf)->right, id, data);
	}
}
/* Destroy a universal tree.
 */
PRS_EXPORT void destroy_utree(utree_t **leaf, void (*destroy)(void *))
{
	if(*leaf != NULL) {
		destroy_utree(&(*leaf)->left, (*destroy));
		destroy_utree(&(*leaf)->right, (*destroy));
		if((*destroy) != NULL) (*destroy)((*leaf)->data);
		free((*leaf));
		(*leaf) = 0;
	}
}
/* Print ascii utree to console.
 */
PRS_EXPORT void print_utree(utree_t *leaf, int indent)
{
	if(leaf != NULL) {
		printf("%*d\n", indent, leaf->id);
		print_utree(leaf->left, indent+4);
		print_utree(leaf->right, indent+4);
	}
}
/* Search utree to find specific id.
 */
PRS_EXPORT utree_t *search_utree(utree_t *leaf, int id)
{
	if(leaf != NULL) {
		if(id == leaf->id)
			return leaf;
		else if(id < leaf->id)
			search_utree(leaf->left, id);
		else
			search_utree(leaf->right, id);
	} else {
		return NULL;
	}
}
/* Get the data from a branch.
 */
PRS_EXPORT void *get_data_utree(utree_t *leaf)
{
	if(leaf != NULL) {
		return leaf->data;
	}
	return NULL;
}
/* Get the id from a branch.
 */
PRS_EXPORT int get_id_utree(utree_t *leaf)
{
	if(leaf != NULL) {
		return leaf->id;
	}
	return -1;
}
/* Get the left branch of a leaf.
 */
PRS_EXPORT utree_t *get_left_utree(utree_t *leaf)
{
	if(leaf != NULL) {
		return leaf->left;
	}
	return NULL;
}
/* Get the right branch of a leaf.
 */
PRS_EXPORT utree_t *get_right_utree(utree_t *leaf)
{
	if(leaf != NULL) {
		return leaf->right;
	}
	return NULL;
}
#ifdef __cplusplus
}
#endif
