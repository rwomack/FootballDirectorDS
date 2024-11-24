
/*

	SortedStringList.h: interface for the SortedStringList class.


	(c) Rw Software 2003 - 2008

---------------------------------------------------------------
*/

#ifndef SORTEDSTRINGLIST
#define SORTEDSTRINGLIST
class CIndexedString
{
public:
	CIndexedString(CString strString, ushort nID);

	CString					GetStr();
	ushort					GetID();

protected:
	CString					m_strString;
	ushort					m_nID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////

class CSortedStringList
{
public:
	CSortedStringList(); 
	~CSortedStringList(); 

	ushort					GetSortedID(ushort nID);
	int						GetSize();
	CString 				GetSortedStr(ushort nID);
	void					Add(CString strString, ushort nID);
	void					SortAscending();
	void					RemoveAll();

protected:
	ushort					m_nCount;
	Vector<CIndexedString*> m_arrIndString;
};
#endif