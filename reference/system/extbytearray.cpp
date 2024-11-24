
/*

	extbytearray.cpp - implementation of CExtByteArray class

	(c) Rw Software Ltd 1994 - 2005

---------------------------------------------------------------------------------------
*/


#include "stdafx.h"
#include "extbytearray.h"


 //---------------------------------------------------------------------------------------------------------------------------
CExtByteArray& CExtByteArray::operator = (const CExtByteArray &p)
{
	RemoveAll();
    for (int ListPos = 0; ListPos < p.GetSize(); ListPos ++) 
	{
        Add(p.GetAt(ListPos));
	}
    return *this;
}


 //---------------------------------------------------------------------------
void CExtByteArray::AddToArray2IfBetweenIncluding(BYTE First, BYTE Last, CExtByteArray* Array2Ptr)
{
	ASSERT (GetSize() != 0 && Array2Ptr->GetSize() != 0);
	for (int n = 0; n < GetSize(); n++) 
	{
		if ((GetAt(n) >= First) && (GetAt(n) <= Last)) 
		{
			Array2Ptr->Add(GetAt(n));
		}
	}
}


 //---------------------------------------------------------------------------
BYTE CExtByteArray::AverageNonZeroValue(BYTE HowMany)
{
	ASSERT (GetSize() != 0 && HowMany <= GetSize());
	int Total = 0, Count = 0;
	if (HowMany == 0)
	{
		// If number of elements not specified then whole array
		HowMany = GetSize();
	}
	for (int n = 0; n < HowMany; n++) 
	{
        if (GetAt(n) != 0) 
		{
			Total += GetAt(n);
			Count++;
		}
	}
	if (Count == 0) 
	{
		return 0;
	}
	else 
	{
		return Total / Count;
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::FillWithValue(BYTE x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	memset(&CByteArray::m_pData [StartPosn], x, (GetSize() - StartPosn) * sizeof(BYTE));
}


 //---------------------------------------------------------------------------
void CExtByteArray::RaiseValue(BYTE x, int StartPosn)
{
	BYTE Value, z;
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++) 
	{
		Value = GetAt(n) + x;
		z = GetAt(n);
		// Ensure no overflow
		if (Value > GetAt(n)) 
		{
			SetAt(n, Value);
		}
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::LowerValue(BYTE x, int StartPosn)
{
	BYTE Value;
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++) 
	{
		Value = GetAt(n) - x;
		// Ensure no overflow
		if (Value < GetAt(n)) 
		{
			SetAt(n, Value);
		}
	}
}


 // Fill the nn elements of the array starting at element nn with given value
 //---------------------------------------------------------------------------
void CExtByteArray::FillWithValue(BYTE x, int StartPosn, int HowMany)
{
    ASSERT (GetSize() != 0 && StartPosn + HowMany <= GetSize());
    for (int n = StartPosn; n < StartPosn + HowMany; n++) 
	{
		SetAt(n, x);
	}
}


 // Fill the array starting at element nn with given value
 //---------------------------------------------------------------------------
void CExtByteArray::FillToEndWithValue(BYTE x, int StartPosn)
{
    ASSERT (GetSize() != 0 && StartPosn < GetSize());
    for (int n = StartPosn; n < GetSize(); n++) 
	{
		SetAt(n, x);
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::FillWithRandomValue(BYTE MaxValue)
{
	for (int n = 0; n < GetSize(); n++) {
		SetAt(n, RandomIntLessThan(MaxValue));
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::FillWithRandomValue(BYTE AverageValue, BYTE Deviation, BYTE MaxValue, BYTE StartPosn)
{
	BYTE NewValue;
	for (int n = StartPosn; n < GetSize(); n++) 
	{
		NewValue = (AverageValue - Deviation) + RandomIntLessThan(Deviation * 2);
		SetAt(n, min(MaxValue, NewValue));
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::SetSizeFillWithValue(int HowMany, BYTE value)
{
	CByteArray::SetSize(HowMany);
	FillWithValue(value);
}


 //---------------------------------------------------------------------------
void CExtByteArray::DecrementIfNotZero(void)
{
	for (int n = 0; n < GetSize(); n++)
	{
		if (GetAt(n) > 0)
		{
			SetAt(n, GetAt(n) - 1);
		}
	}
}


 //---------------------------------------------------------------------------
BYTE CExtByteArray::HighestValue(void)
{
	BYTE bRet = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		bRet = max(GetAt(n), bRet);
	}
	return bRet;
}


 //---------------------------------------------------------------------------
BYTE CExtByteArray::LowestValue(void)
{
	BYTE bRet = 0xff;
	for (int n = 0; n < GetSize(); n++) 
	{
		bRet = min(GetAt(n), bRet);
	}
	return bRet;
}


 //---------------------------------------------------------------------------
void CExtByteArray::RemoveDuplicates( void )
{
    BYTE val =  0;
    int FindResult = -2; // default error val.

    for (int i = 0; i < GetSize(); i++) 
	{
		// Re-init FindResult.
		FindResult = -2;

		// Get value at current position
		// and remove any duplicates.
		val = GetAt(i);

		// Walk array starting from current position.
		// Remove any duplicate items along the way.
        while(FindResult != -1)  
		{
			FindResult = Find(val,i+1);
            if ( (FindResult != i) && (FindResult != -1) ) 
			{
				RemoveAt(FindResult);
            }
		}
	}
}


// Searches array for dwVal, starting at position 'pos.
// Returns 0-based index of value or -1 if not found.
 //---------------------------------------------------------------------------
int CExtByteArray::Find(BYTE dwVal, int StartPos)
{
    for (int i = StartPos; i < GetSize(); i++) 
	{
        if (GetAt(i) == dwVal) 
		{
            return i;           // Success
        }
	}

	// Not found.
	return -1;
}


// Searches array for dwVal, starting at the staret of the array.
// Returns 0-based index of nearest value above or equal.
 //---------------------------------------------------------------------------
int CExtByteArray::NearestAbove(BYTE dwVal)
{
	int NearestSoFar = -1;
    for (int i = 0; i < GetSize(); i++) 
	{
        if (GetAt(i) >= dwVal) 
		{
			if (NearestSoFar == -1 || GetAt(i) < GetAt(NearestSoFar)) 
			{
				NearestSoFar = i;
			}
        }
	}
	return NearestSoFar;
}


// Randomly arrange the contents of the entire array
 //---------------------------------------------------------------------------
void CExtByteArray::ShuffleArray( void )
{
	int HowMany = GetSize();
	ASSERT (HowMany > 0);
    for (int n = 0; n < HowMany; n++) 
	{
		if (HowMany > 1) 
		{
			Swap(n, RandomIntLessThan(HowMany - 1));
		}
    }
}


 // Randomly arrange the contents of the array
 // Between and including startpos and maxpos
 //---------------------------------------------------------------------------
void CExtByteArray::ShuffleArray(int StartPos, int MaxPos)
{
	ASSERT (StartPos > -1 && StartPos < MaxPos);
	ASSERT (MaxPos < GetSize());
    for (int n = StartPos; n <= MaxPos; n++) 
	{
		if ((MaxPos - StartPos) > 1) 
		{
			Swap(n, StartPos + RandomIntLessThan(MaxPos - StartPos));
		}
    }
}


 // Swap the value at position [a] with the value at position [b]
 //---------------------------------------------------------------------------
void CExtByteArray::Swap(int a, int b)
{
	BYTE tmp;
	tmp = GetAt(a);
	SetAt(a, GetAt(b));
	SetAt(b, tmp);
}


 //---------------------------------------------------------------------------
void CExtByteArray::Read(FILE* File)
{
	ASSERT (File != NULL);
	int HowMany;
    fread(&HowMany, sizeof HowMany, 1, File);
	ASSERT (HowMany > -1);
	RemoveAll();
	if (HowMany > 0) 
	{
		SetSize(HowMany);
        fread(&CByteArray::m_pData [0], sizeof CByteArray::m_pData [0], HowMany, File);
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::Get(FILE* File, int HowMany)
{
	ASSERT (File != NULL);
	ASSERT (HowMany > 0);
	SetSize(HowMany);
    if (HowMany > 0) 
	{
        fread(&CByteArray::m_pData [0], sizeof CByteArray::m_pData [0], HowMany, File);
    }
}


 //---------------------------------------------------------------------------
void CExtByteArray::Write(FILE* File)
{
	ASSERT (File != NULL);
	int HowMany = GetSize();
    fwrite(&HowMany, sizeof HowMany, 1, File);
	if (HowMany > 0) 
	{
        fwrite(&CByteArray::m_pData [0], sizeof CByteArray::m_pData [0], GetSize(), File);
	}
}


 // Get the contents of a random entry
 //---------------------------------------------------------------------------
BYTE CExtByteArray::RandomValue(void)
{
	return GetAt(RandomPosition());
}


 // Get a random index
 //---------------------------------------------------------------------------
int CExtByteArray::RandomPositionOfValue(BYTE value)
{
	int bRet;
	do
	{
		bRet = RandomIntLessThan(GetUpperBound());
	} 
	while(GetAt(bRet) != value);
	return bRet;
}


 // Get a random index
 //---------------------------------------------------------------------------
int CExtByteArray::RandomPosition(void)
{
    int ListPos = 0;
	if (GetSize() > 1) 
	{
		return RandomIntLessThan(GetUpperBound());
	}
	else 
	{
		return 0;
	}
}


 // Return the cumulative total of all the values within the array
 //---------------------------------------------------------------------------
int CExtByteArray::TotalSum(void)
{
	int iRet = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // Calculate how many elements of array totaled together <=value
 //------------------------------------------------------------------
int CExtByteArray::PositionLessThanValue(int nm)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		tot += GetAt(n);
		if (nm <= tot) 
		{
			break;
		}
	}
	return n;
}


 //---------------------------------------------------------------------------
int	CExtByteArray::NumberNonZeroLessThan(BYTE x)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		// If the value at [n] is less than x and is not zero, total is incremented
		tot += (GetAt(n) < x) && (GetAt(n) != 0);
	}
	return tot;
}


 //---------------------------------------------------------------------------
int	CExtByteArray::NumberNonZero(void)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		// If the value at [n] is less than x and is not zero, total is incremented
		tot += (GetAt(n) != 0);
	}
	return tot;
}


 //---------------------------------------------------------------------------
int	CExtByteArray::NumberZero(void)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		// If the value at [n] is zero, total is incremented
		tot += (GetAt(n) == 0);
	}
	return tot;
}


 // Return the cumulative total of the values within the array
 //---------------------------------------------------------------------------
int CExtByteArray::TotalSum(int StartPos, int EndPos)
{
	int iRet = 0;
	for (int n = StartPos; n < EndPos; n++) 
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // If a value contained array2 is in the array then it is removed
 // Removes first instance only
 //---------------------------------------------------------------------------
void CExtByteArray::RemoveIfFoundInArray2 (CExtByteArray* Array2Ptr)
{
	BYTE ValueToFind;
	for (int n = 0; n < Array2Ptr->GetSize(); n++) 
	{
		ValueToFind = Array2Ptr->GetAt(n);
		for (int z = 0; z < GetSize(); z++) 
		{
			if (GetAt(z) == ValueToFind) 
			{
				RemoveAt(z);
				break;
			}
		}
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::AddAndRemoveFromArray2 (CExtByteArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = 0; z < HowMany; z++) 
	{
		Add(Array2Ptr->GetAt(Array2Ptr->GetUpperBound()));
		Array2Ptr->RemoveAt(Array2Ptr->GetUpperBound());
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::AddFromArray2 (CExtByteArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = Array2Ptr->GetUpperBound(); z < Array2Ptr->GetUpperBound() - HowMany; z--) 
	{
		Add(Array2Ptr->GetAt(z));
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::SortLow(void)
{
    bool t_Swap = true;
	int n;
    while(t_Swap == true) 
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            if (GetAt(n) < GetAt(n + 1)) 
			{
				Swap(n, n + 1);
				t_Swap = false;
			}
		}
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::SortHigh(void)
{
    bool t_Swap = true;
	int n;
    while(t_Swap == true) 
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            if (GetAt(n) > GetAt(n + 1)) 
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 // Add the requested number of elements to the end of the array
 // each one containing the specified value
 //---------------------------------------------------------------------------
void CExtByteArray::AddInstances(int HowMany, BYTE Value)
{
	for (int n = 0; n < HowMany; n++) 
	{
		Add(Value);
	}
}


 // Count the number of elements that contain the requested value
 //---------------------------------------------------------------------------
int CExtByteArray::CountInstances(BYTE Value)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		tot += (GetAt(n) == Value);
	}
	return tot;
}


 // How many elements are between the values?
 //---------------------------------------------------------------------------
int CExtByteArray::CountInstancesBetweenIncluding(BYTE First, BYTE Last)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		// Add to total found if in range
		tot += (GetAt(n) >= First) && (GetAt(n) <= Last);
	}
	return tot;
}


 // Replace all intances of value 1 with value 2
 //---------------------------------------------------------------------------
void CExtByteArray::Replace(BYTE ToReplace, BYTE ReplaceWith)
{
	 for (int n = 0; n < GetSize(); n++) 
	 {
		if (ToReplace == GetAt(n)) 
		{
			SetAt(n, ReplaceWith);
		}
	}
}


 //---------------------------------------------------------------------------
void CExtByteArray::MoveDown(void)
{
     for (int n = 1; n < GetSize(); n++) 
	 {
        SetAt(n, GetAt(n - 1));
	}
}


 //---------------------------------------------------------------------------
bool CExtByteArray::CheckForDuplicates(void)
{
	bool bRet = false;
	BYTE val        =  0;
	int   FindResult = -2; // default error val.

	for (int i = 0; i < GetSize(); i++) 
	{
		// Re-init FindResult.
		FindResult = -2;

		// Get value at current position
		// and remove any duplicates.
		val = GetAt(i);

		// Walk array starting from current position.
		// Stop at first duplicate.
		while(FindResult != -1) 
		{
			FindResult = Find(val, i + 1);
			if ((FindResult != i) && (FindResult != -1)) 
			{
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}

