#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int Data;
    struct Node *next;
}

struct llstack{
struct Node *top}

void popStack(struct llstack *stack);

void push(int value, struct llstack *stack);


int top(struct llstack *stack);

int init_stack(struct llstack *stack);

