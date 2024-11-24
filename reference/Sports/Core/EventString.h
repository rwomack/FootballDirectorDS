
/*

	File:- ExtString.h

	(c) Rw Software 1997 - 2008

---------------------------------------------------------------------------------------
*/

#ifndef __EVENTSTRING_H__
#define __EVENTSTRING_H__


class CEventString
{
public:
	CEventString() {};

	void Add(int _When, CString _Name);
	CExtString				GetStr();
	void					Empty();

protected :
	CExtString				m_String;
};
#endif