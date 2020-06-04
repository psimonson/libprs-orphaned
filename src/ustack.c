/**
 * @file ustack.h
 * @author Philip R. Simonson
 * @date 01/17/2020
 * @brief Simple implementation of a universal stack.
 **************************************************************************
 * @details Stacks are a data type for storing data to push and pop.
 * Assembler uses a stack for instance.
 **************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include "ustack.h"

/* Universal stack data structure.
 */
struct ustack_data {
	unsigned int id;
	void *data;
};
/* Universal stack structure.
 */
struct ustack {
	unsigned int top;
	unsigned int last;
	ustack_data_t *data;
};
#ifdef __cplusplus
extern "C" {
#endif
/* Create the initial stack.
 */
PRS_EXPORT ustack_t *create_ustack(void)
{
	ustack_t *stack;
	stack = (ustack_t*)malloc(sizeof(ustack_t));
	if(stack == NULL) return NULL;
	stack->top = 0;
	stack->last = 0;
	stack->data = NULL;
	return stack;
}
/* Grow the stack by one each time.
 */
static int grow_ustack(ustack_t *stack, const unsigned int id,
	const void *data)
{
	const int size = sizeof(ustack_data_t)*(stack->top+1);
	stack->data = (ustack_data_t*)realloc(stack->data, size);
	if(stack->data == NULL) return -1;
	stack->data[stack->top].data = (void*)data;
	stack->data[stack->top].id = id;
	stack->top++, stack->last++;
	return 0;
}
/* Add some data to the top of the stack.
 */
PRS_EXPORT int push_ustack(ustack_t *stack, const unsigned int id,
	const void *data)
{
	if(stack != NULL) {
		return grow_ustack(stack, id, data);
	}
	return -1;
}
/* Destroy a stack free all memory.
 */
PRS_EXPORT void destroy_ustack(ustack_t **stack, void (*destroy)(void *))
{
	if(*stack != NULL) {
		unsigned int i;
		reset_ustack(*stack);
		for(i = 0; i < (*stack)->top; i++) {
			ustack_data_t *tmp = pop_ustack(*stack);
			if(tmp != NULL) {
				if((*destroy) != NULL)
					(*destroy)(tmp->data);
				else
					free(tmp->data);
			}
		}
		free(*stack);
		*stack = NULL;
	}
}
/* Pop off the last element on the stack.
 */
PRS_EXPORT ustack_data_t *pop_ustack(ustack_t *stack)
{
	if(stack != NULL) {
		if(stack->data != NULL) {
			return &stack->data[--stack->top];
		} else {
			fprintf(stderr, "Warning: no stack data found.\n");
		}
	}
	return NULL;
}
/* Peek at last element push on the stack.
 */
PRS_EXPORT ustack_data_t *peek_ustack(ustack_t *stack)
{
	if(stack != NULL) {
		if(stack->data != NULL) {
			if(stack->top > 0)
				return &stack->data[stack->top-1];
		} else {
			fprintf(stderr, "Warning: no stack data found.\n");
		}
	}
	return NULL;
}
/* Used internally, reset top to current top.
 */
PRS_EXPORT void reset_ustack(ustack_t *stack)
{
	if(stack != NULL) {
		stack->top = stack->last;
	}
}
/* Get the top of the stack.
 */
PRS_EXPORT unsigned int get_top_ustack(ustack_t *stack)
{
	if(stack != NULL) {
		return stack->top;
	}
	return -1;
}
/* Get id from stack.
 */
PRS_EXPORT unsigned int get_id_ustack(ustack_data_t *data)
{
	if(data != NULL) {
		return data->id;
	}
	return -1;
}
/* Get data from stack.
 */
PRS_EXPORT void *get_data_ustack(ustack_data_t *data)
{
	if(data != NULL) {
		return data->data;
	}
	return NULL;
}
#ifdef __cplusplus
}
#endif
