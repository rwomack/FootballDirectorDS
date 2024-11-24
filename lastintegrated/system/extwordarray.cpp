//
// extwordarray.cpp - implementation of CExtWordArray class
//


#include "stdafx.h"
#include "extwordarray.h"



 //---------------------------------------------------------------------------------------------------------------------------
CExtWordArray& CExtWordArray::operator = (const CExtWordArray &p)
{
	RemoveAll();
    for (int ListPos = 0; ListPos < p.GetSize(); ListPos ++) 
	{
        Add(p.GetAt(ListPos));
	}
    return *this;
}


 //---------------------------------------------------------------------------
CExtWordArray& CExtWordArray::Copy(const CExtWordArray *p /* = NULL */)
{
	RemoveAll();
    for (int ListPos = 0; ListPos < p->GetSize(); ListPos ++) 
	{
        Add(p->GetAt(ListPos));
	}
    return *this;
}


 //---------------------------------------------------------------------------
void CExtWordArray::Get(FILE* File, int HowMany)
{
	ASSERT (File != NULL);
	ASSERT (HowMany > -1);
    SetSize(HowMany);
    if (HowMany > 0) 
	{
        fread(&CWordArray::m_pData [0], sizeof CWordArray::m_pData [0], HowMany, File);
    }
}


 //---------------------------------------------------------------------------
WORD CExtWordArray::AverageNonZeroValue(void)
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
void CExtWordArray::RaiseValue(WORD x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++) 
	{
        SetAt(n, GetAt(n) + x);
	}
}


 //---------------------------------------------------------------------------
void CExtWordArray::LowerValue(WORD x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++) 
	{
        SetAt(n, GetAt(n) - x);
	}
}


 //---------------------------------------------------------------------------
void CExtWordArray::AddToArray2IfBetweenIncluding(WORD First, WORD Last, CExtWordArray* Array2Ptr)
{
	for (int n = 0; n < GetSize(); n++) 
	{
		if ((GetAt(n) >= First) && (GetAt(n) <= Last)) 
		{
			Array2Ptr->Add(GetAt(n));
		}
	}
}


 //---------------------------------------------------------------------------
int	CExtWordArray::NumberNonZeroLessThan(WORD x)
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
int	CExtWordArray::NumberNonZero(void)
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
void CExtWordArray::FillWithValue(WORD x)
{
	for (int n = 0; n < GetSize(); n++) 
	{
		SetAt(n, x);
	}
}


 //---------------------------------------------------------------------------
void CExtWordArray::FillToEndWithValue(WORD x, int StartPosn)
{
	ASSERT (GetSize() != 0 && StartPosn < GetSize());
	for (int n = StartPosn; n < GetSize(); n++) 
	{
		SetAt(n, x);
	}
}


 // Fill the nn elements of the array starting at element nn with given value
 //---------------------------------------------------------------------------
void CExtWordArray::FillWithValue(WORD x, int StartPosn, int HowMany)
{
    ASSERT (GetSize() != 0 && StartPosn + HowMany <= GetSize());
    for (int n = StartPosn; n < StartPosn + HowMany; n++) 
	{
		SetAt(n, x);
	}
}


 //---------------------------------------------------------------------------
WORD CExtWordArray::HighestValue(void)
{
	WORD wRet = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		wRet = max(GetAt(n), wRet);
	}
	return wRet;
}


 //---------------------------------------------------------------------------
WORD CExtWordArray::LowestValue(void)
{
	WORD wRet = 0xffff;
	for (int n = 0; n < GetSize(); n++) 
	{
		wRet = min(GetAt(n), wRet);
	}
	return wRet;
}


 //---------------------------------------------------------------------------
