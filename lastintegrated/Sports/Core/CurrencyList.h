
 /*

    Class:- CurrencyList


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


 #ifndef __CURRENCYLIST_H__
 #define __CURRENCYLIST_H__


class CCurrencyList : public CExtArray
{
public:
    CCurrencyList()		// Constructor
	{
		CBinaryReader theResource(0, IDR_CURRENCY, TEXT("FDDATA"));
		CExtArray::ReadSizeFromResource(theResource);
		EXT_ASSERT(CExtArray::GetSize() > 0);

		// Create the currencies
		m_pList = new CCurrency[CExtArray::GetSize()];
		for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
		{
			m_pList[LoopCount].DoInitialiseFromResource(theResource);
		}
	}

    ~CCurrencyList()				// Destructor
	{
		delete[] m_pList;
	}

    // Methods
    CCurrency&				GetAt(const ushort x)
	{
		EXT_ASSERT(x < CExtArray::GetSize());
		return static_cast<CCurrency&>(m_pList[x]);
	}

	// Variables
private:
	CCurrency*				m_pList;
};

#endif // __CURRENCYLIST_H__

