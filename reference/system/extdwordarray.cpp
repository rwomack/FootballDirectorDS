
/*

	extdwordarray.cpp - implementation of CExtDWordArray class

	(c) RW Software 1994 - 2001

---------------------------------------------------------------------------------------
*/



#include "stdafx.h"
#include "extdwordarray.h"



 //---------------------------------------------------------------------------------------------------------------------------
CExtDWordArray& CExtDWordArray::operator = (CExtDWordArray* p)
{
	m_iList.clear();
    for (int ListPos = 0; ListPos < p->GetSize(); ListPos ++)
	{
        m_iList.push_back(p->GetAt(ListPos));
	}
    return *this;
}


 //---------------------------------------------------------------------------
void CExtDWordArray::Append(CExtDWordArray* ListToAdd)
{
	for (UInt16 nLoopCount = 0; nLoopCount < ListToAdd->GetSize(); nLoopCount++)
	{
		m_iList.push_back(ListToAdd->GetAt(nLoopCount));
	}
}


 //---------------------------------------------------------------------------
UInt32 CExtDWordArray::AverageNonZeroValue(void)
{
	int Total = 0, Count = 0;
	for (int n = 0; n < GetSize(); n++)
	{
        if (GetAt(n) != 0)
		{
			Total += GetAt(n);
			Count++;
		}
	}
	return Total / Count;
}


 //---------------------------------------------------------------------------
