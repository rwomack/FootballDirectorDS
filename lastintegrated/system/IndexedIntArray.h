
 /*

	File:- IndexedIntArray.h

	(c) RW Software 1994 - 2008

 ---------------------------------------------------------------------------------------
 */


#ifndef __INDEXEDINT_H__
#define __INDEXEDINT_H__


class CIndexedInt
{
	// Variables
protected:
	ushort					m_Index;
    int						m_Value;

public:

    // Constructors
	CIndexedInt(const ushort _ElementNumber, int _Value);
    ~CIndexedInt() {};

	int						GetValue();
	ushort GetIndex();
};
/////////////////////////////////////////////////////////////////////////////


class CIndexedIntArray : public CPointerArray<CIndexedInt>
{
public:
    // Constructor
    CIndexedIntArray() {};
    ~CIndexedIntArray();

	void					AddNew(const ushort _Index, int _Value);
	ushort					GetIndex(const ushort _ElementNumber);
	int						GetValue(const ushort _ElementNumber);
	int						IndexPosition(const ushort _Index);
    void					SortHigh();
    void					SortLow();
	void					Swap(const ushort _ElementNumber1, const ushort _ElementNumber2);
};
#endif
