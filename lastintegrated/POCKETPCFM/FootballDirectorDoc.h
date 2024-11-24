// Football DirectorDoc.h : interface of the CFootballDirectorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POCKETPCFMDOC_H__86728729_BF97_4E81_A237_1736913542E5__INCLUDED_)
#define AFX_POCKETPCFMDOC_H__86728729_BF97_4E81_A237_1736913542E5__INCLUDED_


class CFootballDirectorDoc : public CDocument
{
protected: // create from serialization only
	CFootballDirectorDoc();
	DECLARE_DYNCREATE(CFootballDirectorDoc)

// Attributes
public:
// 	CWorld*					m_WorldData;

// Operations
public:

// Overrides
	public:
	virtual BOOL			OnNewDocument();

// Implementation
public:
	virtual ~CFootballDirectorDoc();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POCKETPCFMDOC_H__86728729_BF97_4E81_A237_1736913542E5__INCLUDED_)
