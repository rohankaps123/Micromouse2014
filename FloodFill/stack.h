
#include <stdio.h>
#include <stdlib.h>

struct Node
{
    long *Data;
    struct Node *next;
}Node;

struct llstack{
struct Node *top;}llstack;

void popStack(struct llstack *stack);

void push(long *value, struct llstack *stack);


long* top(struct llstack *stack);
int stackIsEmpty(struct llstack *stack);
void init_stack(struct llstack *stack);

