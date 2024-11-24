

/*

	LeadingDiscipinaryPoints.h 

  	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */



class CLeadingDiscipinaryPoints : public CPlayerClub
{
public:
    CLeadingDiscipinaryPoints() {};
	CLeadingDiscipinaryPoints(const ushort _Num, const ushort _CNum, const byte _YellowCards, const byte _RedCards);

	ushort					GetYellow() const;
	ushort					GetRed() const;
	ushort					GetTotalScored() const;

// Variables
private:
	byte					m_YellowCards;
	byte					m_RedCards;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CLeadingDisciplinaryList : public CPointerArray<CLeadingDiscipinaryPoints>
{
public:
	// Constructor
    CLeadingDisciplinaryList() {};

	bool					CheckForList(CDivision& _Division);
	void					CreateList(CDivision& _Division, const ushort _ListSize);
	void Sort(const int _Left, const int _Right);
};
