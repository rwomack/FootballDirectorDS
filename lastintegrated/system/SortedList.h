
/*

	SortedStringList.h: interface for the SortedStringList class.


	(c) Rw Software 2003-2007

---------------------------------------------------------------
*/

#ifndef SORTEDSTRINGLIST

class CIndexedString
{
public:
	CIndexedString(CString strString, ushort nID);

	CString					GetData();
	ushort					GetID();

protected:
	CString					m_strString;
	ushort					m_nID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////

//template <class Ttype> class CSortedList : public Vector<Ttype>
class CSortedList
{
public:
	CSortedList<Ttype>(); 
	~CSortedList(); 

	ushort					GetSortedID(ushort nID);
	int						GetSize();
	CString 				GetSortedStr(ushort nID);
	void					Add(CString strString, ushort nID);
	void					SortAscending();
	void					RemoveAll();

protected:
	ushort					m_nCount;
	Vector<Ttype>			m_arrIndString;
};
#endif