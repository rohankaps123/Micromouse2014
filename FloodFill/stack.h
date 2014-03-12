#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    volatile int Data;
    volatile struct Node* next;
}Node;

typedef struct llstack
{
	volatile struct Node* top;
} llstack;

void popStack(struct llstack *stack);

void push(int value, struct llstack *stack);

int top(struct llstack *stack);
int stackIsEmpty(struct llstack *stack);
void init_stack(struct llstack *stack);

