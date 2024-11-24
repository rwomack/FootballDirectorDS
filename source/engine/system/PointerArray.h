#pragma once

 /*

	File:- PointerArray.h

	(c) Sports Director Ltd 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

const ushort          c_nGrowBy = 10;

template <typename T> class CPointerArray
{
public:

/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::CPointerArray
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPointerArray()
{
	m_pData = null;
	m_nSize = m_nMaxSize = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::~CPointerArray
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
virtual ~CPointerArray()
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
void SetSize(int nNewSize)
{
	EXT_ASSERT(nNewSize >= 0); //, "Cant resize to zero ");

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
		m_pData = FD_ENGINE_NEW T *[nNewSize * sizeof(T *)];
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
		int nNewMax;
		if (nNewSize < m_nMaxSize + c_nGrowBy)
		{
			nNewMax = m_nMaxSize + c_nGrowBy;  // granularity
		}
		else
		{
			nNewMax = nNewSize;  // no slush
		}

		m_pData = (T **)FD_ENGINE_REALLOC(nNewMax * sizeof(T *), m_pData);

#ifdef WIN32
		// construct remaining elements
		memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#else
		// construct remaining elements
		MemSet(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(T *));
#endif

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
		T ** pNewData = null;
		if (m_nSize != 0)
		{
			m_pData = (T **)FD_ENGINE_REALLOC(m_nSize * sizeof(T *), m_pData);
		}
		else
		{
			delete[] m_pData;
		}

		// get rid of old stuff (note: no destructors called)
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


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::Swap
	Access:    	public
	Parameter: 	const int _Index1
	Parameter: 	const int _Index2
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void Swap(const int _Index1, const int _Index2)
{
    T *pTemp = GetAt(_Index1);
	FDASSERT(pTemp != null);
	SetAt(_Index1, GetAt(_Index2));
	SetAt(_Index2, pTemp);
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::GetAt
	Access:    	public
	Parameter: 	const int _nIndex
	Returns:   	T*
	Qualifier: 	const
	Purpose:	Accessing elements
------------------------------------------------------------------------------*/
inline T* GetAt(const int _nIndex) const
{
	FDASSERT(_nIndex >= 0 && _nIndex < m_nSize);
	return m_pData[_nIndex];
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::operator[]
	Access:    	public
	Parameter: 	const int _nIndex
	Returns:   	T*
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
T* operator[](const int _nIndex) const
{
	return GetAt(_nIndex);
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::SetAt
	Access:    	public
	Parameter: 	const int _nIndex
	Parameter: 	T * _newElement
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void SetAt(const int _nIndex, T* _newElement)
{
	FDASSERT(_nIndex >= 0 && _nIndex < m_nSize);
	m_pData[_nIndex] = _newElement;
}


 /*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::SetAtGrow
	Access:    	public
	Parameter: 	const int _nIndex
	Parameter: 	T* _newElement
	Returns:   	void
	Qualifier:
	Purpose:	Potentially growing the array
------------------------------------------------------------------------------*/
void SetAtGrow(const int _nIndex, T* _newElement)
{
	if (_nIndex >= m_nSize)
	{
		SetSize(_nIndex + 1);
	}
	m_pData[_nIndex] = _newElement;
}


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::Add
	Access:    	public
	Parameter: 	T * _newElement
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int Add(T* _newElement)
{
	int nIndex = m_nSize;
	SetAtGrow(nIndex, _newElement);
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


/*------------------------------------------------------------------------------
	Method:   	CPointerArray<T>::RemoveAt
	Access:    	public
	Parameter: 	const int nIndex
	Parameter: 	const int nCount
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void RemoveAt(const int nIndex, const int nCount = 1)
{
	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
  if (nMoveCount > 0)
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
int Find(const T* _Element) const
{
  FDASSERT(_Element != null); //, "Cant find null!");
	ushort iRet = wNOTFOUND;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
	    if (_Element == m_pData[ListPos])
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
  FDASSERT(_Index < GetSize()); //, "Index out of range");
    delete GetAt(_Index);
	RemoveAt(_Index);
}

// Implementation
protected:
	T **					m_pData;		// the actual array of data
	int						m_nSize;		// # of elements(upperBound - 1)
	int						m_nMaxSize;		// max allocated
};
