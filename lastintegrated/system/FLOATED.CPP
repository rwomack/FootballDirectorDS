// floated.cpp : implementation of the CFloatEdit class.
//
#include "stdafx.h"
#include "afxwin.h"
#include "floated.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFloatSIPEdit, CFloatEdit)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFloatSIPUnderLinedEdit, CFloatSIPEdit)
    ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS() 
END_MESSAGE_MAP()




// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString str;
	GetWindowText(str);
	dc.SetTextColor(RGB(0, 0, 0,));
	dc.ExtTextOut(0, 0, 0, CRect(0, 0, 200, 15), str, str.GetLength(), NULL);
	DrawUnderLine(&dc);
}


// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
   Default(); 
}


// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::OnRecognizeGesture(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = TRUE; 
} 


// ---------------------------------------------------------------------------
BOOL CFloatSIPUnderLinedEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect ClientRect;

	GetClientRect(&ClientRect);
	pDC->ExtTextOut(0, 0, ETO_OPAQUE, ClientRect, NULL, 0, NULL);
	DrawUnderLine(pDC);

	return TRUE;
}


// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::DrawUnderLine(CDC* pDC)
{
	CPen borderpen(PS_SOLID, 1, m_colorUnderLine);
	CPen* oldPen = pDC->SelectObject(&borderpen);
	CRect rc;
	// Get the client area of the edit control
	GetClientRect(&rc);
	pDC->MoveTo(0, m_LineY);
	pDC->LineTo(rc.right, m_LineY);		// Top
	pDC->SelectObject(oldPen);
}


// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	Invalidate(FALSE);
}


// ---------------------------------------------------------------------------
void CFloatSIPUnderLinedEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// Force a repaint
	Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------
void CFloatSIPEdit::OnSetFocus(CWnd* pOldWnd)
{
	SHSipPreference(CWnd::GetSafeHwnd(), SIP_UP);
	CEdit::OnSetFocus(pOldWnd);
}
/////////////////////////////////////////////////////////////////////////////


// Message handler member function for the WM_CHAR message. This function will
// be called whenever a character key is hit while the edit control has
// the input focus. The first argument(nChar) will have the character
// code of the hit key.
//---------------------------------------------------------------------------------------
afx_msg void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Check to see if the character is valid for a floating point
	// number(the character must be either a 0-9 or a decimal point) or
	// a control character was entered.
	// If the character is valid, pass it through to the base class
	// message handler, if the character is invalid, drop it

	// If the character is a decimal point, and there is already a decimal point in the string, then exit.
    CString currentString;
    GetWindowText(currentString);
    if (nChar == '.') 
	{
        if (currentString.Find('.') != -1) 
		{
            return;
        }
    }

	if (isdigit(nChar) || (nChar == '.') || (nChar < 0x20))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}

}


// Utility member function for extracting the floating point value
// represented by the text in the edit control.
//---------------------------------------------------------------------------------------
float CFloatEdit::GetValue(bool b2dp)
{
	CString ControlText;

	// Extract the text from the exit control and then parse it into a floating point value.
	GetWindowText(ControlText);
#ifdef UNICODE
	char szValue[20];
	WideCharToMultiByte(CP_ACP, 0, ControlText, -1, szValue, 20, NULL, NULL);
	if (b2dp == true)
	{
		char szNewValue[20];
		NormalizeAmount(szValue, szNewValue, 20);
		return((float) atof(szNewValue));
	}
	else
	{
		return((float) atof(szValue));
	}
#else
	return((float) atof(ControlText));
#endif
}


//---------------------------------------------------------------------------------------
void CFloatEdit::GetszValue(char* szDest, int iDestBufSize)
{
	CString ControlText;
	GetWindowText(ControlText);
#ifdef UNICODE
	char szValue[20];
	WideCharToMultiByte(CP_ACP, 0, ControlText, -1, szValue, 20, NULL, NULL);
	NormalizeAmount(szValue, szDest, iDestBufSize);
#else
	strcpy(szDest, ControlText);
#endif
}


// Fix formatting of an amount string
//	no leading zeros, only one decimal
//	two digits (exactly) after the decimal
//	if bBlankZero=true, then zero=blank, else zero="0"
//---------------------------------------------------------------------------------------
char* CFloatEdit::NormalizeAmount(char* szAmt, char* szNormalizedAmt, int iDestBufSize, bool bBlankZero, bool bAllowNegative)
{
	int iAllowedLength;
	bool bHasDecimal;
	bool bSawDigit, bSawNegative;
	int iLen, iDecimalLoc;
	char *szSrc, *szDest;


	bSawDigit=false;
	bSawNegative=false;

	//make sure there's room in the buffer for the compulsory ".00"
	iAllowedLength=iDestBufSize;
	bHasDecimal=strchr(szAmt,'.')?true:false;
	if(!bHasDecimal)
		iAllowedLength-=3;

	szSrc=szAmt;
	szDest=szNormalizedAmt;
	iLen=0;
	iDecimalLoc=-1;
	memset(szNormalizedAmt,0,iDestBufSize);

	while(*szSrc && iLen<iAllowedLength)
	{

		if(isdigit(*szSrc) && (iDecimalLoc==-1 || (iLen-iDecimalLoc <=2)))
		{//pass digits so long as there aren't more than two after a decimal
		 //fix 2.01 - added clause to pass zeros if we've seen a decimal
			if(bSawDigit || *szSrc!='0' || iDecimalLoc!=-1/*2.01*/)
			{//strip leading zeros
				bSawDigit=true;
				*szDest=*szSrc;
				szDest++;
				iLen++;
			}
		}
		else if (*szSrc == '.' && (iDecimalLoc==-1))
		{//allow only one decimal and track where it was.
			*szDest=*szSrc;
			iDecimalLoc=iLen;
			szDest++;
			iLen++;
		}
		else if (bAllowNegative && *szSrc=='-' && !bSawDigit &&  !bSawNegative && (iDecimalLoc==-1) )
		{//pass only the first negative (if allowed), and only if no digits have passed already
			bSawNegative=true;
			*szDest=*szSrc;
			szDest++;
			iLen++;
		}
		szSrc++;
	}
	//__try{
	//strcpy(szNormalizedAmt,szTemp);

	//if no decimal, add ".00"  set to "0" if empty
	if(!bHasDecimal && iLen>0)
		strcat(szNormalizedAmt,".00");
	else if(iLen==0 && !bBlankZero)
		strcpy(szNormalizedAmt,"0");

	if(bBlankZero && iLen==1 && *szNormalizedAmt=='0')
		*szNormalizedAmt=0;

	//make sure we have two digits after decimal
	if(bHasDecimal)
	{
		if( (iLen-iDecimalLoc)==1 ) //nothing after the decimal
			strcat(szNormalizedAmt,"00");
		else if ((iLen-iDecimalLoc)==2) //one digit after the decimal
			strcat(szNormalizedAmt,"0");
	}


	return szNormalizedAmt;

}


// Conversion from CFloatEdit to float.
//---------------------------------------------------------------------------------------
CFloatEdit::operator float()
{
	return(GetValue());
}

