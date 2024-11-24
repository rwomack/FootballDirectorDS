
#ifndef _VECTOR_H
#define _VECTOR_H


template <class Ttype> class Vector
{
private:
	Ttype*					m_Elements;

	ushort 					m_nElementsMax;
	ushort					m_nElementsIncrement;
	ushort					m_nElementsCurrent;
	
public:

	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::Vector
		Access:    	public 
		Returns:   	
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Vector()
	{
		m_nElementsMax = 10;
		m_nElementsIncrement = 10;
		m_Elements = new Ttype[m_nElementsMax];
		m_nElementsCurrent = 0;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::Vector
		Access:    	public 
		Parameter: 	int _ElementsInitial
		Parameter: 	int _ElementIncrement
		Returns:   	
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Vector(int _ElementsInitial, int _ElementIncrement)
	{
		m_nElementsMax = _ElementsInitial;
		m_nElementsIncrement = _ElementIncrement;
		m_Elements = new Ttype[_ElementsInitial];
		m_nElementsCurrent = 0;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::~Vector
		Access:    	public 
		Returns:   	
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	~Vector()
	{
		delete []m_Elements;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::operator[]
		Access:    	public 
		Parameter: 	int _Index
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype& operator[](int _Index)
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		return GetAt(_Index);
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::operator[]
		Access:    	public 
		Parameter: 	int _Index
		Returns:   	const Ttype&
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	const Ttype& operator[](int _Index) const
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		return GetAt(_Index);
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::Append
		Access:    	public 
		Parameter: 	Vector<Ttype> & _Array2
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void Append(const Vector<Ttype>& _Array2)
	{
		for (int LoopCount = 0; LoopCount < _Array2.GetSize(); LoopCount++)
		{
			Add(_Array2[LoopCount]);
		}
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::capacity
		Access:    	public 
		Returns:   	ushort
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	ushort capacity() const
	{
		return m_nElementsMax;
	}

	
	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetSize
		Access:    	public 
		Returns:   	ushort
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	ushort GetSize() const
	{
		return m_nElementsCurrent;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetUpperBound
		Access:    	public 
		Returns:   	ushort
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	ushort GetUpperBound() const
	{
		return m_nElementsCurrent - 1;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetAtUpperBound
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype& GetAtUpperBound()
	{
		return m_Elements[m_nElementsCurrent - 1];
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetAtUpperBound
		Access:    	public 
		Returns:   	const Ttype&
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	const Ttype& GetAtUpperBound() const
	{
		return m_Elements[m_nElementsCurrent - 1];
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RandomPosition
		Access:    	public 
		Returns:   	ushort
		Qualifier: 	
		Purpose:	Get a random index
	------------------------------------------------------------------------------*/
	ushort RandomPosition() const
	{
		if (m_nElementsCurrent > 1)
		{
			return RandomNumber.IntLessThan(GetUpperBound());
		}
		else
		{
			return 0;
		}
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RandomValue
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:	Get the contents of a random entry
	------------------------------------------------------------------------------*/
	Ttype RandomValue()
	{
		if (m_nElementsCurrent > 0)
		{
			return m_Elements[RandomPosition()];
		}
		return 0;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RandomValue
		Access:    	public 
		Returns:   	const Ttype
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	const Ttype RandomValue() const
	{
		if (m_nElementsCurrent > 0)
		{
			return m_Elements[RandomPosition()];
		}
		return 0;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::SetAtGrow
		Access:    	public 
		Parameter: 	const ushort _Position
		Parameter: 	const Ttype & _Element
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void SetAtGrow(const ushort _Position, const Ttype& _Element)
	{
		while (_Position >= GetSize())
		{
			Add(_Element);
		}
		m_Elements[_Position] = _Element;
	}
	

	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::SetAt
		Access:    	public 
		Parameter: 	ushort _Index
		Parameter: 	Ttype _Element
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void SetAt(const ushort _Index, const Ttype &_Element)
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		m_Elements[_Index] = _Element;
	}

	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::SetSize
		Access:    	public 
		Parameter: 	const ushort _Length
		Parameter: 	const Ttype & _Value
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void SetSize(const ushort _Length, const Ttype &_Value = (Ttype)null)
	{
		if( _Length < m_nElementsCurrent )
		{
		  RemoveFromEnd( m_nElementsCurrent - _Length );
		} 
		else
		{
			for (int ListPos = m_nElementsCurrent; ListPos < _Length; ListPos++)
			{
				Add(_Value);
			}
		}
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::Add
		Access:    	public
		Parameter: 	Ttype _Element
		Returns:   	void
		Qualifier:
		Purpose:
	------------------------------------------------------------------------------*/
	void Add(const Ttype& _Element)
	{
		if (m_nElementsCurrent == m_nElementsMax)
		{
			int	nElementsMaxNew = m_nElementsMax + m_nElementsIncrement;
			Ttype* pElements = new Ttype[nElementsMaxNew];

			int LoopCount;
			for (LoopCount = 0; LoopCount < m_nElementsMax; LoopCount++)
			{
				pElements[LoopCount] = m_Elements[LoopCount];
			}

			for (LoopCount = 0; LoopCount < m_nElementsIncrement; LoopCount++ )
			{
				pElements[m_nElementsMax + LoopCount] = (Ttype)null;
			}

			delete []m_Elements;

			m_Elements = pElements;
			m_nElementsMax = nElementsMaxNew;
		}
		m_Elements[m_nElementsCurrent++] = _Element;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RemoveAt
		Access:    	public 
		Parameter: 	Ttype _Element
		Returns:   	bool
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	bool RemoveElement(const Ttype& _Element)
	{
		int	index = indexOf(_Element);
		if (index != -1)
		{
			RemoveAt(index);
			return true;
		}
		return false;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RemoveAt
		Access:    	public 
		Parameter: 	short _ID
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void RemoveAt(short _ID)
	{
		for (int iToEnd = _ID; iToEnd < m_nElementsCurrent - 1; iToEnd++)
		{
			m_Elements[iToEnd] = m_Elements[iToEnd + 1];
		}
		m_nElementsCurrent--;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RemoveFromEnd
		Access:    	public 
		Parameter: 	const ushort _HowMany
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void RemoveFromEnd(const ushort _HowMany)
	{
		for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
		{
			RemoveAt(m_nElementsCurrent - 1);
		}
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::RemoveAll
		Access:    	public 
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void RemoveAll()
	{
		m_nElementsCurrent = 0;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::DoSwap
		Access:    	public 
		Parameter: 	const ushort _Index
		Parameter: 	const ushort _Index1
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void DoSwap(const ushort _Index, const ushort _Index1)
	{
		EXT_ASSERT(_Index < m_nElementsCurrent); //, "Index out of range");
		EXT_ASSERT(_Index1 < m_nElementsCurrent); //, "Index1 out of range");
		Ttype temp = m_Elements[_Index];
		m_Elements[_Index] = m_Elements[_Index1];
		m_Elements[_Index1] = temp;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetAt
		Access:    	public 
		Parameter: 	const ushort _Index
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype &GetAt(const ushort _Index)
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		return m_Elements[_Index];
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::GetAt
		Access:    	public 
		Parameter: 	const ushort _Index
		Returns:   	const Ttype&
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	const Ttype& GetAt(const ushort _Index) const
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		return m_Elements[_Index];
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::contains
		Access:    	public 
		Parameter: 	const Ttype & _Element
		Returns:   	bool
		Qualifier: 	const
		Purpose:
	------------------------------------------------------------------------------*/
	bool contains(const Ttype &_Element) const
	{
		int index = indexOf( _Element );
		return (index == -1) ? false : true;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::indexOf
		Access:    	public 
		Parameter: 	Ttype _Element
		Parameter: 	const ushort _Index
		Returns:   	int
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	int indexOf(const Ttype &_Element, const ushort _Index = 0 ) const
	{
		EXT_ASSERT(_Index < m_nElementsCurrent);
		for (int i = _Index; i < m_nElementsCurrent; i++)
		{
			if (m_Elements[i] == _Element)
			{
				return i;
			}
		}

		return -1;
	}


	/*------------------------------------------------------------------------------
		Method:   	Vector<Ttype>::DoShuffleArray
		Access:    	public 
		Parameter: 	const ushort anStartIndex
		Parameter: 	ushort MaxPos
		Returns:   	void
		Qualifier: 	
		Purpose:	Randomly arrange the contents of the array between and 
					including anStartIndex and maxpos
	------------------------------------------------------------------------------*/
	void DoShuffleArray(const ushort anStartIndex = 0, ushort MaxPos = 0)
	{
		EXT_ASSERT(anStartIndex <= MaxPos);
		EXT_ASSERT(MaxPos < m_nElementsCurrent);

		if (MaxPos == 0)
		{
			MaxPos = m_nElementsCurrent - 1;
		}
		if ((MaxPos - anStartIndex) > 1)
		{
			Ttype temp1;
			for (int LoopCount = anStartIndex; LoopCount <= MaxPos; LoopCount++)
			{
				temp1 = anStartIndex + RandomNumber.IntLessThan(MaxPos - anStartIndex);	// Choose a random position
				DoSwap(temp1, LoopCount);					// Swap the value at the current position with the value at the random position
			}
		}
	}
};
#endif //_VECTOR_H
