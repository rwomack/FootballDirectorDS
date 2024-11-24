

    // File:- array.cpp


    // 30/12/96 09:25

 //===========================================================================

#include "stdafx.h"


 // Move all elements of the array 1 place
 // starting with {start)
 //------------------------------------------------------------------------------------------------
void movedownarray(int* arrptr, int arrsize, int start) //*****NOTE:changed arr[] to a pointer****
{
    for (int nLoopCount = start; nLoopCount < arrsize; nLoopCount++)  
	{
        *(arrptr + nLoopCount) = *(arrptr + nLoopCount + 1);
    }
    *(arrptr + arrsize) = 0;
}


 //------------------------------------------------------------------------------------------------
void movedownLarray(long* arrptr, int arrsize, int start)
{
    for (int nLoopCount = start; nLoopCount < arrsize ; nLoopCount++)
	{
        *(arrptr + nLoopCount) = *(arrptr + nLoopCount + 1);
    }
    *(arrptr + arrsize) = 0;
}


 // Count the number of zero's in the single dimension array
 // of integers
 //------------------------------------------------------------------------------------------------
int NumFreeSlots(int* Arrayptr, int arrsize)  //pass a pointer to the array together with the array size
{
    int count = 0;
    for(int nLoopCount = 0; nLoopCount < arrsize; nLoopCount++)
	{
        if (*(Arrayptr + nLoopCount) == 0) 
		{
			count++;
		}
	}
    return(count);
}


 // Find value's position in array
 //------------------------------------------------------------------------------------------------
int posarray(int* arrptr, int arrsize, int value)
{
    for (int nLoopCount = 0; nLoopCount < arrsize; nLoopCount++)
	{
        if (*(arrptr + nLoopCount) == value) 
		{
			return nLoopCount;
		}
	}

    return (FALSE);
}

 //------------------------------------------------------------------------------------------------
int posLarray(long* arrptr, int arrsize, long value)
{
    for (int nLoopCount = 0; nLoopCount < arrsize; nLoopCount++)
	{
        if (*(arrptr + nLoopCount) == value) 
		{
			return nLoopCount;
		}
	}

    return(FALSE);
}


 // Randomly arrange the contents of the entire array
 //---------------------------------------------------------------------------
void ShuffleArray(CWordArray* TheArray)
{
	ASSERT (TheArray != NULL);
    WORD tmp, temp1;
	int HowMany = TheArray->GetSize();
	ASSERT (HowMany > 0);
    for (int nLoopCount = 0; nLoopCount < HowMany; nLoopCount++) 
	{
        temp1 = RandomIntLessThan(HowMany - 1);
		// Swap the value at the current position with the value at the random position
        tmp = TheArray->GetAt(nLoopCount);
        TheArray->SetAt(nLoopCount, TheArray->GetAt(temp1));
        TheArray->SetAt(temp1, tmp);
    }
}


 // Randomly arrange the contents of the array
 // Between and including startpos and maxpos
 //---------------------------------------------------------------------------
void ShuffleArray(CWordArray* TheArray, int StartPos, int MaxPos)
{
	ASSERT (TheArray != NULL);
	ASSERT (StartPos > -1 && StartPos < MaxPos);
	ASSERT (MaxPos < TheArray->GetSize());

    WORD tmp, temp1;
    for (int nLoopCount = StartPos; nLoopCount <= MaxPos; nLoopCount++) 
	{
        temp1 = StartPos + RandomIntLessThan(MaxPos - StartPos);   // Choose a random position
        tmp = TheArray->GetAt(nLoopCount);
		// Swap the value at the current position with the value at the random position
        TheArray->SetAt(nLoopCount, TheArray->GetAt(temp1));
        TheArray->SetAt(temp1, tmp);
    }
}
