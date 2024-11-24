
/*

	SortedStringList.h: interface for the SortedStringList class.


	(c) Rw Software 2003-2007

---------------------------------------------------------------
*/


class CIndexedString
{
public:
	CIndexedString(const CString &strString, const ushort nID);

	const CString &			GetStr() const;
	ushort					GetID() const;

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

	ushort					GetSortedID(ushort nID) const;
	int						GetSize() const;
	const CString &			GetSortedStr(ushort nID) const;
	void					Add(const CString &strString, ushort nID);
	void					SortAscending();
	void					RemoveAll();

protected:
	ushort					m_nCount;
	Vector<CIndexedString*> m_arrIndString;
};
