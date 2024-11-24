
 /*

	File:- IndexedIntArray.h

	(c) RW Software 1994 - 2001

 ---------------------------------------------------------------------------------------
 */


#ifndef __INDEXEDSTRING_H__
#define __INDEXEDSTRING_H__


class CIndexedString
{
	// Variables
protected:
	int						m_Index;
    CString					m_Value;
    CString					m_FirstUpperValue;

public:

    // Constructors
    CIndexedString(int Index, CString Value);
    ~CIndexedString(void) {};

    CString					GetValue(void)						{ return m_Value; }
    CString					GetUpperValue(void)					{ return m_FirstUpperValue; }
    int						GetIndex(void)						{ return m_Index; }
};
/////////////////////////////////////////////////////////////////////////////


class CIndexedStringArray : public CPointerArray
{
public:
    // Constructor
    CIndexedStringArray(void) {};
    ~CIndexedStringArray(void);

    void					AddNew(const int Index, CString Value);
    CIndexedString*         GetAt(const int x)                       
	{ 
		return static_cast<CIndexedString*> (CPointerArray::GetAt(x)); 
	}
    int						GetIndex(const int Index);
    CString					GetValue(const int Index);
	int						IndexPosition(const int iIndex);
    void					SortHigh(void);
    void					SortLow(void);
    void					Swap(int n1, int n2);
};
#endif
