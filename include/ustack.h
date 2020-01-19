/**
 * @file ustack.h
 * @author Philip R. Simonson
 * @date   2019/05/24
 * @brief Dynamic Universal Stack, make a stack that holds anything.
 **********************************************************************
 * @details Make a simple dynamic stack, handling everything from this
 * library with anything to do with the stack.
 **********************************************************************
 */

#ifndef PRS_USTACK_H
#define PRS_USTACK_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Simple universal stack data struct. */
typedef struct ustack_data ustack_data_t;
/** @brief Simple universal stack type. */
typedef struct ustack ustack_t;

/* ------------------------ Normal Functions ------------------------- */

/** @brief Create initial stack. (Run first) */
PRS_EXPORT ustack_t *create_ustack(void);
/** @brief Add some data to the stack. */
PRS_EXPORT int push_ustack(ustack_t *stack, const unsigned int id,
		const void *data);
/** @brief Destroy all data from stack. */
PRS_EXPORT void destroy_ustack(ustack_t **stack, void (*destroy_data)(void*));
/** @brief Pop off top of stack. */
PRS_EXPORT ustack_data_t *pop_ustack(ustack_t *stack);
/** @brief Peek at top of stack. */
PRS_EXPORT ustack_data_t *peek_ustack(ustack_t *stack);
/** @brief Reset top to last element on stack. */
PRS_EXPORT void reset_ustack(ustack_t *stack);

/* ------------------------ Get/Set Functions ------------------------ */

/** @brief Get top from stack. */
PRS_EXPORT unsigned int get_top_ustack(ustack_t *stack);
/** @brief Get id from stack. */
PRS_EXPORT unsigned int get_id_ustack(ustack_data_t *data);
/** @brief Get data from stack. */
PRS_EXPORT void *get_data_ustack(ustack_data_t *data);

#ifdef __cplusplus
}
#endif

#endif
