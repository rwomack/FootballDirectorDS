
 /*

	File:- IndexedIntArray.cpp

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------------------------------
 */


 #include    "stdafx.h"
 #include    "IndexedIntArray.h"



/*------------------------------------------------------------------------------
	Method:   	CIndexedInt::CIndexedInt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Parameter: 	int _Value
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CIndexedInt::CIndexedInt(const ushort _ElementNumber, int _Value)
{
	m_Index = _ElementNumber;
	m_Value = _Value;
}


/*------------------------------------------------------------------------------
Method:   	CIndexedInt::GetIndex
Access:    	public 
Parameter: 	void
Returns:   	const ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CIndexedInt::GetIndex()
{ 
	return m_Index; 
}


/*------------------------------------------------------------------------------
Method:   	CIndexedInt::GetValue
Access:    	public 
Parameter: 	void
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int CIndexedInt::GetValue()						
{ 
	return m_Value; 
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::~CIndexedIntArray
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CIndexedIntArray::~CIndexedIntArray()
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++) 
	{
        if (static_cast<CIndexedInt*> (GetAt(ListPos))) 
		{
            delete static_cast<CIndexedInt*> (GetAt(ListPos));
        }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::GetIndex
	Access:    	public 
	Parameter: 	ushort _ElementNumber
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CIndexedIntArray::GetIndex(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < GetSize());
	return static_cast<CIndexedInt*> (GetAt(_ElementNumber))->GetIndex();
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::GetValue
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CIndexedIntArray::GetValue(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < GetSize());
	return GetAt(_ElementNumber)->GetValue();
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::SortHigh
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIndexedIntArray::SortHigh()
{
    CIndexedInt* Table1Ptr;
    CIndexedInt* Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int LoopCount = 0; LoopCount < GetSize() - 1; LoopCount++)
		{
            Table1Ptr = static_cast<CIndexedInt*> (GetAt(LoopCount));
            Table2Ptr = static_cast<CIndexedInt*> (GetAt(LoopCount + 1));
            if (Table1Ptr->GetValue() < Table2Ptr->GetValue())
			{
				Swap(LoopCount, LoopCount + 1);
				t_Swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::SortLow
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIndexedIntArray::SortLow()
{
    CIndexedInt* Table1Ptr;
    CIndexedInt* Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int LoopCount = 0; LoopCount < GetSize() - 1; LoopCount++)
		{
            Table1Ptr = static_cast<CIndexedInt*> (GetAt(LoopCount));
            Table2Ptr = static_cast<CIndexedInt*> (GetAt(LoopCount + 1));
            if (Table1Ptr->GetValue() > Table2Ptr->GetValue())
			{
				Swap(LoopCount, LoopCount + 1);
				t_Swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::AddNew
	Access:    	public 
	Parameter: 	const ushort _Index
	Parameter: 	int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIndexedIntArray::AddNew(const ushort _Index, int _Value)
{
    Add(new CIndexedInt(_Index, _Value));
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::Swap
	Access:    	public 
	Parameter: 	const ushort _ElementNumber1
	Parameter: 	const ushort _ElementNumber2
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIndexedIntArray::Swap(const ushort _ElementNumber1, const ushort _ElementNumber2)
{
    CIndexedInt* pTable;
    pTable = GetAt(_ElementNumber1);
	SetAt(_ElementNumber1, GetAt(_ElementNumber2));
	SetAt(_ElementNumber2, pTable);
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedIntArray::IndexPosition
	Access:    	public 
	Parameter: 	const ushort _Index
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CIndexedIntArray::IndexPosition(const ushort _Index)
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
	EXT_ASSERT(iRet != NOTFOUND);
	return iRet;
}
