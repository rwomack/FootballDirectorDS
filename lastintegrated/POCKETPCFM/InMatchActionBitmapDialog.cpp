/*

	InMatchActionBitmapDialog.cpp

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"


BEGIN_MESSAGE_MAP(CInMatchActionBitmapDialog, CBitmapDialog)
	ON_COMMAND(ID_ACTION1, OnMakeSubstitution)
	ON_COMMAND(ID_ACTION2, OnTactics)
	ON_COMMAND(ID_ACTION3, OnFormation)
	ON_COMMAND(ID_ACTION4, OnRole)
	ON_COMMAND(ID_ACTION5, OnOK)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::CInMatchActionBitmapDialog
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParentWnd
	Returns:   	
	Qualifier: 	: CBitmapDialog(nIDTemplate, pParentWnd)
	Purpose:
------------------------------------------------------------------------------*/
CInMatchActionBitmapDialog::CInMatchActionBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
	: CBitmapDialog(nIDTemplate, pParentWnd)
{

}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::CInMatchActionBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInMatchActionBitmapDialog::CInMatchActionBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::~CInMatchActionBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInMatchActionBitmapDialog::~CInMatchActionBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnOK()
{	
	EndDialog(SUBS_MATCH);
}


/*------------------------------------------------------------------------------
Method:   	CInMatchActionBitmapDialog::OnMakeSubstitution
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnMakeSubstitution()
{
	EndDialog(SUBS_SUBS);
}


/*------------------------------------------------------------------------------
Method:   	CInMatchActionBitmapDialog::OnTactics
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnTactics()
{
	EndDialog(SUBS_TACTICS);
}


/*------------------------------------------------------------------------------
Method:   	CInMatchActionBitmapDialog::OnFormation
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnFormation()
{
	EndDialog(SUBS_FORMATION);
}


/*------------------------------------------------------------------------------
Method:   	CInMatchActionBitmapDialog::OnRole
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnRole()
{
	EndDialog(SUBS_ROLES);
}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::OnInitDialog
	Access:    	protected 
	Parameter: 	const int _HelpTopic
	Parameter: 	int _StrID
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CInMatchActionBitmapDialog::OnInitDialog(const int _HelpTopic, int _StrID)
{
	return CBitmapDialog::OnInitDialog(_HelpTopic, _StrID);
}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			ReDraw();
			break;

	}
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CInMatchActionBitmapDialog::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInMatchActionBitmapDialog::OnPaint()
{
	CBitmapDialog::OnPaint(true, false);
	CBitmapDialog::DoBlit();
}
