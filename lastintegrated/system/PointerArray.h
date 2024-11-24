
template <typename T> class CPointerArray
{
public:

// Construction
	CPointerArray()
	{
		m_pData = null;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}

	~CPointerArray()
	{
		delete[] m_pData;
	}

 // Attributes

	/*------------------------------------------------------------------------------
		Method:   	CPointerArray::GetSize
		Access:    	public 
		Returns:   	int
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	int GetSize() const
	{
		return m_nSize;
	}

	
	/*------------------------------------------------------------------------------
		Method:   	CPointerArray<T>::GetUpperBound
		Access:    	public 
		Returns:   	int
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	int GetUpperBound() const
	{
		if (m_nSize > 0)
		{
			return m_nSize - 1;
		}
		else
		{
			return 0;
		}
	}
 
	
	/*------------------------------------------------------------------------------
		Method:   	CPointerArray<T>::SetSize
		Access:    	public 
		Parameter: 	int nNewSize
		Parameter: 	int nGrowBy
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void SetSize(int nNewSize, int nGrowBy = -1)
	{
		EXT_ASSERT(nNewSize >= 0); //, "Cant resize to zero ");

		if (nGrowBy != -1)
		{
			m_nGrowBy = nGrowBy;  // set new size
		}

		if (nNewSize == 0)
		{
			// shrink to nothing
			delete[] m_pData;
			m_pData = null;
			m_nSize = m_nMaxSize = 0;
		}
		else if (m_pData == null)
		{
			// create one with exact size
			m_pData = new T *[nNewSize * sizeof(T *)];
#ifdef WIN32
			memset(m_pData, 0, nNewSize * sizeof(T *));  // zero fill
#else
			MemSet(m_pData, 0, nNewSize * sizeof(T *));  // zero fill
#endif
			m_nSize = m_nMaxSize = nNewSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{
			// it fits
			if (nNewSize > m_nSize)
			{
				// initialize the new elements
#ifdef WIN32
				memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#else
				MemSet(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#endif
			}
			m_nSize = nNewSize;
		}
		else
		{
			// otherwise, grow array
			int nGrowBy = m_nGrowBy;
			if (nGrowBy == 0)
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = m_nSize / 8;
				if (nGrowBy < 4)
				{
					nGrowBy = 4;
				}
				else if (nGrowBy > 1024)
				{
					nGrowBy = 1024;
				}
			}
			int nNewMax;
			if (nNewSize < m_nMaxSize + nGrowBy)
			{
				nNewMax = m_nMaxSize + nGrowBy;  // granularity
			}
			else
			{
				nNewMax = nNewSize;  // no slush
			}
			T ** pNewData = new T *[nNewMax * sizeof(T *)];

			// copy new data from old
#ifdef WIN32
			(memcpy)(pNewData, m_pData, m_nSize * sizeof(T *));
			// construct remaining elements
			memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#else
			MemMove(pNewData, m_pData, m_nSize * sizeof(T *));
			// construct remaining elements
			MemSet(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#endif
			// get rid of old stuff (note: no destructors called)
			delete[] m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
			m_nMaxSize = nNewMax;
		}
	}


// Operations

/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::FreeExtra
	Access:    	public 
	Returns:   	void
	Qualifier: 	Clean up
	Purpose:
------------------------------------------------------------------------------*/
void FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
		T** pNewData = null;
		if (m_nSize != 0)
		{
			pNewData = new T *[m_nSize * sizeof(T *)];
			// copy new data from old
#ifdef WIN32
			memcpy(pNewData, m_pData, m_nSize * sizeof(T *));
#else
			MemMove(pNewData, m_pData, m_nSize * sizeof(T *));
#endif
		}

		// get rid of old stuff (note: no destructors called)
		delete[] m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray::RemoveAll
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void RemoveAll()
{
	SetSize(0);
}

/*------------------------------------------------------------------------------
	Method:   	CPointerArray::RemoveList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void RemoveList()
{
	DeleteAll();
	RemoveAll();
 }


 //---------------------------------------------------------------------------
void Swap(const int n1, const int n2)
{
    T *pTemp = GetAt(n1);
	EXT_ASSERT(pTemp != null);
	SetAt(n1, GetAt(n2));
	SetAt(n2, pTemp);
}


 // Accessing elements
 //---------------------------------------------------------------------------
T * GetAt(const int nIndex) const
{
	EXT_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}


 //---------------------------------------------------------------------------
T * operator[](const int nIndex) const
{
	return GetAt(nIndex);
}


 //---------------------------------------------------------------------------
void SetAt(int nIndex, T * newElement)
{
	EXT_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = newElement;
}


 // Potentially growing the array
 //---------------------------------------------------------------------------
void SetAtGrow(int nIndex, T * newElement)
{
	if (nIndex >= m_nSize)
	{
		SetSize(nIndex + 1);
	}
	m_pData[nIndex] = newElement;
}


//---------------------------------------------------------------------------
int Add(T * newElement)
{
	int nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray::AddNew
	Access:    	public 
	Parameter: 	void * _newElement
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int AddNew(T * _newElement)
{
	int nRetID = NOTFOUND;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos) == null)
		{
			SetAt(ListPos, _newElement);
			nRetID = ListPos;
		}
	}
	if (nRetID == NOTFOUND)
	{
		return Add(_newElement);
	}
	return nRetID;
}


 // Operations that move elements around
 // -------------------------------------------------------------------------------------------------------------------------
void RemoveAt(const int nIndex, const int nCount = 1)
{
	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	if (nMoveCount)
	{
#ifdef WIN32
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount], nMoveCount * sizeof(T *));
#else
		for(int LoopCount = nIndex; LoopCount < nIndex + nMoveCount; LoopCount++)
		{
			m_pData[nIndex] = m_pData[nIndex + LoopCount];
		}
#endif

	}
	m_nSize -= nCount;
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray::DeleteAll
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
private:
void DeleteAll()
{
	T* pElement;
	int max = GetSize();
    for (int ListPos = 0; ListPos < max; ListPos ++)
	{
	    pElement = GetAt(ListPos);
		if (pElement != null)
		{
			delete pElement;
			SetAt(ListPos, null);
		}
	}
}

public:
/*------------------------------------------------------------------------------
	Method:   	CPointerArray::Remove
	Access:    	public 
	Parameter: 	void * pElement
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void Remove(T * pElement)
{
	RemoveAt(Find(pElement));
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray::Find
	Access:    	public 
	Parameter: 	void * _Element
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int Find(T* _Element)
{
	EXT_ASSERT(_Element != null); //, "Cant find null!");
	ushort iRet = wNOTFOUND;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
	    if (_Element == GetAt(ListPos))
		{
			iRet = ListPos;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray::DeleteOne
	Access:    	public 
	Parameter: 	const int _Index
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void DeleteOne(const int _Index)
{
	EXT_ASSERT(_Index < GetSize()); //, "Index out of range");
    delete GetAt(_Index);
	RemoveAt(_Index);
}

// Implementation
protected:
	T **					m_pData;		// the actual array of data
	int						m_nSize;		// # of elements(upperBound - 1)
	int						m_nMaxSize;		// max allocated
	int						m_nGrowBy;		// grow amount
};
