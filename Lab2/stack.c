#include "stack.h"

#include <stdlib.h>

Stack stack_create(void) {
    Stack stack = {.top = NULL, .size = 0};
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack == NULL)
        return;
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
    stack->size = 0;
}

bool stack_push(Stack *stack, long value) {
    if (stack == NULL)
        return false;

    StackNode *node = malloc(sizeof *node);
    if (node == NULL)
        return false;

    node->value = value;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    return true;
}

bool stack_pop(Stack *stack, long *out) {
    if (stack == NULL || stack->top == NULL)
        return false;

    StackNode *temp = stack->top;
    if (out != NULL) {
        *out = temp->value;
    }
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return true;
}

bool stack_peek(const Stack *stack, long *out) {
    if (stack == NULL || stack->top == NULL)
        return false;

    if (out != NULL) {
        *out = stack->top->value;
    }
    return true;
}

bool stack_is_empty(const Stack *stack) {
    return stack == NULL || stack->top == NULL;
}

size_t stack_size(const Stack *stack) {
    return stack == NULL ? 0 : stack->size;
}

void stack_print(const Stack *stack, FILE *out) {
    if (stack->size == 0) {
        fprintf(out, "[]");
        return;
    }

    long *values = malloc(stack->size * sizeof *values);
    if (values == NULL) {
        fprintf(out, "[print unavailable: out of memory]");
        return;
    }

    size_t i = stack->size;
    for (const StackNode *node = stack->top; node != NULL; node = node->next) {
        values[--i] = node->value;
    }

    fputc('[', out);
    for (i = 0; i < stack->size; i++) {
        if (i > 0)
            fprintf(out, ", ");
        fprintf(out, "%ld", values[i]);
    }
    fputc(']', out);
    free(values);
}

bool stack_check_invariant(const Stack *stack) {
    if (stack == NULL)
        return false;

    size_t count = 0;
    for (const StackNode *curr = stack->top; curr != NULL; curr = curr->next) {
        count++;
    }

    bool rule1 = (count == stack->size);
    bool rule2 = (stack->top == NULL) == (stack->size == 0);

    return rule1 && rule2;
}