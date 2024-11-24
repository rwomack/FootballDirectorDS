

 /*

    Class:- PlayerCareerPath


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */



#if !defined(AFX_PlayerCareerPATH_H__E6737082_2232_11D2_BD3F_0000B4908891__INCLUDED_)
#define AFX_PlayerCareerPATH_H__E6737082_2232_11D2_BD3F_0000B4908891__INCLUDED_


	static const byte		MINIMUPLAYERAGE = 16;
	static const byte		MAXIMUMPLAYERAGE = 46;		// Ensure age is reasonable
	static const byte		PLAYERAGERANGE = 41/*MAXIMUMPLAYERAGE*/ - MINIMUPLAYERAGE;					
	static const int		BOSMANAGE = 24;

	
class CPlayerCareerPath  
{
public:

    // Methods
	byte					GetFrequency() const;
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	double 					PositiveMultiplier(const byte _Age) const;
	double 					NegativeMultiplier(const byte _Age) const;
	ushort 					DoCalculateWhichElementForAge(const byte _Age) const;

    // Variables
private:
	byte					m_Frequency;
	byte					m_Positive[PLAYERAGERANGE];
	byte					m_Negative[PLAYERAGERANGE];
};
//////////////////////////////////////////////////////////////////////////////////////


class CPlayerCareerPathList : public CExtArray 
{
public:
    CPlayerCareerPathList();			// Constructor
    ~CPlayerCareerPathList();			// Destructor

    // Methods
	byte					DoFindPath(byte _FirstPathID = 0);
	CPlayerCareerPath*		GetAt(const ushort _ElementNumber);
	byte					GetFirstOutfieldID() const;					
	byte					RandomCareerPath(ePlayerPosition _Position);
	byte					RandomGoalkeeperCareerPath();
	byte					RandomOutfieldCareerPath();

    // Variables
private:
	byte					m_FirstOutfieldId;
	CPlayerCareerPath*		m_pList;
};
#endif // !defined(AFX_PlayerCareerPATH_H__E6737082_2232_11D2_BD3F_0000B4908891__INCLUDED_)
