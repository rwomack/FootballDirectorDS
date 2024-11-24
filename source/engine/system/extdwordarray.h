
/*

    extdwordarray.h - interface of CExtDWordArray class

	(c) RW Software 1994 - 2004

---------------------------------------------------------------------------------------
*/


#ifndef __EXTDWORDARRAY_H__
#define __EXTDWORDARRAY_H__


class CExtDWordArray
{
protected:
	 Vector<UInt32>	m_iList;

public:

    CExtDWordArray() 						{};
    ~CExtDWordArray() 						{};

    CExtDWordArray&	operator = (CExtDWordArray* p);

  void          Read(CDataFile* File);
    void          Write(CDataFile* File);

	const UInt16			GetSize(void)								{ return m_iList.size(); }
	UInt16					GetUpperBound(void)							{ return m_iList.size() - 1; }
  void          SetAt(Int16 iPosition, UInt32 iValue)   { FDASSERT (iPosition < GetSize()); m_iList.setAt(iPosition, iValue); }
	//void					SetSize(Int16 iLength, UInt32 iValue = 0)	{ m_iList.resize(iLength, iValue); }
	void					Add(UInt32 iValue)							{ m_iList.addElement(iValue); }
	void					RemoveAt(UInt16 iPosition);
	void					RemoveAll(void)								{ m_iList.removeElementAll(); }
	void					Append(CExtDWordArray* ListToAdd);
	void					AddAndRemoveFromArray2 (CExtDWordArray* Array2Ptr, int HowMany);
	void					AddFromArray2 (CExtDWordArray* Array2Ptr, int HowMany);
	void					AddInstances(int HowMany, UInt32 Value);
	UInt32					AverageNonZeroValue(void);
	UInt32					AverageValue(void);
	UInt32					AverageValue(int StartPos, int EndPos);
	void			AddToArray2IfBetweenIncluding(UInt32 a, UInt32 b, CExtDWordArray* Array2Ptr);
	bool			CheckForDuplicates(void);
	int				CountInstances(UInt32 Value);
	int				CountInstancesBetweenIncluding(UInt32 First, UInt32 Last);
	void			DecrementIfNotZero(void);
	void			FillWithValue(UInt32 x);
	void			FillToEndWithValue(UInt32 x, int StartPosn);
    void            FillWithValue(UInt32 x, int StartPosn, int HowMany);
    void            FillWithRandomValue(int MaxValue);
	int				Find(UInt32 dwVal,int StartPos = 0);
    void      Get(CDataFile* File, int HowMany);
	UInt32			GetAtUpperBound(void)						{ return GetAt(GetSize() -1); }
  UInt32      GetAt(Int16 iPosition)            { FDASSERT (iPosition < GetSize()); return m_iList.elementAt(iPosition); }
	UInt32			HighestValue(void);
	UInt32			LowestValue(void);
    void            MoveDown(void);
    int             NearestAbove(UInt32 dwVal);
	int				NumberNonZero(void);
	int				NumberNonZeroLessThan(UInt32 x);
    void            LowerValue(UInt32 x, int StartPosn = 0);
    void            RaiseValue(UInt32 x, int StartPosn = 0);
    int             RandomPosition(void);
	UInt32			RandomValue(void);
	void			RemoveDuplicates(void);
	void			RemoveIfFoundInArray2 (CExtDWordArray* Array2Ptr);
	void			Replace(UInt32 ToReplace, UInt32 ReplaceWith);
	void			SetSizeFillWithValue(Int16 HowMany, UInt32 value);
	void			ShuffleArray(void);
	void			ShuffleArray(int StartPos, int MaxPos);
	void			Swap(int a, int b);
	int				TotalSum(void);
	int				TotalSum(int StartPos, int EndPos);

};

#endif

