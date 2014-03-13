#include floodfill_c/Floodfill/floofill_move.c
#include floodfill_c/floodfill/stack.h





void floodfill(int mazecells[][16], char a){
int level=0;
Struct llstack stack1;
Struct llstack stack2;
init_stack(stack1);
init_stack(stack2);
	if(a=='C'){
	//	setValue(mazecells[MAZEHEIGHT/2][MAZEWIDTH/2],0);
		push(&stack1,&mazecells[MAZEHEIGHT/2][MAZEWIDTH/2],0)
		//setValue(mazecells[MAZEHEIGHT/2+1][MAZEWIDTH/2],0);
		push(&stack1,&mazecells[MAZEHEIGHT/2+1][MAZEWIDTH/2],0)
		//setValue(mazecells[MAZEHEIGHT/2][MAZEWIDTH/2+1],0);
		push(&stack1,&mazecells[MAZEHEIGHT/2][MAZEWIDTH/2+1],0)
		//setValue(mazecells[MAZEHEIGHT/2+1][MAZEWIDTH/2+1],0);
		push(&stack1,&mazecells[MAZEHEIGHT/2+1][MAZEWIDTH/2+1],0)
	}
	else 
		//{setValue(mazecells[0][MAZEHEIGHT]);
		push(&stack1,mazecells[0][MAZEHEIGHT],0);}
while(!stackIsEmpty(&stack1))
{
  int temp= top(&stack1);
  setDistance(temp)
  pop(&stack1);
  

}

Array 
