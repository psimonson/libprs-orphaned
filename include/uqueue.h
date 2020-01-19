/**
 * @file uqueue.h
 * @author Philip R. Simonson
 * @date 01/17/2020
 * @brief Simple implementation of a universal queue.
 **************************************************************************
 * @details Queues are just like a stack but, you get data from the bottom
 * not the top. All the way to the top until you reach the top level.
 **************************************************************************
 */

#ifndef _PRS_UQUEUE_H_
#define _PRS_UQUEUE_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Simple universal queue data struct. */
typedef struct uqueue_data uqueue_data_t;
/** @brief Simple universal queue type. */
typedef struct uqueue uqueue_t;

/* ------------------------ Normal Functions ------------------------- */

/** @brief Create initial queue. (Run first) */
PRS_EXPORT uqueue_t *create_uqueue(void);
/** @brief Add some data to the queue. */
PRS_EXPORT int add_uqueue(uqueue_t *queue, const unsigned int id,
		const void *data);
/** @brief Destroy all data from queue. */
PRS_EXPORT void destroy_uqueue(uqueue_t **queue, void (*destroy_data)(void*));
/** @brief Get start element in queue. */
PRS_EXPORT uqueue_data_t *get_start_uqueue(uqueue_t *queue);
/** @brief Get next element in queue. */
PRS_EXPORT uqueue_data_t *get_next_uqueue(uqueue_t *queue);
/** @brief Search queue for id. */
PRS_EXPORT uqueue_data_t *search_uqueue(uqueue_t *queue, const unsigned int id);

/* ------------------------ Get/Set Functions ------------------------ */

/** @brief Get top from queue. */
PRS_EXPORT unsigned int get_top_uqueue(uqueue_t *queue);
/** @brief Get bottom from queue. */
PRS_EXPORT unsigned int get_bottom_uqueue(uqueue_t *queue);
/** @brief Get id from queue. */
PRS_EXPORT unsigned int get_id_uqueue(uqueue_data_t *data);
/** @brief Get data from queue. */
PRS_EXPORT void *get_data_uqueue(uqueue_data_t *data);

#ifdef __cplusplus
}
#endif

#endif
