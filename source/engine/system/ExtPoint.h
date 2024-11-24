

/*

	ExtPoint.h: interface for the CExtPoint class.


	Football Director

	(c) Rw Software 1994 - 2007

	---------------------------------------------------------------
*/



#if !defined(AFX_EXTPOINT_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_EXTPOINT_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_

class CExtPoint
{
	// Variables
private:
	ushort					m_X;
	ushort					m_Y;

public:
	CExtPoint(const ushort _x, const ushort _y);
	CExtPoint() {};

	CExtPoint&				operator = (const CExtPoint& _Point);

	// Methods
	ushort					GetX() const;
	ushort					GetY() const;
	void					SetX(const ushort _x);
	void					SetY(const ushort _x);
};
#endif