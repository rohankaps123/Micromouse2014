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
int getN(int input)
{
	return ((input<<12)>>15);
}
int getE(int input)
{
	return ((input<<13)>>15);
}
int getS(int input)
{
	return ((input<<14)>>15);
}
int getW(int input)
{
	return ((input<<15)>>15);
}
int getExp(int input)
{
	return ((input<<11)>>15);
}
int getExitDir(int input)
{
	return ((input<<9)>>14);
}
int getDist(int input)
{
	return input>>7;
}

//Setting components: var=variable to change, val=value to give
void setN(int *var, int val)
{
	setVarWithVal(var, NORTH, val);
}
void setE(int *var, int val)
{
	setVarWithVal(var, EAST, val);
}
void setS(int *var, int val)
{
	setVarWithVal(var, SOUTH, val);
}
void setW(int *var, int val)
{
	setVarWithVal(var, WEST, val);
}
void setExp(int *var, int val)
{
	setVarWithVal(var, EXPLORED, val);
}

void setExitDir(int *var, int val)
{
	//Erase Bits to 0
	setVarWithVal(var, DEP_DIRECT, 0);
	
	//Set New Bits, ensure we're don't overwrite other bits
	val = val & 0b00000011;
	setVarWithVal(var, val << 5, 1);
	
}
void setDistance(int *var, int val)
{
	//Erase Bits to 0
	setVarWithVal(var, DISTANCE, 0);
	
	//Set New Bits, ensure we're not overwriting other bits
	val = val & 0b011111111;
	setVarWithVal(var, val << 7, 1);
}

void setVarWithVal(int *var, int val, int bitOn)
{
	//If we're setting a bit to zero
	if(bitOn == 0)
	{
		//Say val = 0001, 
		//  the ~ operand will flip all bits
		//  then AND the current value of var
		//  giving var AND 1110, which say var = 1011, 
		//  will result in 1010, turning off the last bit
		*var &= ~val;
	}
	else
	{
		//Say val = 0001
		//	var OR val
		*var |= val;
	}
}
