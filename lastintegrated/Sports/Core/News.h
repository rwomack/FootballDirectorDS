
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
    CNewsItem(CExtString& TheNews, eNewsType type, CCalendar& Thedate);
    CNewsItem(int TheNews, eNewsType type, CCalendar& Thedate);
    CNewsItem(CExtString& TheNews, CCalendar& Thedate);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoBuildDisplayList(CStringArray& _TheList, const bool _bAddType, CString& _TheDate, const bool _bReverse);
	CString					GetDateStr();
	CString&				GetMessage();
	eNewsType				GetType() const;

	// Variables
private:
    CExtString              m_Message;
    CCalendar				m_TheDate;
    eNewsType				m_Type;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNewsList : public CPointerArray
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
	void					DoShrinkList(const ushort MaxEntries);
	int						GetCount();
};
#endif
