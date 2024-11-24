#if !defined(AFX_SCREEN90_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
#define AFX_SCREEN90_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_


 /*

    Class:- CScreen90 - Load or Save game


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------

*/


/////////////////////////////////////////////////////////////////////////////
// CScreen90 dialog


class CScreen90Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen90Dlg(CWnd* pParent = null);   // standard constructor

// Dialog Data
	enum
	{
		IDD = IDD_SCREENDLGNOCONTROLS,
		NUMONSCREEN = 4,
		GRID_WIDTH = 220,
	};

	enum eDIALOG90
	{
		eLOADORSAVE,

		eBUTTONS
	};

// Implementation
protected:
	CCalendar				m_CurrentDate;
	CSaveGameFile			m_SaveGame[4];
	CPNGDrawInfo			m_pngLargeKits;

	void					DoBuildList();
	void					OnPaint();
	virtual BOOL			OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnOK();
	void					OnSize(UINT nType, int cx, int cy);

	virtual void			OnLoadOrSave(const int _FileNum) = 0;
	virtual void			OnAction1Clicked() = 0;
	virtual void			OnAction2Clicked() = 0;

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CScreen90Load : public CScreen90Dlg
{

protected:
	virtual BOOL			OnInitDialog();
	virtual void			OnLoadOrSave(const int _FileNum);
	virtual void			OnAction1Clicked();
	virtual void			OnAction2Clicked();
};
/////////////////////////////////////////////////////////////////////////////

class CScreen90Save : public CScreen90Dlg
{

protected:
	virtual BOOL			OnInitDialog();
	virtual void			OnLoadOrSave(const int _FileNum);
	virtual void			OnAction1Clicked();
	virtual void			OnAction2Clicked();
};

#endif // !defined(AFX_SCREEN90_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
