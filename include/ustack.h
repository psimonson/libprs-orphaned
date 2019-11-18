/**
 * @file ustack.h
 * @author Philip R. Simonson
 * @date   2019/05/24
 * @brief Dynamic Universal Stack, make a stack that holds anything.
 **********************************************************************
 * @details
 *
 * Make a simple dynamic stack, handling everything from this library
 * with anything to do with the stack.
 **********************************************************************
 */

#ifndef PRS_USTACK_H
#define PRS_USTACK_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STACKSIZE 255

/** @brief Universal stack structure. */
typedef struct ustack {
    void* data;
    int size;
    int top;
    int cur;
} ustack_t;

PRS_EXPORT ustack_t* init_stack();
PRS_EXPORT void push_stack(ustack_t* stack, void* data);
PRS_EXPORT void* pop_stack(ustack_t* stack);
PRS_EXPORT void* peek_stack(ustack_t* stack);
PRS_EXPORT void reset_top(ustack_t* stack);
PRS_EXPORT void free_stack(ustack_t* stack, void (*)(void*));

#ifdef __cplusplus
}
#endif

#endif
