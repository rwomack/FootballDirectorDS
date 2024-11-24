
 /*

	Screen19Display.cpp : League Fixtures implementation file

	Football Director

	Platform:- PocketPC

	(c) Rw Software 1994 - 2007

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "..\system\KeyScrollBar.h"
#include "..\system\Screen\Grid.h"
#include "DivisionListBox.h"

#include "Screen19Display.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::CScreen19Display
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen19Display::CScreen19Display()
{
	m_TheDate.DoInitialise(WorldData().GetCurrentDate());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::~CScreen19Display
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen19Display::~CScreen19Display()
{
	m_ResultList.RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::GetDivisionList
	Access:    	public 
	Parameter: 	void
	Returns:   	CDivisionListBox&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionListBox& CScreen19Display::GetDivisionList()
{
	return m_DivisionList;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::GetTheDate
	Access:    	public 
	Parameter: 	void
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar&	CScreen19Display::GetTheDate()
{
	return m_TheDate;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::GetResultList
	Access:    	public 
	Parameter: 	void
	Returns:   	CSoccerResultList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultList& CScreen19Display::GetResultList()
{
	return m_ResultList;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::OnDraw
	Access:    	public 
	Parameter: 	CRect & _rRect
	Parameter: 	COffScreenBuffer & _rMemDC
	Parameter: 	CGrid & _Grid
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19Display::OnDraw(CRect& _rRect, COffScreenBuffer& _rMemDC, CGrid& _Grid)
{
	CDivision& pDivision = WorldData().GetDivisionList().GetAt(m_DivisionList.GetCurSel());
	switch(pDivision.GetMatchNumber(m_TheDate.GetMatchNumber()))
	{
		case DIVFRIENDLYMATCH1:
		case DIVFRIENDLYMATCH2:
		case DIVFRIENDLYMATCH3:
		case DIVFRIENDLYMATCH4:
			if (m_ResultList.GetSize() == 0)
			{
				_rMemDC.DisplayNoData(IDS_FRIENDLYMATCH);
			}
			else
			{
				DisplayMatches(_rRect, _rMemDC, _Grid);
			}
			break;
	
		case DIVPLAYOFFMATCH1:
		case DIVPLAYOFFMATCH2:
		case DIVPLAYOFFMATCH3:
		case DIVPLAYOFFMATCH4:
		case DIVPLAYOFFMATCH5:
		case DIVPLAYOFFMATCH6:
		case DIVPLAYOFFMATCH7:
		case DIVPLAYOFFMATCH8:
		case DIVPLAYOFFMATCH9:
		case DIVPLAYOFFMATCH10:
		case DIVPLAYOFFMATCH11:
		case DIVPLAYOFFMATCH12:
		case DIVPLAYOFFMATCH13:
		case DIVPLAYOFFMATCH14:
		case DIVPLAYOFFMATCH15:
		case DIVPLAYOFFMATCH16:
		case DIVPLAYOFFMATCH17:
		case DIVPLAYOFFMATCH18:
		case DIVPLAYOFFMATCH19:
		case DIVPLAYOFFMATCH20:
			if (m_ResultList.GetSize() == 0)
			{
				_rMemDC.DisplayNoData(IDS_PLAYOFFS);
			}
			else
			{
				DisplayMatches(_rRect, _rMemDC, _Grid);
			}
			break;
	
		case 0:
		case DIVPLAYOFFINIT:
		case DIVENDSEASON:
		case DIVNEWSEASON:
		case DIVCLOSESEASON:
		case DIVTRAININGONLY:
		case DIVNOMATCH:
			_rMemDC.DisplayNoData(IDS_NOMATCH);
			break;
	
		default :
			ASSERT (m_ResultList.GetSize() == (uint)(pDivision.GetNumberOfClubsInDivision() / 2));
			DisplayMatches(_rRect, _rMemDC, _Grid);
			break;
	}

	_rRect.top = 244;
	_rMemDC.DisplayTitle(m_TheDate.LongDateStr(), &_rRect, (_rMemDC.GetBufferWidth() /2), BLACK, 16, false, FW_SEMIBOLD);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::DisplayMatches
	Access:    	protected 
	Parameter: 	CRect & _rRect
	Parameter: 	COffScreenBuffer & _rMemDC
	Parameter: 	CGrid & _Grid
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19Display::DisplayMatches(CRect& _rRect, COffScreenBuffer& _rMemDC,  CGrid& _Grid)
{
    COLORREF TextCol;
	m_BasicDisplay->SetFontNormal();
	CString str;
	
    for (int nLoopCount = _Grid.GetVerticalScroll(); nLoopCount < m_ResultList.GetSize(); nLoopCount++)
	{
        if (nLoopCount < _Grid.GetFirstRowOffBottomScreen())
		{
			if ((nLoopCount - _Grid.GetVerticalScroll()) % 2 == 1)
			{
				_rRect.left = 2;
				_rMemDC.FillSolidRect(_rRect.left, _rRect.top, 236, 16, PALEYELLOW);
				_rMemDC.SetBkMode(TRANSPARENT);
			}

            // Highlight the user's result
			if (m_ResultList.GetAt(nLoopCount)->GetHomeClubDetails().IsUserControlledClub() == true || 
				m_ResultList.GetAt(nLoopCount)->GetAwayClubDetails().IsUserControlledClub() == true)
			{
                TextCol = ORANGE;
            }
            else {
                TextCol = BLUE;
            }
			
			_rMemDC.SetTextColor(TextCol);
			str = m_ResultList.GetAt(nLoopCount)->GetHomeClubDetails().GetName();
			_rRect.left = 102 - _rMemDC.GetTextExtent(str).cx;
			_rMemDC.ExtTextOut(_rRect.left, _rRect.top, 0, _rRect, str, str.GetLength(), null);
			
			_rRect.left = 138;
			str = m_ResultList.GetAt(nLoopCount)->GetAwayClubDetails().GetName();
			_rMemDC.ExtTextOut(_rRect.left, _rRect.top, 0, _rRect, str, str.GetLength(), null);
			
            str = m_ResultList.GetAt(nLoopCount)->GoalsStr();
			_rRect.left = (_rMemDC.GetBufferWidth() /2) - _rMemDC.GetTextExtent(str).cx / 2;
			_rMemDC.ExtTextOut(_rRect.left, _rRect.top, 0, _rRect, str, str.GetLength(), null);
			
			_rRect.top += GAPY;
			_rRect.left = 8;
        }
	}
	m_BasicDisplay->RestoreFont();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::OnInitDialog
	Access:    	public 
	Parameter: 	UINT _ID
	Parameter: 	CWnd * _pParent
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen19Display::OnInitDialog(UINT _ID, CWnd* _pParent, CBasicDisplay* _BasicDisplay)
{
	m_BasicDisplay = _BasicDisplay;
	return m_DivisionList.OnInitDialog(_ID, _pParent);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::DoBuildList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19Display::DoBuildList()
{
	m_ResultList.RemoveList();
	WorldData().GetDivisionList().GetAt(m_DivisionList.GetCurSel()).DoBuildLeagueFixtureList(m_TheDate, m_ResultList);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19Display::OnLButtonUp
	Access:    	public 
	Parameter: 	const ushort _Column
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19Display::OnLButtonUp(const ushort _Column)
{
	if (_Column != wNOTFOUND)
	{
		if (m_ResultList.GetAt(_Column)->GetFixture() != null)
		{
			m_ResultList.GetAt(_Column)->GetFixture()->DoShowSummary(null, WorldData().GetDivisionList().GetAt(m_DivisionList.GetCurSel()).GetName());
		}
	}
}
