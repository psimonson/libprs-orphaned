/**
 * @file ustack.c
 * @author Philip R. Simonson
 * @date 24 May 2019
 * @brief Simple universal stack implementation.
 *
 * Main source file for universal stack. Fully working stack that
 * all you need to do is create your own functions for data
 * handling. Also, a struct for the data if you need more than one
 * data type inside of one stack. This code remains the same for
 * everything. Or atleast that's how my design should be, let me
 * know if something don't let you add your data to this stack and
 * I'll fix it.
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
 */

#include <stdio.h>
#include <stdlib.h>
#include "ustack.h"

/**
 * @brief Initialize the a stack object.
 *
 * Returns: ustack_t*
 */
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

/**
 * @brief Grow a stack object to add more objects.
 *
 * Returns: ustack_t*
 */
ustack_t*
grow_stack (ustack_t* stack)
{
    ustack_t* nstack;
    
    stack->size += STACKSIZE;
    nstack = (ustack_t*)realloc(stack, stack->size);
    if(!nstack) return stack;
    return nstack;
}

/**
 * @brief Push some data on the top of the given stack pointer.
 *
 * Returns: void
 */
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

/**
 * @brief Pop top of stack off and return it.
 *
 * Returns: void*
 */
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

/**
 * @brief Peek at the top of stack and return it.
 *
 * Returns: void*
 */
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

/**
 * @brief Reset the top of the stack.
 *
 * Returns: void
 */
void
reset_top (ustack_t* stack)
{
    stack->top = stack->cur > 0 ? stack->cur : 0;
}

/**
 * @brief Free the entire stack object given.
 *
 * Returns: void
 */
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
