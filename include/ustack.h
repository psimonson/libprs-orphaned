/*
 * ustack.h - Simple stack data structure. Please see comment header
 *            inside of the main source file "ustack.c".
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/05/24
 **********************************************************************
 */

#define PRS_USTACK_H

#define STACKSIZE 255

typedef struct ustack {
    void* data;
    int size;
    int top;
    int cur;
} ustack_t;

#ifdef __cplusplus
extern "C" {
#endif

ustack_t* init_stack();
void push_stack(ustack_t* stack, void* data);
void* pop_stack(ustack_t* stack);
void* peek_stack(ustack_t* stack);
void reset_top(ustack_t* stack);
void free_stack(ustack_t* stack, void (*)(void*));

#ifdef __cplusplus
}
#endif