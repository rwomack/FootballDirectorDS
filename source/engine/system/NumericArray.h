
/*

    NumericArray.h - interface of CNumericArray template class

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------------------------------
*/



#ifndef __NUMERIC_H__
#define __NUMERIC_H__

template <class Ttype> class CNumericArray : public Vector<Ttype>
{
public:


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Increment
	Access:    	public
	Parameter: 	const ushort _Position
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Increment(const int _Position)
{
	EXT_ASSERT(_Position < Vector<Ttype>::GetSize());
	m_Data[_Position]++;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::AverageNonZeroValue
	Access:    	public
	Parameter: 	ushort _HowMany
	Returns:   	Ttype
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
Ttype AverageNonZeroValue(int _HowMany)
{
	EXT_ASSERT(_HowMany < Vector<Ttype>::GetSize());
	uint Total = 0, Count = 0;
	if (_HowMany == 0)
	{
		// If number of elements not specified then whole array
		_HowMany = Vector<Ttype>::GetSize();
	}
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
        if (Vector<Ttype>::GetAt(LoopCount) != 0)
		{
			Total += Vector<Ttype>::GetAt(LoopCount);
			Count++;
		}
	}
	if (Count == 0)
	{
		// No values found
		return 0;
	}
	return Total / Count;
}


// Searches array for anValue, starting at position 'pos.
// Returns 0-based index of value or -1 if not found.
 //---------------------------------------------------------------------------
ushort Find(const Ttype _Value, const int _StartIndex = 0)
{
    for (int LoopCount = _StartIndex; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
        if (Vector<Ttype>::GetAt(LoopCount) == _Value)
		{
			return LoopCount; // Success
        }
	}

	// Not found.
	return wNOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::CheckForDuplicates
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CheckForDuplicates()
{
	bool bRet = false;
	Ttype val;
	ushort FindResult = 0; // default error val.

	for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		// Re-init FindResult.
		FindResult = 0;

		// Get value at current position
		val = Vector<Ttype>::GetAt(LoopCount);

		// Walk array starting from current position.
		// Stop at first duplicate.
		while(FindResult != wNOTFOUND)
		{
			FindResult = Find(val, LoopCount + 1);
			if ((FindResult != LoopCount) && (FindResult != wNOTFOUND))
			{
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Copy
	Access:    	public
	Parameter: 	CNumericArray & _Array
	Returns:   	CNumericArray&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray& Copy(CNumericArray& _Array)
{
	RemoveAll();
    for (int ListPos = 0; ListPos < _Array.GetSize(); ListPos ++)
	{
        Vector<Ttype>::Add(_Array.m_Data[ListPos]);
	}
    return *this;
}


 // Count the number of elements that contain the requested value
 //---------------------------------------------------------------------------
ushort CountInstances(const Ttype _Value) const
{
	ushort tot = 0;
	for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		tot += (Vector<Ttype>::GetAt(LoopCount) == _Value);
	}
	return tot;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::CountInstancesBetween
	Access:    	public
	Parameter: 	const Ttype _LowValue
	Parameter: 	const Ttype _HighValue
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CountInstancesBetween(const Ttype _LowValue, const Ttype _HighValue) const
{
	ushort Count = 0;
	for (int ListPos = 0; ListPos < Vector<Ttype>::GetSize(); ListPos++)
	{
		if (Vector<Ttype>::GetAt(ListPos) >= _LowValue && Vector<Ttype>::GetAt(ListPos) <= _HighValue)
		{
			Count++;
		}
	}
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::FillToEndWithValue
	Access:    	public
	Parameter: 	const Ttype _Value
	Parameter: 	const ushort _StartIndex
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void FillToEndWithValue(const Ttype _Value, const ushort _StartIndex)
{
	EXT_ASSERT(Vector<Ttype>::GetSize() != 0 && _StartIndex < Vector<Ttype>::GetSize());
	for (int LoopCount = _StartIndex; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		Vector<Ttype>::SetAt(LoopCount, _Value);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::FillWithValue
	Access:    	public
	Parameter: 	Ttype _Value
	Parameter: 	ushort _StartIndex
	Parameter: 	ushort _EndPosn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void FillWithValue(Ttype _Value, ushort _StartIndex = 0, ushort _EndPosn = 0)
{
	EXT_ASSERT(Vector<Ttype>::GetSize() != 0 && _StartIndex < Vector<Ttype>::GetSize());
	if (_EndPosn == 0)
	{
		_EndPosn = Vector<Ttype>::GetSize();
	}
	for (int LoopCount = _StartIndex; LoopCount < _EndPosn; LoopCount++)
	{
		Vector<Ttype>::SetAt(LoopCount, _Value);
	}
}


 //---------------------------------------------------------------------------
CNumericArray& operator = (const CNumericArray& _Array2)
{
	Vector<Ttype>::RemoveAll();
    for (int ListPos = 0; ListPos < _Array2.GetSize(); ListPos++)
	{
        Vector<Ttype>::Add(_Array2.Vector<Ttype>::GetAt(ListPos));
	}
    return *this;
}

/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::IsElementBetween
	Access:    	public
	Parameter: 	const ushort _Element
	Parameter: 	Ttype _LowValue
	Parameter: 	Ttype _HighValue
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool IsElementBetween(const ushort _Element, Ttype _LowValue, Ttype _HighValue)
{
	if (Vector<Ttype>::GetAt(_Element) >= _LowValue && Vector<Ttype>::GetAt(_Element) <= _HighValue)
	{
		return true;
	}
	return false;
}

/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::DoAddAndRemoveFromArray2
	Access:    	public
	Parameter: 	CNumericArray & _Array2
	Parameter: 	const ushort _HowMany
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void DoAddAndRemoveFromArray2(CNumericArray& _Array2, const ushort _HowMany)
{
	EXT_ASSERT(_Array2.GetSize() >= _HowMany);		// Ensure enough to add
	ushort nToRemovePos = _Array2.GetSize() - _HowMany;
 	for (int LoopCount = nToRemovePos; LoopCount < _Array2.GetSize(); LoopCount++)
	{
		Vector<Ttype>::Add(_Array2[LoopCount]);
	}
	_Array2.RemoveFromEnd(_HowMany);
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Get
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Parameter: 	const ushort _HowMany
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Get(CDataFile* _pDataFile, const ushort _HowMany)
{
	EXT_ASSERT(File != null);
	Ttype nTemp;
    Vector<Ttype>::SetSize(_HowMany);
    for (int nCounter = 0; nCounter < Vector<Ttype>::GetSize(); nCounter++)
	{
        File->Read(&nTemp, sizeof(nTemp));
		Vector<Ttype>::SetAt(nCounter, nTemp);
    }
}


#ifdef CBinaryReader
 //---------------------------------------------------------------------------
void Get(CBinaryReader& _Resource, const ushort _HowMany)
{
	Ttype nTemp;
    Vector<Ttype>::SetSize(_HowMany);
    for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
    _Resource.ReadBytes((BYTE*)&nTemp, sizeof(nTemp));
		Vector<Ttype>::SetAt(LoopCount, nTemp);
    }
}
#endif


 //---------------------------------------------------------------------------
void LowerValue(const Ttype _Value, ushort _StartIndex)
{
	EXT_ASSERT(Vector<Ttype>::GetSize() != 0 && _StartIndex < Vector<Ttype>::GetSize());
	for (int LoopCount = _StartIndex; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
        Vector<Ttype>::SetAt(LoopCount, Vector<Ttype>::GetAt(LoopCount) - _Value);
	}
}


// Searches array for anValue, starting at the staret of the array.
// Returns 0-based index of nearest value above or equal or -1 if not found.
 //---------------------------------------------------------------------------
ushort NearestAbove(const Ttype _Value)
{
	ushort NearestSoFar = wNOTFOUND;
    for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
        if (Vector<Ttype>::GetAt(LoopCount) >= _Value)
		{
			if (NearestSoFar == wNOTFOUND || Vector<Ttype>::GetAt(LoopCount) <= Vector<Ttype>::GetAt(NearestSoFar))
			{
				NearestSoFar = LoopCount;
			}
        }
	}
	return NearestSoFar;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::NumberNonZero
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort NumberNonZero()
{
	ushort tot = 0;
	for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		// If the value at [LoopCount] is less than x and is not zero, total is incremented
		tot += (Vector<Ttype>::GetAt(LoopCount) != 0);
	}
	return tot;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::NumberNonZeroLessThan
	Access:    	public
	Parameter: 	const Ttype _Value
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort NumberNonZeroLessThan(const Ttype _Value)
{
	ushort tot = 0;
	for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		// If the value at [LoopCount] is less than x and is not zero, total is incremented
		tot += (Vector<Ttype>::GetAt(LoopCount) < _Value) && (Vector<Ttype>::GetAt(LoopCount) != 0);
	}
	return tot;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::NumberZero
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort NumberZero()
{
	ushort tot = 0;
	for (int LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		// If the value at [LoopCount] is zero, total is incremented
		tot += (Vector<Ttype>::GetAt(LoopCount) == 0);
	}
	return tot;
}


 // Calculate how many elements of array totaled together <=value
 //------------------------------------------------------------------
ushort PositionLessThanValue(const Ttype _Value)
{
	ushort tot = 0;
	ushort LoopCount;
	for (LoopCount = 0; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
		tot += Vector<Ttype>::GetAt(LoopCount);
		if (_Value <= tot)
		{
			break;
		}
	}
	return LoopCount;
}


 //---------------------------------------------------------------------------
void RaiseValue(Ttype x, ushort _StartIndex)
{
	EXT_ASSERT(_StartIndex < Vector<Ttype>::GetSize());
	for (int LoopCount = _StartIndex; LoopCount < Vector<Ttype>::GetSize(); LoopCount++)
	{
        Vector<Ttype>::SetAt(LoopCount, Vector<Ttype>::GetAt(LoopCount) + x);
	}
}


 // Get a random index
 //---------------------------------------------------------------------------
ushort RandomPositionOfValue(Ttype value, int nStart = 0, int nEnd = -1)
{
	EXT_ASSERT(Vector<Ttype>::GetSize() > 0);
	if (nEnd == -1)
	{
		nEnd = Vector<Ttype>::GetUpperBound();
	}
	EXT_ASSERT(nEnd >= nStart);
	int nDiff = nEnd - nStart;
	ushort bRet = nStart;
	if (nDiff > 0)
	{
		do
		{
			bRet = nStart + RandomNumber.IntLessThan(nDiff);
		}
		while(Vector<Ttype>::GetAt(bRet) != value);
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::RemoveIfFoundInArray2
	Access:    	public
	Parameter: 	CNumericArray<Ttype> * Array2Ptr
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void RemoveIfFoundInArray2 (CNumericArray<Ttype>* Array2Ptr)
{
	Ttype ValueToFind;
	for (int LoopCount = 0; LoopCount < Array2Ptr->GetSize(); LoopCount++)
	{
		ValueToFind = Array2Ptr->GetAt(LoopCount);
		for (int ListPos = 0; ListPos < Vector<Ttype>::GetSize(); ListPos++)
		{
			if (Vector<Ttype>::GetAt(ListPos) == ValueToFind)
			{
				Vector<Ttype>::RemoveAt(ListPos);
				break;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Swap
	Access:    	public 
	Parameter: 	const ushort _Index1
	Parameter: 	const ushort _Index2
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void Swap(const ushort _Index1, const ushort _Index2)
{
	Ttype Val = Vector<Ttype>::GetAt(_Index1);
	Vector<Ttype>::SetAt(_Index1, Vector<Ttype>::GetAt(_Index2));
	Vector<Ttype>::SetAt(_Index2, Val);
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::TotalSum
	Access:    	public 
	Parameter: 	const ushort _StartIndex
	Parameter: 	ushort _EndIndex
	Returns:   	uint - the cumulative total of the values within the array
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint TotalSum(const ushort _StartIndex = 0, ushort _EndIndex = 0)
{
	EXT_ASSERT(_StartIndex < Vector<Ttype>::GetSize());
	uint iRet = 0;
	if (_EndIndex == 0)
	{
		_EndIndex = Vector<Ttype>::GetSize();
	}
	EXT_ASSERT(_StartIndex < _EndIndex);
	for (int ListPos = _StartIndex; ListPos < _EndIndex; ListPos++)
	{
		iRet += Vector<Ttype>::GetAt(ListPos);
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Read(CDataFile* _pDataFile)
{
	EXT_ASSERT(_pDataFile != NULL);
	Vector<Ttype>::RemoveAll();
    ushort nHowMany;
    _pDataFile->Read(&nHowMany, sizeof(nHowMany));
	Read(nHowMany, _pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Read
	Access:    	public
	Parameter: 	ushort _HowMany
	Parameter: 	CDataFile * File
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Read(ushort _HowMany, CDataFile* File)
{
	EXT_ASSERT(File != null);
	Vector<Ttype>::SetSize(_HowMany);
	Ttype nTemp;
    for (int nCounter = 0; nCounter < Vector<Ttype>::GetSize(); nCounter++)
	{
        File->Read(&nTemp, sizeof(nTemp));
		Vector<Ttype>::SetAt(nCounter, nTemp);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::ReadFromResource
	Access:    	public
	Parameter: 	ushort _HowMany
	Parameter: 	CBinaryReader & _Resourcce
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void ReadFromResource(ushort _HowMany, CBinaryReader& _Resourcce)
{
	Vector<Ttype>::SetSize(_HowMany);
	Ttype nTemp;
    for (int nCounter = 0; nCounter < Vector<Ttype>::GetSize(); nCounter++)
	{
    int typeSize = sizeof(Ttype);

    switch(typeSize)
		{
			case 1:
				nTemp = _Resourcce.ReadByte();
				break;
			case 2:
				nTemp = _Resourcce.ReadUInt16();
				break;
			case 4:
				nTemp = _Resourcce.ReadUInt32();
				break;
			default:
				EXT_ASSERT(false);
				nTemp = 0;
				break;
		}
		Vector<Ttype>::SetAt(nCounter, nTemp);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::SetSize
	Access:    	public
	Parameter: 	const ushort _HowMany
	Parameter: 	Ttype _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void SetSize(const ushort _HowMany, Ttype _Value = 0)
{
	Vector<Ttype>::SetSize(_HowMany, _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CNumericArray<Ttype>::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Write(CDataFile* File)
{
	EXT_ASSERT(File != null);
    ushort nHowMany = Vector<Ttype>::GetSize();
    File->Write(&nHowMany, sizeof(nHowMany));
	Ttype nTemp;
    for (int nCounter = 0; nCounter < nHowMany; nCounter++)
	{
		nTemp = Vector<Ttype>::GetAt(nCounter);
        File->Write(&nTemp, sizeof(Ttype));
    }
}
};
#endif
