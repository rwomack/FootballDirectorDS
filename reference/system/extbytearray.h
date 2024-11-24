
/*

    extBytearray.h - interface of CExtByteArray class

	(c) RW Software 1994 - 2002

  ---------------------------------------------------------------------------------------
*/


#ifndef __EXTBYTEARRAY_H__
#define __EXTBYTEARRAY_H__


class CExtByteArray : public CByteArray
{
  public:
    CExtByteArray&	operator = (const CExtByteArray &p);
	void			AddAndRemoveFromArray2 (CExtByteArray* Array2Ptr, int HowMany);
	void			AddFromArray2 (CExtByteArray* Array2Ptr, int HowMany);
	void			AddInstances(int HowMany, BYTE Value);
	void			AddToArray2IfBetweenIncluding(BYTE a, BYTE b, CExtByteArray* Array2Ptr);
	BYTE			AverageNonZeroValue(BYTE HowMany = 0);
	void			RemoveIfFoundInArray2 (CExtByteArray* Array2Ptr);
	BYTE			AverageValue(void)							{ return TotalSum() / GetSize(); }
    BYTE            AverageValue(int StartPos, int EndPos)     { return TotalSum(StartPos, EndPos) / (EndPos - StartPos); }
	bool			CheckForDuplicates(void);
	int				CountInstances(BYTE Value);
	int				CountInstancesBetweenIncluding(BYTE First, BYTE Last);
	inline void		Decrement(int x)							{ SetAt(x, GetAt(x) - 1); }
	void			DecrementIfNotZero(void);
	void			FillWithValue(BYTE x, int StartPosn = 0);
    void            FillWithValue(BYTE x, int StartPosn, int HowMany);
	void			FillToEndWithValue(BYTE x, int StartPosn);
    void            FillWithRandomValue(BYTE MaxValue);
	void			FillWithRandomValue(BYTE AverageValue, BYTE MaxValue, BYTE Deviation, BYTE StartPosn);
    int				Find(BYTE dwVal, int StartPos = 0);
    void			Get(FILE* File, int HowMany);
	BYTE			HighestValue(void);
	inline void		Increment(int x)							{ SetAt(x, GetAt(x) + 1); }
	BYTE			LowestValue(void);
    void            MoveDown(void);
    int             NearestAbove(BYTE dwVal);
	int				NumberNonZero(void);
	int				NumberNonZeroLessThan(BYTE x);
	int				NumberZero(void);
	int				PositionLessThanValue(int nm);
    void            LowerValue(BYTE x, int StartPosn = 0);
    void            RaiseValue(BYTE x, int StartPosn = 0);
    BYTE            RandomValue(void);
	int				RandomPosition(void);
	int 			RandomPositionOfValue(BYTE value);
    void			Read(FILE* File);
	void			RemoveDuplicates(void);
	void			Replace(BYTE ToReplace, BYTE ReplaceWith);
	void			SetSizeFillWithValue(int HowMany, BYTE value);
	void			ShuffleArray(void);
	void			SortLow(void);
	void			SortHigh(void);
	void			ShuffleArray(int StartPos, int MaxPos );
	void			Swap(int a, int b);
	int				TotalSum(void);
	int				TotalSum(int StartPos, int EndPos);
    void			Write(FILE* File);

};

#endif
