// FootballDirectorDoc.cpp : implementation of the CFootballDirectorDoc class
//

#include "stdafx.h"
#include "FootballDirectorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFootballDirectorDoc

IMPLEMENT_DYNCREATE(CFootballDirectorDoc, CDocument)

BEGIN_MESSAGE_MAP(CFootballDirectorDoc, CDocument)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFootballDirectorDoc construction/destruction

CFootballDirectorDoc::CFootballDirectorDoc()
{
}

CFootballDirectorDoc::~CFootballDirectorDoc()
{
}

BOOL CFootballDirectorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}
