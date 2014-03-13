#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <RobotModel.h>
#include <RobotModel_Controls.h>
#include <create stack.h>
#include <update stack.h>
#include <DataComponents.h>

int Check_For_Walls()
{
    /*Declare array and array lists used in code.*/
    Maze[][] maze = new Maze[16][16];
    init_stack[] Current_Walls = new init_stack[];
    init_stack[] Discovered_Walls = new init_stack[];
    init_stack[] Explored_Squares = new init_stack[];
    init_stack[] Available_Adjacent_Squares = new init_stack[];
    Current_Square = 14;

    /*Mouse moves and checks for walls.*/
    while (Current_Square > 0){
            Push(Current_Square, Explored_Squares);
            Front_Value = getN(int isWallFront());
            Right_Value = getE(int isWallRight());
            Left_Value = getW(int isWallLeft());
            /*Test each adjacent square to determine whether it is free or there is a wall.*/
            if (Front_Value >= 16) {
                    Push(Front_Value, Available_Adjacent_Squares);
            }
            else {
                    Push(Front_Value, Current_Walls);
            }
            if (Right_Value >= 8) {
                    Push(Right_Value, Available_Adjacent_Squares);
            }
            else {
                    Push(Right_Value, Current_Walls);
            }
            if (Left_Value >= 8) {
                    Push(Left_Value, Available_Adjacent_Squares);

            }
            else {
                    Push(Left_Value, Current_Walls);
            }
    }
}
