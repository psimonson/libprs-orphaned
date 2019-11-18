#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustack.h"

void
free_data (void* data)
{
    free(data);
}

int
main ()
{
    ustack_t* stack;
    char *str1,*str2,*str3;
    
    /* allocate strings */
    str1 = (char*)malloc(sizeof(char)*64);
    if(str1 == NULL) return 1;
    str2 = (char*)malloc(sizeof(char)*64);
    if(str2 == NULL) { free(str1); return 1; }
    str3 = (char*)malloc(sizeof(char)*64);
    if(str3 == NULL) { free(str1); free(str2); return 1; }

    /* fill strings */
    strcpy(str1, "Hello world!");
    strcpy(str2, "This is a test.");
    strcpy(str3, "This is another test.");
    
    /* initialize and push to stack */
    stack = init_stack();
    if(stack == NULL) { free(str1); free(str2); free(str3); return 2; }
    push_stack(stack, str1);
    push_stack(stack, str2);
    push_stack(stack, str3);
    while(stack->top > 0)
        printf("String: %s\n", (char*)pop_stack(stack));
    reset_top(stack);
    free_stack(stack, free_data);
    return 0;
}
