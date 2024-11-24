#if !defined(AFX_NAMETEXTREGION_H__702EA47A_2E5D_4ED3_87A1_EFD93688D0EE__INCLUDED_)
#define AFX_NAMETEXTREGION_H__702EA47A_2E5D_4ED3_87A1_EFD93688D0EE__INCLUDED_

/*

	PlayerNameTextRegion.h: header for the NameTextRegion classes.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CNameTextRegion //: public CTextRegion
{
public:
	CNameTextRegion(void);
	virtual ~CNameTextRegion(void);

	void					DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CString _Name);
	CTextRegion*			GetTextRegion();
	void					MoveWindow();

protected:
	CTextRegion*			m_TextRegion;
	int						m_NameTextID;
	int						m_YPos;
	CFont					m_Font;
};
/////////////////////////////////////////////////////////////////////////////


class CPlayerNameTextRegion : public CNameTextRegion
{
public:
	void					DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CPlayer& _Player);
};
/////////////////////////////////////////////////////////////////////////////


class CEmployeeNameTextRegion : public CNameTextRegion
{
public:
	void					DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CEmployee& _Employee);
};
/////////////////////////////////////////////////////////////////////////////


class CYouthNameTextRegion : public CNameTextRegion
{
public:
	void					DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CYouth& _Youth);
};
/////////////////////////////////////////////////////////////////////////////


class CClubNameTextRegion : public CNameTextRegion
{
public:
	void					DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CClub& _Club);
};
/////////////////////////////////////////////////////////////////////////////


class CClubBalanceTextRegion
{
public:
	CClubBalanceTextRegion();

	void					DoInitialise(CBasicDisplay* _BasicDisplay, CClub* _Club, const int _Style, const bool _Abbreviated = false);
	void					DoUpdate();
	void					MoveWindow(CRect _Rect);
	void					SetFont(CFont* _Font);
	bool					SetTextColour(COLORREF _Colour);

protected:
	CClub*					m_Club;
	CTextRegion*			m_TextRegion;
	bool					m_Abbreviated;
};
#endif