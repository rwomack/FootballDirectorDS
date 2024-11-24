
 /*

    News.h: interface for the CNewsItem class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#ifndef __NEWS_H__
#define __NEWS_H__


class CNewsItem
{
public:

    // Constructor
    CNewsItem();
    CNewsItem(const CExtString& TheNews, eNewsType type, CCalendar& Thedate, bool m_bUserRelated = false );
    CNewsItem(int TheNews, eNewsType type, CCalendar& Thedate, bool m_bUserRelated = false);
    CNewsItem(const CExtString& TheNews, CCalendar& Thedate, bool m_bUserRelated = false );

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoBuildDisplayList(CStringArray& _TheList, const bool _bAddType, CString& _TheDate, const bool _bReverse);
	void					DoBuildUserRelatedList(lArray<bool>& _TheList, CString& _TheDate);
	CString					GetDateStr();
	const CString&			GetMessage() const;
	const CCalendar &		GetDate() const;
	eNewsType				GetType() const;
	bool                IsUserRelated();

	typedef CString (*NewsPreprocessPlayerNameLinkFunc)(CPlayer *);
	typedef CString (*NewsPreprocessClubNameLinkFunc)(CClub *);

	static CString PreprocessPlayerName(CPlayer *pPlayer);
	static CString PreprocessClubName(CClub *pClub);

	static void SetPlayerNamePreprocessFunc(NewsPreprocessPlayerNameLinkFunc cPreprocessor);
	static void SetClubNamePreprocessFunc(NewsPreprocessClubNameLinkFunc cPreprocessor);

private:
	// Variables
	static NewsPreprocessPlayerNameLinkFunc ms_NewsPreprocessPlayerNameLink;
	static NewsPreprocessClubNameLinkFunc ms_NewsPreprocessClubNameLink;

    CExtString              m_Message;
    CCalendar				m_TheDate;
    eNewsType				m_Type;
    bool                m_bUserRelated;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNewsList : public CPointerArray<CNewsItem>
{
public:

	// Constructor
    CNewsList() {};
    ~CNewsList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoBuildDisplayList(CStringArray& _TheList, const bool _bAddType, CString& _TheDate, const bool _bReverse);
	void					DoBuildUserRelatedList(lArray<bool>& _TheList, CString& _TheDate);
	void					DoShrinkList(const ushort MaxEntries);
	int						GetCount();
};
#endif
