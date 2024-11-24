
/*
 
	HandangoRegCode.cpp : implementation file

	(c) RW Software 2003

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "HandangoRegCode.h"


// CHandangoRegCode


/*------------------------------------------------------------------------------
	Method:   	CHandangoRegCode::CHandangoRegCode
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CHandangoRegCode::CHandangoRegCode()
{
}


/*------------------------------------------------------------------------------
	Method:   	CHandangoRegCode::~CHandangoRegCode
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CHandangoRegCode::~CHandangoRegCode()
{
}


/*------------------------------------------------------------------------------
	Method:   	CHandangoRegCode::DoCalculateRegCode
	Access:    	public 
	Parameter: 	uint* _RPN
	Parameter: 	char* _FullPUN - must be valid tokens made from a list of 
				RPN_* opcode defines and 16-bit numbers, terminated by RPN_END
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort	CHandangoRegCode::DoCalculateRegCode (uint* _RPN, char* _FullPUN)
{
	ushort	Key, C, I;
	ushort	Arg1, Arg2;
	int	MaxLen, Next , i, NextOpPos;
	uint	Op;
	char	PUN[16];

	// Clip PUN to 10 characters if necessary
	//PR_ClearStruct(PUN);
	MaxLen = strlen(_FullPUN);
	Next = 0;
	for (i = 0; i < MaxLen; i++)
	{
		PUN[Next++] = _FullPUN[i];
		if (Next >= 10)
		{
			break;
		}
	}
	PUN[Next] = '\0';
	
	if (strlen(_FullPUN) > 10)
	{
		Next = 9;
		for (i = MaxLen - 1; i >= 0; i--)
		{   
			PUN[Next--] = _FullPUN[i];
			if (Next < 5)
			{
				break;
			}
		}
	}

	// Apply the RPN string to the PUN according to Handango's algorithm
	Key = 0;
	int PUNLength = strlen(PUN);
	RPN_StackPtr = 0;
	for (I = 0; I < PUNLength; I++)
	{
		NextOpPos = 0;
		C = (ushort)PUN[I];
		while ((Op = _RPN[NextOpPos++]) != RPN_END)
		{
			// Keep applying operators until there are no more, meaning
			// we have reached the end of the RPN code!
			if (Op & BIT31)
			{
				// Do the stack-based calculation. I wish I was doing this
				// on the Jupiter Ace. Who remembers that then !  :)
				switch (Op)
				{
					case RPN_OP_ADD:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 + Arg2);
						break;

					case RPN_OP_SUB:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 - Arg2);
						break;

					case RPN_OP_MULT:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 * Arg2);
						break;

					case RPN_OP_DIV:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 / Arg2);
						break;

					case RPN_OP_ISNOTEQ:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if ((!Arg1) && (!Arg2))
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;

					case RPN_OP_LSHIFT:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 << Arg2);
						break;

					case RPN_OP_RSHIFT:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 >> Arg2);
						break;

					case RPN_OP_ISLESSTHAN:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if (Arg1 < Arg2)
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;	

					case RPN_OP_ISGREATERTHAN:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if (Arg1 > Arg2)
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;

					case RPN_OP_ISEQUAL:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if (Arg1 == Arg2)
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;

					case RPN_OP_ISGREATEREQUAL:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if (Arg1 >= Arg2)
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;

					case RPN_OP_ISLESSEQUAL:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						if (Arg1 <= Arg2)
							RPN_Push(RPN_TRUE);
						else
							RPN_Push(RPN_FALSE);
						break;

					case RPN_OP_BITAND:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 & Arg2);
						break;

					case RPN_OP_BITOR:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 | Arg2);
						break;

					case RPN_OP_BITXOR:
						Arg2 = RPN_Pop();
						Arg1 = RPN_Pop();
						RPN_Push(Arg1 ^ Arg2);
						break;

					default:
						EXT_ASSERT(FALSE);
						break;
				}
			}
			else
			{
				if (Op & BIT30)
				{
					// Decode the variable and push it
					if (Op == RPN_VAR_KEY)
					{
						RPN_Push(Key);
					}
					else if (Op == RPN_VAR_I)
					{
						RPN_Push(I);
					}
					else
					{
						EXT_ASSERT(Op == RPN_VAR_C);
						RPN_Push(C);
					}
				}
				else
				{
					// Decode the number and push it
					RPN_Push((ushort)Op);
				}
			}
		}
		Key = RPN_Pop();
	}
//	afxDump << Key;
	return (Key);
}
