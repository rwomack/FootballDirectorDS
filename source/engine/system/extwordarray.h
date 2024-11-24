
/*

    extwordarray.h - interface of CExtWordArray class

	(c) RW Software 1994 - 2001

 ---------------------------------------------------------------------------------------
*/


#ifndef __EXTWORDARRAY_H__
#define __EXTWORDARRAY_H__


class CExtWordArray : public CWordArray
{
  public:
    CExtWordArray&			operator = (const CExtWordArray &p);
	CExtWordArray&			Copy(const CExtWordArray *p = NULL);

    void					AddAndRemoveFromArray2(CExtWordArray* Array2Ptr, int HowMany);
	void			AddFromArray2(CExtWordArray* Array2Ptr, int HowMany);
	void			AddInstances(int HowMany, WORD Value);
	void			AddToArray2IfBetweenIncluding(WORD a, WORD b, CExtWordArray* Array2Ptr);
	WORD			AverageNonZeroValue(void);
    WORD            AverageValue(void)
	{
		return TotalSum() / GetSize();
	}
	WORD			AverageValue(int StartPos, int EndPos)
	{
		return TotalSum(StartPos, EndPos) /(EndPos - StartPos);
	}
	bool			CheckForDuplicates(void);
	int				CountInstances(WORD Value);
	int				CountInstancesBetweenIncluding(WORD First, WORD Last);
	void			Decrement(int x)
	{
		SetAt(x, GetAt(x) - 1);
	}
	void			DecrementIfNotZero(void);
	void			FillWithValue(WORD x);
	void			FillToEndWithValue(WORD x, int StartPosn);
    void            FillWithValue(WORD x, int StartPosn, int HowMany);
    int             Find( WORD dwVal, int StartPos = 0 );
    void            Get(CDataFile* File, int HowMany);
    WORD            HighestValue(void);
	WORD			LowestValue(void);
    void            MoveDown(void);
    int             NearestAbove(WORD dwVal);
	int				NumberNonZero(void);
	int				NumberNonZeroLessThan(WORD x);
    void            LowerValue(WORD x, int StartPosn = 0);
    void            RaiseValue(WORD x, int StartPosn = 0);
    void            RemoveIfFoundInArray2(CExtWordArray* Array2Ptr);
    void            Read(CDataFile* File);
	void			RemoveDuplicates(void);
	void			Replace(WORD ToReplace, WORD ReplaceWith);
	void			SetSizeFillWithValue(int HowMany, WORD value);
	void			ShuffleArray(void);
	void			ShuffleArray(int StartPos, int MaxPos);
	void			Swap(int a, int b);
	int				TotalSum(void);
	int				TotalSum(int StartPos, int EndPos);
    void      Write(CDataFile* File);
};

#endif

