
 /*

	File:- IndexedStringArray.cpp

	(c) RW Software 2003

 ---------------------------------------------------------------------------------------
 */


 #include    "stdafx.h"
 #include    "PointerArray.h"
 #include    "IndexedStringArray.h"



 //---------------------------------------------------------------------------
CIndexedString::CIndexedString(int Index, CString Value)
{
	m_Index = Index;
	m_Value = Value;
	// This is for internal use only, the first letter of the string is upper case, 
	// to enable the sort routines to sort correctly
	m_FirstUpperValue = Value;
	CString strFirst = m_FirstUpperValue.Left(1);
	strFirst.MakeUpper();
	m_FirstUpperValue = strFirst + m_FirstUpperValue.Right(m_FirstUpperValue.GetLength() - 1);
}
/////////////////////////////////////////////////////////////////////////////


 //---------------------------------------------------------------------------
CIndexedStringArray::~CIndexedStringArray(void)
{
    for (int nListPos = 0; nListPos < GetSize(); nListPos++) 
	{
        if (static_cast<CIndexedString*> (GetAt(nListPos))) 
		{
            delete static_cast<CIndexedString*> (GetAt(nListPos));
        }
    }
}


 //---------------------------------------------------------------------------
int CIndexedStringArray::GetIndex(const int Index)
{
	ASSERT (Index > -1 && Index < GetSize());
	return static_cast<CIndexedString*> (GetAt(Index))->GetIndex();
}


 //---------------------------------------------------------------------------
CString CIndexedStringArray::GetValue(const int Index)
{
	ASSERT (Index > -1 && Index < GetSize());
	return static_cast<CIndexedString*> (GetAt(Index))->GetValue();
}


 //---------------------------------------------------------------------------
void CIndexedStringArray::SortHigh(void)
{
    CIndexedString* Table1Ptr;
    CIndexedString* Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int nLoopCount = 0; nLoopCount < GetSize() - 1; nLoopCount++)
		{
            Table1Ptr = static_cast<CIndexedString*> (GetAt(nLoopCount));
            Table2Ptr = static_cast<CIndexedString*> (GetAt(nLoopCount + 1));
            if (Table1Ptr->GetUpperValue() > Table2Ptr->GetUpperValue())
			{
				Swap(nLoopCount, nLoopCount + 1);
				t_Swap = true;
			}
		}
	}
}


 //---------------------------------------------------------------------------
void CIndexedStringArray::SortLow(void)
{
    CIndexedString* Table1Ptr;
    CIndexedString* Table2Ptr;
    bool t_Swap = true;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (int nLoopCount = 0; nLoopCount < GetSize() - 1; nLoopCount++)
		{
            Table1Ptr = static_cast<CIndexedString*> (GetAt(nLoopCount));
            Table2Ptr = static_cast<CIndexedString*> (GetAt(nLoopCount + 1));
			if (Table1Ptr->GetValue().CompareNoCase(Table2Ptr->GetValue()) > 0)
            //if (Table1Ptr->GetUpperValue() > Table2Ptr->GetUpperValue())
			{
				Swap(nLoopCount, nLoopCount + 1);
				t_Swap = true;
			}
		}
	}
}


 //---------------------------------------------------------------------------
void CIndexedStringArray::AddNew(const int Index, CString Value)
{
    Add(new CIndexedString(Index, Value));
}


 //---------------------------------------------------------------------------
void CIndexedStringArray::Swap(int n1, int n2)
{
    CIndexedString* TablePtr;
    TablePtr = GetAt(n1);
	SetAt(n1, GetAt(n2));
	SetAt(n2, TablePtr);
}


 //---------------------------------------------------------------------------
int CIndexedStringArray::IndexPosition(const int iIndex)
{
	int iRet = -1;
	for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
	{
		if (GetAt(nLoopCount)->GetIndex() == iIndex)
		{
			iRet = nLoopCount;
			break;
		}
	}
	ASSERT (iRet != -1);
	return iRet;

}
