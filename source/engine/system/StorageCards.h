// StorageCards.h: interface for the CStorageCards class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STORAGECARDS_H__3A366113_5C0C_4773_AC93_8A8B9CBAE839__INCLUDED_)
#define AFX_STORAGECARDS_H__3A366113_5C0C_4773_AC93_8A8B9CBAE839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStorageCards  
{
public:
	CString DoFindPath(const UInt16 aIdx);
	void DoBuildList(CDropList& arListBox);
	CStorageCards();
	virtual ~CStorageCards();

};

#endif // !defined(AFX_STORAGECARDS_H__3A366113_5C0C_4773_AC93_8A8B9CBAE839__INCLUDED_)