void CExtWordArray::RemoveDuplicates( void )
{
    WORD val =  0;
    int  FindResult = -2; // default error val.

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


 //---------------------------------------------------------------------------
void CExtWordArray::SetSizeFillWithValue(int HowMany, WORD value)
{
	CWordArray::SetSize(HowMany);
	FillWithValue(value);
}


// Searches array for dwVal, starting at position 'pos.
// Returns 0-based index of value or -1 if not found.
 //---------------------------------------------------------------------------
int CExtWordArray::Find(WORD dwVal, int StartPos /* =0 */)
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


// Randomly arrange the contents of the entire array
 //---------------------------------------------------------------------------
void CExtWordArray::ShuffleArray( void )
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
void CExtWordArray::Swap(int a, int b)
{
	WORD tmp;
	tmp = GetAt(a);
	SetAt(a, GetAt(b));
	SetAt(b, tmp);
}


 // Randomly arrange the contents of the array
 // Between and including startpos and maxpos
 //---------------------------------------------------------------------------
void CExtWordArray::ShuffleArray(int StartPos, int MaxPos)
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


 //---------------------------------------------------------------------------
void CExtWordArray::Read(FILE* File)
{
    ASSERT (File != NULL);
    int HowMany;
    fread(&HowMany, sizeof HowMany, 1, File);
	ASSERT (HowMany > -1);
    SetSize(HowMany);
    if (HowMany > 0) 
	{
        fread(&CWordArray::m_pData [0], sizeof CWordArray::m_pData [0], HowMany, File);
    }
}


 //---------------------------------------------------------------------------
void CExtWordArray::Write(FILE* File)
{
    ASSERT (File != NULL);
    int HowMany = CWordArray::m_nSize;
    fwrite(&HowMany, sizeof HowMany, 1, File);
    if (CWordArray::m_nSize > 0) 
	{
        fwrite(&CWordArray::m_pData [0], sizeof CWordArray::m_pData [0], CWordArray::m_nSize, File);
    }
}


 // Return the cumulative total of all the values within the array
 //---------------------------------------------------------------------------
int CExtWordArray::TotalSum(void)
{
	int iRet = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // Searches array for dwVal, starting at the staret of the array.
 // Returns 0-based index of nearest value above or equal.
 //---------------------------------------------------------------------------
int CExtWordArray::NearestAbove(WORD dwVal)
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


 // If a value contained array2 is in the array then it is removed
 // Removes first instance only
 //---------------------------------------------------------------------------
void CExtWordArray::RemoveIfFoundInArray2 (CExtWordArray* Array2Ptr)
{
	WORD ValueToFind;
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
void CExtWordArray::AddAndRemoveFromArray2 (CExtWordArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = 0; z < HowMany; z++) 
	{
		Add(Array2Ptr->GetAt(Array2Ptr->GetUpperBound()));
		Array2Ptr->RemoveAt(Array2Ptr->GetUpperBound());
	}
}


 //---------------------------------------------------------------------------
void CExtWordArray::AddFromArray2 (CExtWordArray* Array2Ptr, int HowMany)
{
	ASSERT (Array2Ptr->GetSize() >= HowMany);		// Ensure enough to add
	for (int z = Array2Ptr->GetUpperBound(); z < Array2Ptr->GetUpperBound() - HowMany; z--) 
	{
		Add(Array2Ptr->GetAt(z));
	}
}


 //---------------------------------------------------------------------------
void CExtWordArray::DecrementIfNotZero(void)
{
	for (int n = 0; n < GetSize(); n++) 
	{
		if (GetAt(n) > 0) 
		{
			SetAt(n, GetAt(n) - 1);
		}
	}
}


 // Return the cumulative total of the values within the array
 //---------------------------------------------------------------------------
int CExtWordArray::TotalSum(int StartPos, int EndPos)
{
	int iRet = 0;
	for (int n = StartPos; n < EndPos; n++) 
	{
		iRet += GetAt(n);
	}
	return iRet;
}


 // Add the requested number of elements to the end of the array
 // each one containing the specified value
 //---------------------------------------------------------------------------
void CExtWordArray::AddInstances(int HowMany, WORD Value)
{
	for (int n = 0; n < HowMany; n++) 
	{
		Add(Value);
	}
}


 // Count the number of elements that contain the requested value
 //---------------------------------------------------------------------------
int CExtWordArray::CountInstances(WORD Value)
{
	int tot = 0;
	for (int n = 0; n < GetSize(); n++) 
	{
		tot += (GetAt(n) == Value);
	}
	return n;
}


 // How many elements are between the values?
 //---------------------------------------------------------------------------
int CExtWordArray::CountInstancesBetweenIncluding(WORD First, WORD Last)
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
void CExtWordArray::Replace(WORD ToReplace, WORD ReplaceWith)
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
void CExtWordArray::MoveDown(void)
{
     for (int n = 1; n < GetSize(); n++) 
	 {
        SetAt(n, GetAt(n - 1));
	}
}


 //---------------------------------------------------------------------------
bool CExtWordArray::CheckForDuplicates(void)
{
	bool bRet = false;
	WORD val        =  0;
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
