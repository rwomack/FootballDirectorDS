
/*

	File:- News.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

CNewsItem::NewsPreprocessPlayerNameLinkFunc CNewsItem::ms_NewsPreprocessPlayerNameLink = NULL;
CNewsItem::NewsPreprocessClubNameLinkFunc CNewsItem::ms_NewsPreprocessClubNameLink = NULL;

CString CNewsItem::PreprocessPlayerName(CPlayer *pPlayer)
{
	if (ms_NewsPreprocessPlayerNameLink != NULL)
		return (*ms_NewsPreprocessPlayerNameLink)(pPlayer);

	return pPlayer->GetName();
}

CString CNewsItem::PreprocessClubName(CClub *pClub)
{
	if (ms_NewsPreprocessClubNameLink != NULL)
		return (*ms_NewsPreprocessClubNameLink)(pClub);

	return pClub->GetName();
}

void CNewsItem::SetPlayerNamePreprocessFunc(NewsPreprocessPlayerNameLinkFunc cPreprocessor)
{
	ms_NewsPreprocessPlayerNameLink = cPreprocessor;
}

void CNewsItem::SetClubNamePreprocessFunc(NewsPreprocessClubNameLinkFunc cPreprocessor)
{
	ms_NewsPreprocessClubNameLink = cPreprocessor;
}




/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem()
	: m_TheDate()
    , m_Type(NEWSMAX)
    , m_bUserRelated(false)
{
	m_Message.Empty();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public
	Parameter: 	const CExtString & _TheNews
	Parameter: 	eNewsType _Type
	Parameter: 	CCalendar & _Thedate
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem(const CExtString& _TheNews, eNewsType _Type, CCalendar& _Thedate, bool bUserRelated )
	: m_TheDate(_Thedate)
    , m_Type(_Type)
    , m_bUserRelated( bUserRelated)
{
    m_Message = _TheNews;
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public
	Parameter: 	int TheNews
	Parameter: 	eNewsType type
	Parameter: 	CCalendar & Thedate
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem(int TheNews, eNewsType type, CCalendar& Thedate, bool bUserRelated)
	: m_TheDate(Thedate)
    , m_Type(type)
    , m_bUserRelated( bUserRelated)
{
    m_Message.LoadString(TheNews);
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public
	Parameter: 	const CExtString & TheNews
	Parameter: 	CCalendar & Thedate
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem(const CExtString& TheNews, CCalendar& Thedate, bool bUserRelated)
	: m_TheDate(Thedate)
	, m_Type(NEWSALL)
	, m_bUserRelated( bUserRelated)
{
	m_Message = TheNews;
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::Read
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsItem::Read(CDataFile* _pDataFile)
{
    m_Message.Read(_pDataFile);
    _pDataFile->Read(&m_Type, sizeof(m_Type));
    _pDataFile->Read(&m_TheDate, sizeof(CCalendar));
    _pDataFile->Read(&m_bUserRelated, sizeof(bool));
//	g_DebugStr.Format(_T("Type : %d, Date %s, %s"), m_Type, m_TheDate.DayMonthStr(), m_Message);
//	g_DebugStr.OutputDebug();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::Write
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsItem::Write(CDataFile* _pDataFile)
{
    m_Message.Write(_pDataFile);
    _pDataFile->Write(&m_Type, sizeof(m_Type));
    _pDataFile->Write(&m_TheDate, sizeof(CCalendar));
    _pDataFile->Write(&m_bUserRelated, sizeof(bool));
//	g_DebugStr.Format(_T("Type : %d, Date %s, %s"), m_Type, m_TheDate.DayMonthStr(), m_Message);
//	g_DebugStr.OutputDebug();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::DoBuildDisplayList
	Access:    	public
	Parameter: 	CStringArray & _TheList
	Parameter: 	const bool _bAddType
	Parameter: 	CString & _TheDate
	Parameter: 	const bool _bReverse
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsItem::DoBuildDisplayList(CStringArray& _TheList, const bool _bAddType, CString& _TheDate, const bool _bReverse)
{
	CString  Str;

// NOTE: Unused
//  static int NewsHeaders[] =
//  {
//    IDS_NEWSTRANSFER,
//    IDS_NEWSFEDERATIONHEADQUARTERS,
//    IDS_NEWSCOUNTRYHEADQUARTERS,
//    IDS_NEWSDISCIPLINE,
//    IDS_NEWSINJURY,
//    IDS_NEWSMANAGMENT,
//    IDS_NEWSCLUB,
//    IDS_NEWSPLAYER,
//  };

	// Pass null date string to add all news
	if (_TheDate == GetDateStr() || _TheDate.IsEmpty() == TRUE)
	{
		if (_bAddType == true)
		{
			EXT_ASSERT(false);
			//Str.LoadString(NewsHeaders[GetType()]);
			//Str += " ";
		}
		Str = GetDateStr();
		if (_bReverse == false)
		{
			_TheList.Add(Str);
			_TheList.Add(GetMessage());
		}
		else
		{
			_TheList.Add(GetMessage());
			_TheList.Add(Str);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::DoBuildUserRelatedList
	Access:    	public
	Parameter: 	lArray<bool>& _TheList
	Parameter: 	CString & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsItem::DoBuildUserRelatedList(lArray<bool>& _TheList, CString& _TheDate)
{
	CString  Str;

// NOTE: Unused
//  static int NewsHeaders[] =
//  {
//    IDS_NEWSTRANSFER,
//    IDS_NEWSFEDERATIONHEADQUARTERS,
//    IDS_NEWSCOUNTRYHEADQUARTERS,
//    IDS_NEWSDISCIPLINE,
//    IDS_NEWSINJURY,
//    IDS_NEWSMANAGMENT,
//    IDS_NEWSCLUB,
//    IDS_NEWSPLAYER,
//  };

	// Pass null date string to add all news
	if (_TheDate == GetDateStr() || _TheDate.IsEmpty() == TRUE)
	{
		_TheList.Add(1);
    _TheList.Last() = IsUserRelated();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::GetMessage
	Access:    	public
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const CString& CNewsItem::GetMessage() const
{
	return m_Message;
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::GetDateStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CNewsItem::GetDateStr()
{
	return m_TheDate.LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::GetType
	Access:    	public
	Returns:   	eNewsType
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eNewsType CNewsItem::GetType() const
{
	return m_Type;
}

const CCalendar &CNewsItem::GetDate() const
{
	return m_TheDate;
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::IsUserRelated
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CNewsItem::IsUserRelated()
{
  return m_bUserRelated;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CNewsList::~CNewsList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CNewsList::~CNewsList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsList::Read
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	ushort LoopCount;
	ushort HowMany;
    _pDataFile->Read(&HowMany, sizeof(HowMany));
	g_DebugStr.Format(_T("New Item Count %d"), HowMany);
	g_DebugStr.OutputDebug();
	// Ensure the list is the correct size
	if (HowMany > GetSize())
	{
		// Add items
		SetSize(HowMany);
		for (LoopCount = 0; LoopCount < HowMany; LoopCount++)
		{
            if (GetAt(LoopCount) == null)
			{
				SetAtGrow(LoopCount, FD_ENGINE_NEW CNewsItem());
			}
		}

	}
	if (HowMany < GetSize())
	{
		// Remove items
		for (LoopCount = HowMany; LoopCount < GetSize(); LoopCount++)
		{
            delete  GetAt(LoopCount);
		}
		SetSize(HowMany);
		FreeExtra();
	}
	for (LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        static_cast<CNewsItem*>(GetAt(LoopCount))->Read(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CNewsList::Write
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	ushort LoopCount;
	LoopCount = GetSize();
    _pDataFile->Write(&LoopCount, sizeof(LoopCount));
	g_DebugStr.Format(_T("New Item Count %d"), LoopCount);
	g_DebugStr.OutputDebug();
    for (LoopCount  = 0; LoopCount < GetSize(); LoopCount++)
	{
        static_cast<CNewsItem*> (GetAt(LoopCount))->Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CNewsList::GetCount
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CNewsList::GetCount()
{
	int Count = 0;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (WorldData().GetCurrentDate().LongDateStr() == static_cast<CNewsItem*>(GetAt(ListPos))->GetDateStr())
		{
			Count++;
		}
	}
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CNewsList::DoBuildDisplayList
	Access:    	public
	Parameter: 	CStringArray & _TheList
	Parameter: 	const bool _bAddType
	Parameter: 	CString & _TheDate
	Parameter: 	const bool _bReverse
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsList::DoBuildDisplayList( CStringArray& _TheList, const bool _bAddType, CString& _TheDate, const bool _bReverse )
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		static_cast<CNewsItem*>(GetAt(ListPos))->DoBuildDisplayList(_TheList, _bAddType, _TheDate, _bReverse);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNewsList::DoBuildUserRelatedList
	Access:    	public
	Parameter: 	CStringArray & _TheList
	Parameter: 	const bool _bAddType
	Parameter: 	CString & _TheDate
	Parameter: 	const bool _bReverse
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsList::DoBuildUserRelatedList( lArray<bool>& _TheList, CString& _TheDate )
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		static_cast<CNewsItem*>(GetAt(ListPos))->DoBuildUserRelatedList(_TheList, _TheDate);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNewsList::DoShrinkList
	Access:    	public
	Parameter: 	const ushort MaxEntries
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CNewsList::DoShrinkList(const ushort MaxEntries)
{
	while(GetSize() > MaxEntries)
	{
        delete static_cast<CNewsItem*>(GetAt(0));
		RemoveAt(0);
	}
}
