#if !defined (HELPINFO_H_INCLUDED__73F6F4A6_FF9C_4ACB_89BF_D72035E02917_)
#define HELPINFO_H_INCLUDED__73F6F4A6_FF9C_4ACB_89BF_D72035E02917_
#if _MSC_VER > 1000
#pragma once
#endif

#ifdef ORIGINAL_CODE

//==============================================================================

#define AfxSig_bHELPINFO 42

#define ON_WM_HELPINFO() \
	{ WM_HELP, 0, 0, 0, AfxSig_bHELPINFO, \
	(AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(HELPINFO*))&OnHelpInfo },
typedef struct tagHELPINFO
{
}  HELPINFO;
typedef void* LPHELPINFO;

#endif

//==============================================================================
#endif // HELPINFO_H_INCLUDED__73F6F4A6_FF9C_4ACB_89BF_D72035E02917_


