#ifndef __ALPHATAB_H__
#define __ALPHATAB_H__


class CAlphaTab : public CTabCtrl
{
public:
	CAlphaTab() : 
	  CTabCtrl() 
	{
	}
	  
	~CAlphaTab() {}

	void					DoInitialise(void);
	UInt16					GetWhichTab(CString& szStr, CTabbedAlphaListBox& pListBox);

protected:
};
#endif
