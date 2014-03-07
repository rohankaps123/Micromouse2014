#include<stdio.h>
#include<stdlib.h>
 
struct node{
	int data;
	struct node *link;
};
 
struct my_stack{
	struct node *head;
	struct node *list_node;
};
 
int push(struct my_stack *stack,int e)
{
	stack->list_node = malloc(sizeof(struct node));
	if(stack->list_node == NULL)
	{
		puts("Error : Cannot Allocate Memory.");
		exit(1);
	}
	stack->list_node->data = e;
	stack->list_node->link = stack->head;
	stack->head= stack->list_node;
	return 0;
}
 
int pop(struct my_stack *stack){
	if(stack->head==NULL){
		puts("Error : Stack is Empty.");
		exit(1);
	}
	int tmp = stack->head->data;
	stack->list_node = stack->head;
	stack->head = stack->head->link;
	free(stack->list_node);
	return tmp;
}
 
int init_stack(struct my_stack *stack)
{
	stack->head = NULL;
	stack->list_node = NULL;
}
 
int main(){
	struct my_stack test;
	init_stack(&test);
 
	int i;
	for (i =1; i<=10; i++)
		push(&test, i);
 
	for (i =1; i<=10; i++)
		printf("%d\n", pop(&test));
 
	return 0;
}
