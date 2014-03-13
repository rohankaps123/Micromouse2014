#include "DataComponents.h"

//Functions to get and store info from and to integer data cells

//Notes:
// - What I find online says this implementation of C uses a logical bitshift, not an arithmetic. If it is arithmetic, then some things will need changing. If it doesn't work, this is probably why.
// - All inputs that can be call-by-value currently are. I'm not sure whether by-value or by-reference is better for efficiency, so feel free to change everything to call-by-reference if that is appropriate.

//My general procedures, for debugging:
//Get:
//	Bit-shift left to wipe out all digits to the left of the desired value.
//	Bit-shift right to wipe out values to the right and get the relevant value to the one's place.
//Set:
//	Bit-shift right to wipe out the digit and things to its right.
//	Bit-shift left to get 0s where value will go in last place(s)
//	Add value desired to be set to
//	Bit-shift left to get to correct positions
//	Add remaining digits back, obtained by:
//		Bit-shift left to wipe out digits already possessed
//		Bit-shift right to get to original positions

//Example (Suppose: find/set value B from ABCD)
//Get:
//	ABCD<<1 -> BCD0
//	BCD0>>3 -> 000B
//Set (to E):
//	ABCD>>3 -> 000A
//	000A<<1 -> 00A0
//	00A0+E -> 00AE
//	00AE<<2 -> AE00
//	AE00+00CD=AECD
//		ABCD<<2 -> CD00
//		CD00>>2 -> 00CD

//Data storage Mechanism: -XXXXXXXXDDINSEW
//-: Extra bit
//X: Digit of distance
//DD: Direction departed
//I: Is explored
//NSEW: North, South, East, West


//Getting components
int getN(long input)
{
	return (input & 0b1000) >> 3;
}
int getE(long input)
{
	return (input & 0b0100) >> 2;
}
int getS(long input)
{
	return (input & 0b0010) >> 1;
}
int getW(long input)
{
	return (input & 0b0001) >> 0;
}
int getExp(long input)
{
	return (input & 0b00010000) >> 4;
}
int getExitDir(long input)
{
	return (input & 0b01100000) >> 6;
}
int getDist(long input)
{
	return (input & 0b0111111110000000) >> 7;
}

int getX(long input)
{	
	return (input & 0b11111111L << 15L) >> 15;
}

int getY(long input)
{
	return (input & 0b11111111L << 23L) >> 23;
}

//Setting components: var=variable to change, val=value to give
void setN(long *var, int val)
{
	setVarWithVal(var, val << 3, 0b1000);
}

//Set East Wall
void setE(long *var, int val)
{
	setVarWithVal(var, val << 2, 0b0100);
}

//Set South Wall
void setS(long *var, int val)
{
	setVarWithVal(var, val << 1, 0b0010);
}

//Set West Wall
void setW(long *var, int val)
{
	setVarWithVal(var, val << 0, 0b0001);
}

//Set the Explored bit
void setExp(long *var, int val)
{
	setVarWithVal(var, val << 4, 0b00010000);
}

//Set Exit direction
void setExitDir(long *var, int val)
{
	setVarWithVal(var, val << 5, 0b01100000);	
}

//Set Distance from Goal
void setDistance(long *var, int val)
{
	setVarWithVal(var, val << 7, 0b0111111110000000);
}

void setXY(long *var, int x, int y)
{
	setVarWithVal(var, (long)x << 15L, 0b11111111L << 15L);
	setVarWithVal(var, (long)y << 23L, 0b11111111L << 23L);
}

//Function that writes a bit
void setVarWithVal(long *var, long val, long bits)
{
	//Cut Off any extra bits
	val = val & bits;
	
	//Reset any bits in those spots
	*var &= ~(bits);
	
	//Set our new data to those bits
	*var |= val;
}
