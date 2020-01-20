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

#include <stdio.h>
#include <stdlib.h>
#include "uqueue.h"

/* Universal queue data structure.
 */
struct uqueue_data {
	unsigned int id;
	void *data;
};
/* Universal queue structure.
 */
struct uqueue {
	unsigned int top;
	unsigned int bottom;
	uqueue_data_t *data;
};
#ifdef __cplusplus
extern "C" {
#endif
/* Create the initial queue.
 */
PRS_EXPORT uqueue_t *create_uqueue(void)
{
	uqueue_t *queue;
	queue = (uqueue_t*)malloc(sizeof(uqueue_t));
	if(queue == NULL) return NULL;
	queue->top = 0;
	queue->bottom = 0;
	queue->data = NULL;
	return queue;
}
/* Grow the queue by one each time.
 */
static int grow_uqueue(uqueue_t *queue, const unsigned int id,
	const void *data)
{
	const int size = sizeof(uqueue_data_t)*(queue->top+1);
	queue->data = (uqueue_data_t*)realloc(queue->data, size);
	if(queue->data == NULL) return -1;
	queue->data[queue->top].data = (void*)data;
	queue->data[queue->top].id = id;
	queue->top++;
	return 0;
}
/* Add some data to the top of the queue.
 */
PRS_EXPORT int add_uqueue(uqueue_t *queue, const unsigned int id,
	const void *data)
{
	if(queue != NULL) {
		return grow_uqueue(queue, id, data);
	}
	return -1;
}
/* Destroy a queue free all memory.
 */
PRS_EXPORT void destroy_uqueue(uqueue_t **queue, void (*destroy)(void *))
{
	if(*queue != NULL) {
		unsigned int i;
		for(i = 0; i < (*queue)->top; i++) {
			if((*destroy) != NULL)
				(*destroy)((*queue)->data[i].data);
			else
				free((*queue)->data[i].data);
		}
		free(*queue);
		*queue = NULL;
	}
}
/* Get first element of data from the bottom of the queue.
 */
PRS_EXPORT uqueue_data_t *get_start_uqueue(uqueue_t *queue)
{
	if(queue != NULL) {
		if(queue->data != NULL) {
			queue->bottom = 0;
			return &queue->data[queue->bottom];
		} else {
			fprintf(stderr, "Warning: no queue data found.\n");
		}
	}
	return NULL;
}
/* Get data from bottom of the queue.
 */
PRS_EXPORT uqueue_data_t *get_next_uqueue(uqueue_t *queue)
{
	if(queue != NULL) {
		if(queue->data != NULL) {
			if(queue->bottom >= queue->top-1)
				queue->bottom = 0;
			else
				return &queue->data[++queue->bottom];
		} else {
			fprintf(stderr, "Warning: no queue data found.\n");
		}
	}
	return NULL;
}
/* Search uqueue from bottom to top.
 */
PRS_EXPORT uqueue_data_t *search_uqueue(uqueue_t *queue, const unsigned int id)
{
	if(queue != NULL) {
		uqueue_data_t *tmp;
		for(tmp = get_start_uqueue(queue); tmp != NULL;
				tmp = get_next_uqueue(queue)) {
			if(id == tmp->id)
				return tmp;
		}
	}
	return NULL;
}
/* Get top of queue.
 */
PRS_EXPORT unsigned int get_top_uqueue(uqueue_t *queue)
{
	return queue->top;
}
/* Get bottom of queue.
 */
PRS_EXPORT unsigned int get_bottom_uqueue(uqueue_t *queue)
{
	return queue->bottom;
}
/* Get id from queue.
 */
PRS_EXPORT unsigned int get_id_uqueue(uqueue_data_t *data)
{
	if(data != NULL) {
		return data->id;
	}
	return -1;
}
/* Get data from queue.
 */
PRS_EXPORT void *get_data_uqueue(uqueue_data_t *data)
{
	if(data != NULL) {
		return data->data;
	}
	return NULL;
}
#ifdef __cplusplus
}
#endif
