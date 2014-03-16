#include <stdio.h>
#include <stdlib.h>
#include "FloodFill_Stack.h"



void popStack(struct llstack *stack)
{
	if(stack->pos < 0)
	{
		stack->nodes[0] = -1;
		stack->pos = 0;
	}
	else
	{
		stack->nodes[stack->pos] = -1;
		stack->pos--;
	}
/*     struct Node *var = stack->top;
    if(var->next!=NULL)
    {
        stack->top = stack->top->next; 
        free(var);
    }
    else if(var!=NULL&&var->next==NULL){
    	stack->top=NULL;
    }	
 */}

void push(long value, struct llstack *stack)
{
	stack->pos++;
	stack->nodes[stack->pos] = value;
/*     struct Node *temp;
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
    } */
}


long top(struct llstack *stack)
{
	if(stack->pos >= 0)
		return stack->nodes[stack->pos];
	else
		return -1;
     /* struct Node *var=stack->top;
     if(var!=NULL)
      return var->Data;
     else
      return -1; */
}

int stackIsEmpty(struct llstack *stack){
	if(stack->pos == -1)
		return 1;
	else
		return 0;
	/* if (stack->top==NULL)
	return 1;
	else 
	return 0; */
}
void init_stack(struct llstack *stack)
{
	for(int i = 0; i < 125; i++)
	{
		stack->nodes[i] = -1;
	}
	stack->pos = -1;
	//stack->top = NULL;
}