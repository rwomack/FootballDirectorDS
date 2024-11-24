
 /*

	File:- IndexedIntArray.h

	(c) RW Software 1994 - 2008

 ---------------------------------------------------------------------------------------
 */


#ifndef __INDEXEDINT_H__
#define __INDEXEDINT_H__

template <typename IndexType>
class CIndexedInt
{
	// Variables
protected:
	IndexType				m_Index;
    int						m_Value;

public:

    // Constructors
	CIndexedInt(const IndexType _ElementNumber, int _Value)
	{
		m_Index = _ElementNumber;
		m_Value = _Value;
	}

    ~CIndexedInt() {};

	int						GetValue() const 					{ return m_Value; }
	IndexType 				GetIndex() const 					{ return m_Index; }
};
/////////////////////////////////////////////////////////////////////////////


template <typename IndexType>
class CIndexedIntArray : public CPointerArray< CIndexedInt<IndexType> >
{
public:
    // Constructor
    CIndexedIntArray() {};
	~CIndexedIntArray()
	{
		RemoveList();
	}

	void					AddNew(const IndexType _Index, int _Value)
	{
		Add(FD_ENGINE_NEW CIndexedInt<IndexType>(_Index, _Value));
	}

	IndexType				GetIndex(const ushort _ElementNumber) { return GetAt(_ElementNumber)->GetIndex(); }
	int						GetValue(const ushort _ElementNumber) { return GetAt(_ElementNumber)->GetValue(); }
	int						IndexPosition(const IndexType _Index);
    void					SortHigh();
    void					SortLow();
	void					Swap(const ushort _ElementNumber1, const ushort _ElementNumber2);
};


/////////////////////////////////////////////////////////////////////////////

template <typename IndexType>
void CIndexedIntArray<IndexType>::SortHigh()
{
    CIndexedInt<IndexType> *Table1Ptr;
    CIndexedInt<IndexType> *Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int LoopCount = 0; LoopCount < GetSize() - 1; LoopCount++)
		{
            Table1Ptr = GetAt(LoopCount);
            Table2Ptr = GetAt(LoopCount + 1);
            if (Table1Ptr->GetValue() < Table2Ptr->GetValue())
			{
				Swap(LoopCount, LoopCount + 1);
				t_Swap = true;
			}
		}
	}
}

template <typename IndexType>
void CIndexedIntArray<IndexType>::SortLow()
{
    CIndexedInt<IndexType> *Table1Ptr;
    CIndexedInt<IndexType> *Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int LoopCount = 0; LoopCount < GetSize() - 1; LoopCount++)
		{
            Table1Ptr = GetAt(LoopCount);
            Table2Ptr = GetAt(LoopCount + 1);
            if (Table1Ptr->GetValue() > Table2Ptr->GetValue())
			{
				Swap(LoopCount, LoopCount + 1);
				t_Swap = true;
			}
		}
	}
}

template <typename IndexType>
void CIndexedIntArray<IndexType>::Swap(const ushort _ElementNumber1, const ushort _ElementNumber2)
{
    CIndexedInt<IndexType> *pTable = GetAt(_ElementNumber1);
	SetAt(_ElementNumber1, GetAt(_ElementNumber2));
	SetAt(_ElementNumber2, pTable);
}

template <typename IndexType>
int CIndexedIntArray<IndexType>::IndexPosition(const IndexType _Index)
{
	int iRet = NOTFOUND;
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		if (GetAt(LoopCount)->GetIndex() == _Index)
		{
			iRet = LoopCount;
			break;
		}
	}
	FDASSERT(iRet != NOTFOUND);//, "Index not found");
	return iRet;
}
#endif
