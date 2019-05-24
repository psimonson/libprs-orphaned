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

typedef struct data {
    char *data;
    int slen;
} data_t;

typedef struct ustack {
    void* data;
    int size;
    int top;
} ustack_t;

#ifdef __cplusplus
extern "C" {
#endif

ustack_t* init_stack();
void push_stack(ustack_t* stack, void* data);
void* pop_stack(ustack_t* stack);
void* peek_stack(ustack_t* stack);
void free_stack(ustack_t* stack, void (*)(void*));

#define FREE_DATA(data) free_data(data);

#ifdef __cplusplus
}
#endif
