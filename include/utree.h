/**
 * @file utree.h
 * @author Philip R. Simonson
 * @date 01/17/2019
 * @brief Simple implementation of a unversal tree.
 **********************************************************************
 * @details This is a unversal tree implementation, it will accept any
 * data uses a void pointer. It's left up to the user what type of data
 * also, creation and deletion of the data. You have to implement one
 * function at least. Name of function can be anything. Function
 * prototype as follows...
 *
 * static void destroy_data(void *data)
 * {
 * 	// TODO: Free all allocated memory for your data struct or
 * 	// data variables.
 * }
 **********************************************************************
 */

#ifndef _PRS_UTREE_H_
#define _PRS_UTREE_H_

#include "export.h"

/** @brief Universal tree type */
typedef struct utree utree_t;

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------ Normal Functions ----------------------- */

/** @brief Add a branch to the tree. */
PRS_EXPORT void add_utree(utree_t **tree, int id, const void *data);
/** @brief Searches tree for id. */
PRS_EXPORT utree_t *search_utree(utree_t *leaf, int id);
/** @brief Prints the tree in ascii form on screen. */
PRS_EXPORT void print_utree(utree_t *tree, int indent);
/** @brief Frees all allocated memory from tree. */
PRS_EXPORT void destroy_utree(utree_t **tree, void (*destroy_data)(void*));

/* ------------------------ Get/Set Functions ---------------------- */

/** @brief Gets the data from the tree. */
PRS_EXPORT void *get_data_utree(utree_t *tree);
/** @brief Gets the id from the tree. */
PRS_EXPORT int get_id_utree(utree_t *leaf);
/** @brief Gets the left limb of the tree. */
PRS_EXPORT utree_t *get_left_utree(utree_t *leaf);
/** @brief Gets the right limb of the tree. */
PRS_EXPORT utree_t *get_right_utree(utree_t *leaf);

#ifdef __cplusplus
}
#endif

#endif
