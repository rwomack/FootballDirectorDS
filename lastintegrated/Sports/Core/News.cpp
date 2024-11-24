
/*

	File:- News.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



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
{
	m_Message.Empty();
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public 
	Parameter: 	CExtString & _TheNews
	Parameter: 	eNewsType _Type
	Parameter: 	CCalendar & _Thedate
	Returns:   		
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem(CExtString& _TheNews, eNewsType _Type, CCalendar& _Thedate)
	: m_TheDate(_Thedate)
    , m_Type(_Type)
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
CNewsItem::CNewsItem(int TheNews, eNewsType type, CCalendar& Thedate)
	: m_TheDate(Thedate)
    , m_Type(type)
{
    m_Message.LoadString(TheNews);
}


/*------------------------------------------------------------------------------
	Method:   	CNewsItem::CNewsItem
	Access:    	public 
	Parameter: 	CExtString & TheNews
	Parameter: 	CCalendar & Thedate
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CNewsItem::CNewsItem(CExtString& TheNews, CCalendar& Thedate)
	: m_TheDate(Thedate)
	, m_Type(NEWSALL)
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
    _pDataFile->Read(&m_Type, sizeof m_Type);
    _pDataFile->Read(&m_TheDate, sizeof CCalendar);
//	DEBUG_OUTPUT(("Type : %d, Date %s, %s"), (m_Type, m_TheDate.DayMonthStr(), m_Message));
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
    _pDataFile->Write(&m_Type, sizeof m_Type);
    _pDataFile->Write(&m_TheDate, sizeof CCalendar);
//	DEBUG_OUTPUT(("Type : %d, Date %s, %s"), (m_Type, m_TheDate.DayMonthStr(), m_Message));
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
    static int NewsHeaders[] =
	{
		IDS_NEWSTRANSFER,
		IDS_NEWSFEDERATIONHEADQUARTERS,
		IDS_NEWSCOUNTRYHEADQUARTERS,
		IDS_NEWSDISCIPLINE,
		IDS_NEWSINJURY,
		IDS_NEWSMANAGMENT,
		IDS_NEWSCLUB,
		IDS_NEWSPLAYER,
	};
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
	Method:   	CNewsItem::GetMessage
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CNewsItem::GetMessage()
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
	DeleteAll();
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
    _pDataFile->Read(&HowMany, sizeof HowMany);
	DEBUG_OUTPUT(("New Item Count %d"), (HowMany));
	// Ensure the list is the correct size
	if (HowMany > GetSize())
	{
		// Add items
		SetSize(HowMany);
		for (LoopCount = 0; LoopCount < HowMany; LoopCount++)
		{
            if (GetAt(LoopCount) == null)
			{
				SetAtGrow(LoopCount, new CNewsItem());
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
    _pDataFile->Write(&LoopCount, sizeof LoopCount);
	DEBUG_OUTPUT(("New Item Count %d"), (LoopCount));
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
