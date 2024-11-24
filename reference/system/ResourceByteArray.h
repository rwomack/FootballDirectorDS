
/*

	File:- ResourceByteArray.h
  
	Interface for the CResourceByteArray class.
	
	(c) RW Software 2004
	  
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
*/


#if !defined(AFX_RESOURCEBYTEARRAY_H__44E5CDDC_045A_4305_8CCB_D91C2F74FB6B__INCLUDED_)
#define AFX_RESOURCEBYTEARRAY_H__44E5CDDC_045A_4305_8CCB_D91C2F74FB6B__INCLUDED_


class CResourceByteArray  
{
public:
	CResourceByteArray(UInt32 dwResourceID, LPCTSTR pcszClass);
	virtual ~CResourceByteArray();

	const BYTE				GetAt(const Int16 aIdx);
	const PBYTE				GetData(void);
	const UInt16			GetUpperBound(void);

protected:
	PBYTE					m_pData;
	UInt32					m_dwSize;
};

#endif // !defined(AFX_RESOURCEBYTEARRAY_H__44E5CDDC_045A_4305_8CCB_D91C2F74FB6B__INCLUDED_)
