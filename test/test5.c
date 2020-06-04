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
    stack = create_ustack();
    if(stack != NULL) {
        push_ustack(stack, 0, (void*)data_new("Hello World!"));
        push_ustack(stack, 1, (void*)data_new("This is a test."));
        push_ustack(stack, 2, (void*)data_new("Final test."));
        
        while(get_top_ustack(stack) > 0)
            printf("%s\n", (char*)((data_t*)pop_ustack(stack))->line);
        printf("%p: stack pointer.\n", peek_ustack(stack));
				reset_ustack(stack);
        destroy_ustack(&stack, data_free);
        return 0;
    }
    return 1;
}
