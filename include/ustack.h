/*
 * ustack.h - Simple stack data structure. Please see comment header
 *            inside of the main source file "ustack.c".
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/05/24
 **********************************************************************
 */

#define PRS_USTACK_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STACKSIZE 255

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
