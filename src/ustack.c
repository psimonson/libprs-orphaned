/*
 * ustack.c - Main source file for universal stack. Fully working stack that
 *            all you need to do is create your own functions for data
 *            handling. Also, a struct for the data if you need more than one
 *            data type inside of one stack. This code remains the same for
 *            everything. Or atleast that's how my design should be, let me
 *            know if something don't let you add your data to this stack and
 *            I'll fix it.
 ******************************************************************************
 * Functions:
 ******************************************************************************
 * init_stack() - create initial stack.
 * push_stack() - push element on top of stack.
 * pop_stack()  - pop element off top of stack.
 * peek_stack() - peek at top of stack.
 * reset_top()  - reset top of stack to last element.
 * free_stack() - free all memory and cleanup.
 ******************************************************************************
 * Author   : Philip R. Simonson
 * Date     : 2019/05/24
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include "ustack.h"

ustack_t*
init_stack ()
{
    ustack_t *stack;
    stack = (ustack_t*)malloc(sizeof(ustack_t)*STACKSIZE);
    if(!stack) return NULL;
    stack->data = NULL;
    stack->size = STACKSIZE;
    stack->top = 0;
    stack->cur = 0;
    return stack;
}

ustack_t*
grow_stack (ustack_t* stack)
{
    ustack_t* nstack;
    
    stack->size += STACKSIZE;
    nstack = (ustack_t*)realloc(stack, stack->size);
    if(!nstack) return stack;
    return nstack;
}

void
push_stack (ustack_t* stack, void* data)
{
    if(stack->top >= stack->size) {
        fprintf(stderr, "Warning: Stack to full, growing.\n");
        stack = grow_stack(stack);
    }
    stack[stack->top++].data = data;
    stack->cur++;
}

void*
pop_stack (ustack_t* stack)
{
    if(stack->top > 0) {
        return stack[--stack->top].data;
    } else {
        fprintf(stderr, "Warning: Stack empty.\n");
    }
    return NULL;
}

void*
peek_stack (ustack_t* stack)
{
    if(stack->top > 0) {
        return stack[stack->top-1].data;
    } else {
        fprintf(stderr, "Warning: Stack empty.\n");
    }
    return NULL;
}

void
reset_top (ustack_t* stack)
{
    stack->top = stack->cur > 0 ? stack->cur : 0;
}

void
free_stack (ustack_t* stack, void (*func)(void*))
{
    if(func != NULL) {
        stack->top = stack->cur;
        while(stack->top > 0) {
            void* data = pop_stack(stack);
            if(data == NULL) break;
            (*func)(data);
        }
        free(stack[stack->top].data);
        stack->cur = 0;
    }
    free(stack);
}
