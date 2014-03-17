#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    long Data;
    struct Node* next;
}Node;

typedef struct llstack
{
	int pos;
	//struct Node* top;
	long nodes[125];
}llstack;

void popStack(struct llstack *stack);

void push(long value, struct llstack *stack);

long top(struct llstack *stack);
int stackIsEmpty(struct llstack *stack);
void init_stack(struct llstack *stack);

