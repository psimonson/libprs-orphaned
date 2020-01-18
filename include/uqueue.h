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

/** @brief Simple universal queue data struct. */
typedef struct uqueue_data uqueue_data_t;
/** @brief Simple universal queue type. */
typedef struct uqueue uqueue_t;

/* ------------------------ Normal Functions ------------------------- */

/** @brief Create initial queue. (Run first) */
uqueue_t *create_uqueue(void);
/** @brief Add some data to the queue. */
int add_uqueue(uqueue_t *queue, const unsigned int id, const void *data);
/** @brief Destroy all data from queue. */
void destroy_uqueue(uqueue_t **queue, void (*destroy_data)(void*));
/** @brief Get start element in queue. */
uqueue_data_t *get_start_uqueue(uqueue_t *queue);
/** @brief Get next element in queue. */
uqueue_data_t *get_next_uqueue(uqueue_t *queue);
/** @brief Search queue for id. */
uqueue_data_t *search_uqueue(uqueue_t *queue, const unsigned int id);

/* ------------------------ Get/Set Functions ------------------------ */

/** @brief Get top from queue. */
unsigned int get_top_uqueue(uqueue_t *queue);
/** @brief Get bottom from queue. */
unsigned int get_bottom_uqueue(uqueue_t *queue);
/** @brief Get id from queue. */
unsigned int get_id_uqueue(uqueue_data_t *data);
/** @brief Get data from queue. */
void *get_data_uqueue(uqueue_data_t *data);

#endif
