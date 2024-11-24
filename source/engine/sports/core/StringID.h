
/*

	Class - StringID class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __CLUBNAME_H__
#define __CLUBNAME_H__
	
class CStringID
{
public:
	CStringID()
		: m_ID(wNOTFOUND) {};

	CStringID&				operator = (const CStringID& p);
	bool					operator == (const CStringID& p);

	void					DoFindID(const CString &strName);
	void					DoReset();
	ushort					GetID() const;
	CString&				GetStr() const;
	bool					IsEmpty();
	void					SetID(const ushort wID);

protected:
	ushort					m_ID;
};
#endif