void CExtDWordArray::RaiseValue(UInt32 x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++)
	{
        SetAt(n, GetAt(n) + x);
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::LowerValue(UInt32 x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++)
	{
        SetAt(n, GetAt(n) - x);
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::FillWithValue(UInt32 x)
{
	for (int n = 0; n < GetSize(); n++)
	{
		SetAt(n, x);
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::FillToEndWithValue(UInt32 x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++)
	{
		SetAt(n, x);
	}
}


 // Fill the nn elements of the array starting at element nn with given value
 //---------------------------------------------------------------------------
void CExtDWordArray::FillWithValue(UInt32 x, int StartPosn, int HowMany)
{
    ASSERT (GetSize() != 0 && StartPosn + HowMany <= GetSize());
    for (int n = StartPosn; n < StartPosn + HowMany; n++) {
		SetAt(n, x);
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::RemoveDuplicates(void)
{
	UInt32 val        =  0;
	int   FindResult = -2; // default error val.

	for (int i = 0; i < GetSize(); i++)
	{
		// Re-init FindResult.
		FindResult = -2;

		// Get value at current position
		// and remove any duplicates.
		val = GetAt(i);

		// Walk array starting from current position.
		// Remove any duplicate items along the way.
		while(FindResult != -1) {
			FindResult = Find(val, i + 1);
			if ((FindResult != i) && (FindResult != -1))
			{
				RemoveAt(FindResult);
			}
		}
	}
}


 //---------------------------------------------------------------------------
bool CExtDWordArray::CheckForDuplicates(void)
{
	bool bRet = false;
	UInt32 val        =  0;
	int   FindResult = -2; // default error val.

	for (int i = 0; i < GetSize(); i++) {
		// Re-init FindResult.
		FindResult = -2;

		// Get value at current position
		// and remove any duplicates.
		val = GetAt(i);

		// Walk array starting from current position.
		// Stop at first duplicate.
		while(FindResult != -1) {
			FindResult = Find(val, i + 1);
			if ((FindResult != i) && (FindResult != -1)) {
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


// Searches array for dwVal, starting at position 'pos.
// Returns 0-based index of value.
 //---------------------------------------------------------------------------
int CExtDWordArray::Find(UInt32 dwVal,int StartPos)
{
	for (int i = StartPos; i < GetSize(); i++)
	{
		if (GetAt(i) == dwVal)
		{
			return i; // Success
		}
	}

	// Not found.
	return -1;
}


// Searches array for dwVal, starting at the staret of the array.
// Returns 0-based index of nearest value above or equal or -1 if not found.
 //---------------------------------------------------------------------------
int CExtDWordArray::NearestAbove(UInt32 dwVal)
{
	int NearestSoFar = -1;
    for (int i = 0; i < GetSize(); i++)
	{
        if (GetAt(i) >= dwVal)
		{
			if (NearestSoFar == -1 || GetAt(i) <= GetAt(NearestSoFar))
			{
				NearestSoFar = i;
			}
        }
	}
	return NearestSoFar;
}


 //---------------------------------------------------------------------------
void CExtDWordArray::Get(FILE* File, int HowMany)
{
	ASSERT (File != NULL);
	ASSERT (HowMany > -1);
    SetSize(HowMany);
    if (HowMany > 0)
	{
        fread(&m_iList [0], sizeof m_iList [0], HowMany, File);
    }
}


// Randomly arrange the contents of the entire array
 //---------------------------------------------------------------------------
void CExtDWordArray::ShuffleArray( void )
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


 // Swap the value at position [a] with the value at position [b]
 //---------------------------------------------------------------------------
void CExtDWordArray::Swap(int a, int b)
{
	UInt32 tmp;
	tmp = GetAt(a);
	SetAt(a, GetAt(b));
	SetAt(b, tmp);
}


 //---------------------------------------------------------------------------
void CExtDWordArray::SetSizeFillWithValue(Int16 HowMany, UInt32 value)
{
	SetSize(HowMany, value);
}


 //---------------------------------------------------------------------------
int	CExtDWordArray::NumberNonZeroLessThan(UInt32 x)
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
int	CExtDWordArray::NumberNonZero(void)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++)
	{
		// If the value at [n] is less than x and is not zero, total is incremented
		tot +=  (GetAt(n) != 0);
	}
	return tot;
}


 //---------------------------------------------------------------------------
void CExtDWordArray::DecrementIfNotZero(void)
{
	for (int n = 0; n < GetSize(); n++)
	{
		if (GetAt(n) > 0) {
			SetAt(n, GetAt(n) - 1);
		}
	}
}


 // Randomly arrange the contents of the array
 // Between and including startpos and maxpos
 //---------------------------------------------------------------------------
void CExtDWordArray::ShuffleArray(int StartPos, int MaxPos)
{
	ASSERT (StartPos > -1 && StartPos < MaxPos);
	ASSERT (MaxPos < GetSize());

    UInt32 tmp, temp1;
    for (int n = StartPos; n <= MaxPos; n++)
	{
		if ((MaxPos - StartPos) > 1)
		{
			temp1 = StartPos + RandomIntLessThan(MaxPos - StartPos);   // Choose a random position
			tmp = GetAt(n);
			// Swap the value at the current position with the value at the random position
			SetAt(n, GetAt(temp1));
			SetAt(temp1, tmp);
		}
    }
}


 //---------------------------------------------------------------------------
void CExtDWordArray::Read(FILE* File)
{
	ASSERT (File != NULL);
    int HowMany;
    fread(&HowMany, sizeof HowMany, 1, File);
	ASSERT (HowMany > -1);
    SetSize(HowMany);
    if (HowMany > 0)
	{
        fread(&m_iList [0], sizeof m_iList [0], HowMany, File);
    }
}


 //---------------------------------------------------------------------------
void CExtDWordArray::Write(FILE* File)
{
	ASSERT (File != NULL);
    int HowMany = GetSize();
    fwrite(&HowMany, sizeof HowMany, 1, File);
    if (GetSize() > 0)
	{
        fwrite(&m_iList [0], sizeof m_iList [0], HowMany, File);
    }
}


 // Get the contents of a random entry
 //---------------------------------------------------------------------------
UInt32 CExtDWordArray::RandomValue(void)
{
	if (GetSize() > 0)
	{
		return GetAt(RandomPosition());
	}
	return MAXDWORD;
}


 //---------------------------------------------------------------------------
void CExtDWordArray::FillWithRandomValue(int MaxValue)
{
	ASSERT (GetSize() != 0);
	for (int n = 0; n < GetSize(); n++)
	{
		SetAt(n, RandomIntLessThan(MaxValue));
	}
}


 //---------------------------------------------------------------------------
UInt32 CExtDWordArray::HighestValue(void)
{
	UInt32 dRet = 0;
	for (int n = 0; n < GetSize(); n++)
	{
		dRet = max(dRet, GetAt(n));
	}
	return dRet;
}


 //---------------------------------------------------------------------------
UInt32 CExtDWordArray::LowestValue(void)
{
	UInt32 dRet = 0xffffffff;
	for (int n = 0; n < GetSize(); n++)
	{
		dRet = min(dRet, GetAt(n));
	}
	return dRet;
}


 // Get a random index
 //---------------------------------------------------------------------------
int CExtDWordArray::RandomPosition(void)
{
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
int CExtDWordArray::TotalSum(void)
{
	int iRet = 0;
	for (int n = 0; n < GetSize(); n++)
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // Return the cumulative total of the values within the array
 //---------------------------------------------------------------------------
int CExtDWordArray::TotalSum(int StartPos, int EndPos)
{
	int iRet = 0;
	for (int n = StartPos; n < EndPos; n++)
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // If a value contained array2 is in the array then it is removed
 // Removes first instance only(assumes no duplicates)
 //---------------------------------------------------------------------------
void CExtDWordArray::RemoveIfFoundInArray2 (CExtDWordArray* Array2Ptr)
{
	UInt32 ValueToFind;
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
void CExtDWordArray::AddAndRemoveFromArray2 (CExtDWordArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = 0; z < HowMany; z++)
	{
		m_iList.push_back(Array2Ptr->GetAt(Array2Ptr->GetUpperBound()));
		Array2Ptr->RemoveAt(Array2Ptr->GetUpperBound());
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::AddToArray2IfBetweenIncluding(UInt32 First, UInt32 Last, CExtDWordArray* Array2Ptr)
{
	for (int n = 0; n < GetSize(); n++)
	{
		if ((GetAt(n) >= First) && (GetAt(n) <= Last))
		{
			Array2Ptr->m_iList.push_back(GetAt(n));
		}
	}
}


 //---------------------------------------------------------------------------
void CExtDWordArray::AddFromArray2 (CExtDWordArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = Array2Ptr->GetUpperBound(); z < Array2Ptr->GetUpperBound() - HowMany; z--)
	{
		m_iList.push_back(Array2Ptr->GetAt(z));
	}
}


 // Add the requested number of elements to the end of the array
 // each one containing the specified value
 //---------------------------------------------------------------------------
void CExtDWordArray::AddInstances(int HowMany, UInt32 Value)
{
	for (int n = 0; n < HowMany; n++)
	{
		m_iList.push_back(Value);
	}
}


 // Count the number of elements that contain the requested value
 //---------------------------------------------------------------------------
int CExtDWordArray::CountInstances(UInt32 Value)
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
int CExtDWordArray::CountInstancesBetweenIncluding(UInt32 First, UInt32 Last)
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
void CExtDWordArray::Replace(UInt32 ToReplace, UInt32 ReplaceWith)
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
void CExtDWordArray::MoveDown(void)
{
     for (int n = 1; n < GetSize(); n++)
	 {
        SetAt(n, GetAt(n - 1));
	}
}

UInt32 CExtDWordArray::AverageValue(void)								{ return TotalSum() / GetSize(); }
UInt32 CExtDWordArray::AverageValue(int StartPos, int EndPos)			{ return TotalSum(StartPos, EndPos) / (EndPos - StartPos); }
