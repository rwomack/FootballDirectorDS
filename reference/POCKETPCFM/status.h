
/*

	StatusTextRegion.h : header file

	Status Screen

	Football Director

	(c) Sports Director Ltd 2008

 ---------------------------------------------------------------

*/


class CClubStatusText
{
	void					OnInitialise(CBasicDisplay* _BasicDisplay, CClub* _Club);
	void					OnSize(CRect& _Rect);

protected:
	CTextRegion				m_TextRegion[4];
};
/////////////////////////////////////////////////////////////////////////////

class CNextMatchText
{
	protected
		CTextRegion				m_TextRegion[4];
}
