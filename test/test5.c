#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustack.h"

typedef struct data {
    char *line;
    int slen;
} data_t;

data_t*
data_new (const char *text)
{
    data_t *data;
    data = (data_t*)malloc(sizeof(data_t));
    if(data != NULL) {
        data->line = strdup(text);
        data->slen = strlen(text);
    }
    return data;
}

void
data_free (void *data)
{
    data_t *p = (data_t*)data;
    free(p->line);
    p->slen = 0;
    free(p);
}

int
main ()
{
    ustack_t *stack;
    stack = init_stack();
    if(stack != NULL) {
        push_stack(stack, (void*)data_new("Hello World!"));
        push_stack(stack, (void*)data_new("This is a test."));
        push_stack(stack, (void*)data_new("Final test."));
        
        while(stack->top > 0)
            printf("%s\n", (char*)((data_t*)pop_stack(stack))->line);
        printf("%p: stack pointer.\n", peek_stack(stack));
        free_stack(stack, data_free);
        return 0;
    }
    return 1;
}
