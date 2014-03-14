#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void popStack(struct llstack *stack)
{
    struct Node *var = stack->top;
    if(var->next!=NULL)
    {
        stack->top = stack->top->next; 
        free(var);
    }
    else if(var!=NULL&&var->next==NULL){
    	stack->top=NULL;
    }	
}

void push(long value, struct llstack *stack)
{
    struct Node *temp;
    temp = (struct Node *)malloc(sizeof(struct Node));
    temp->Data=value;
    if (stack->top == NULL)
    {
         stack->top=temp;
         stack->top->next=NULL;
    }
    else
    {
        temp->next=stack->top;
        stack->top=temp;
    }
}


long top(struct llstack *stack)
{
     struct Node *var=stack->top;
     if(var!=NULL)
      return var->Data;
     else
      return -1;
}

int stackIsEmpty(struct llstack *stack){
	if (stack->top==NULL)
	return 1;
	else 
	return 0;
}
void init_stack(struct llstack *stack)
{
	stack->top = NULL;
}