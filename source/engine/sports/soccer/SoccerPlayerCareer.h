#ifndef __PCAREER_H__
#define __PCAREER_H__


/*

    PlayerCareer Class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



class CPlayer;

//NOTE: Reducing this down will get us win x7 in CPlayer
template <int APPEARANCESBITS, int GOALSBITS, int BOOKINGSBITS, int SENDINGOFFBITS, int SUSPENSIONSBITS, int ASSISTBITS, int MANOFMATCHBITS>
class CPlayerCareer
{
	// Ugh. This needs to be public to allow other template versions of this class
	// to have access to the data members. There's probably a better way to do this, but
	// for right now this will do. Don't go abusing public data member access. :)
public:
  // based on max 2048 appearences
  //TODO: Assert this - could get booked every match and we'd overflow?
  //NOTE: Previously some of these were only 8 bits (signed)

  struct
  {
	unsigned int    m_Appearances:APPEARANCESBITS;		// standard: 11 bits
	unsigned int    m_Goals:GOALSBITS;					// standard: 12 bits
	unsigned int    m_Bookings:BOOKINGSBITS;				// standard: 8 bits
	unsigned int    m_SendingOffs:SENDINGOFFBITS;			// standard: 7 bits
	unsigned int    m_Suspensions:SUSPENSIONSBITS;		// standard: 7 bits, number of times been suspended
	#ifdef ASSISTS
	unsigned int    m_Assists:ASSISTBITS;					// standard: 12 bits
	#endif
	#ifdef STOREMANOFMATCH
	unsigned int    m_ManOfMatchAwards:MANOFMATCHBITS;	// standard: 11 bits
	#endif
  } m_bitfields;

public:
	CPlayerCareer()
	{
		m_bitfields.m_Appearances = 0;
		m_bitfields.m_Goals = 0;
		m_bitfields.m_Bookings = 0;
		m_bitfields.m_SendingOffs = 0;
		m_bitfields.m_Suspensions = 0;
#ifdef ASSISTS
		m_bitfields.m_Assists = 0;
#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards = 0;
#endif
	}

	template <int T1, int T2, int T3, int T4, int T5, int T6, int T7>
	CPlayerCareer& operator + (CPlayerCareer<T1, T2, T3, T4, T5, T6, T7> & p)
	{
		m_bitfields.m_Appearances += p.m_bitfields.m_Appearances;
		m_bitfields.m_Goals += p.m_bitfields.m_Goals;
		m_bitfields.m_Bookings += p.m_bitfields.m_Bookings;
		m_bitfields.m_SendingOffs += p.m_bitfields.m_SendingOffs;
		m_bitfields.m_Suspensions += p.m_bitfields.m_Suspensions;
#ifdef ASSISTS
		m_bitfields.m_Assists += p.m_bitfields.m_Assists;
#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards += p.m_bitfields.m_ManOfMatchAwards;
#endif
		return *this;
	}

	template <int T1, int T2, int T3, int T4, int T5, int T6, int T7>
	CPlayerCareer& operator += (CPlayerCareer<T1, T2, T3, T4, T5, T6, T7> & p)
	{
		m_bitfields.m_Appearances += p.m_bitfields.m_Appearances;
		m_bitfields.m_Goals += p.m_bitfields.m_Goals;
		m_bitfields.m_Bookings += p.m_bitfields.m_Bookings;
		m_bitfields.m_SendingOffs += p.m_bitfields.m_SendingOffs;
		m_bitfields.m_Suspensions += p.m_bitfields.m_Suspensions;
#ifdef ASSISTS
		m_bitfields.m_Assists += p.m_bitfields.m_Assists;
#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards += p.m_bitfields.m_ManOfMatchAwards;
#endif
		return *this;
	}

	template <int T1, int T2, int T3, int T4, int T5, int T6, int T7>
	CPlayerCareer& operator = (CPlayerCareer<T1, T2, T3, T4, T5, T6, T7> & p)
	{
		m_bitfields.m_Appearances = p.m_bitfields.m_Appearances;
		m_bitfields.m_Goals = p.m_bitfields.m_Goals;
		m_bitfields.m_Bookings = p.m_bitfields.m_Bookings;
		m_bitfields.m_SendingOffs = p.m_bitfields.m_SendingOffs;
		m_bitfields.m_Suspensions = p.m_bitfields.m_Suspensions;
#ifdef ASSISTS
		m_bitfields.m_Assists = p.m_bitfields.m_Assists;
#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards = p.m_bitfields.m_ManOfMatchAwards;
#endif
		return *this;
	}

  	unsigned int			GetAppearances() const
	{
		return m_bitfields.m_Appearances;
	}
  	unsigned int			GetGoals() const
	{
		return m_bitfields.m_Goals;
	}
  	unsigned int			GetBookings() const
	{
		return m_bitfields.m_Bookings;
	}
  	unsigned int			GetSendingOffs() const
	{
		return m_bitfields.m_SendingOffs;
	}
  	unsigned int			GetSuspensions() const
	{
		return m_bitfields.m_Suspensions; 			// number of times been suspended
	}
	void					DoClearSeasonDisciplinaryPoints()
	{
		m_bitfields.m_Bookings = 0;
		m_bitfields.m_SendingOffs = 0;
	}
	void					DoIncrementGoals()
	{
		m_bitfields.m_Goals++;
	}
	void					DoIncrementAppearances()
	{
		m_bitfields.m_Appearances++;
	}
	void					DoIncrementBookings()
	{
		m_bitfields.m_Bookings++;
	}
	void					DoIncrementSendingOffs()
	{
		m_bitfields.m_SendingOffs++;
	}

#ifdef ASSISTS
	ushort					GetAssists() const
	{
		return m_bitfields.m_Assists;
	}
	void					DoIncrementAssists()
	{
		m_bitfields.m_Assists++;
	}
#endif
	void					DoResetVariables()
	{
   		m_bitfields.m_Appearances = 0;
   		m_bitfields.m_Goals = 0;
		m_bitfields.m_Bookings = 0;
		m_bitfields.m_SendingOffs = 0;
		m_bitfields.m_Suspensions = 0;			// number of times been suspended
	#ifdef ASSISTS
		m_bitfields.m_Assists = 0;
	#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards = 0;
#endif
	}

	void DoGenerateInitialData(const ushort _CareerAppearances, const ePlayerPosition _Position)
	{
		m_bitfields.m_Appearances = _CareerAppearances;
		if (m_bitfields.m_Appearances > 4)
		{
			switch(_Position) 
			{
				case GOALKEEPER:
					// Number of goals conceded
					m_bitfields.m_Goals = m_bitfields.m_Appearances - m_bitfields.m_Appearances / 4 + RandomNumber.IntLessThan(m_bitfields.m_Appearances) / 4;
   					break;
				case ATTACKER:
					m_bitfields.m_Goals = m_bitfields.m_Appearances / 6 + RandomNumber.IntLessThan(m_bitfields.m_Appearances / 3);
   					break;
				default:
					m_bitfields.m_Goals = RandomNumber.IntLessThan(m_bitfields.m_Appearances / 6);
					break;
			}
		}
	}

	// IO Methods
	void			Write(CDataFile* _pDataFile)
	{
		_pDataFile->WriteBits(m_bitfields.m_Appearances, APPEARANCESBITS);
		_pDataFile->WriteBits(m_bitfields.m_Goals, GOALSBITS);
		_pDataFile->WriteBits(m_bitfields.m_Bookings, BOOKINGSBITS);
		_pDataFile->WriteBits(m_bitfields.m_SendingOffs, SENDINGOFFBITS);
		_pDataFile->WriteBits(m_bitfields.m_Suspensions, SUSPENSIONSBITS);
#ifdef ASSISTS
		_pDataFile->WriteBits(m_bitfields.m_Assists, ASSISTBITS);
#endif
#ifdef STOREMANOFMATCH
		_pDataFile->WriteBits(m_bitfields.m_ManOfMatchAwards, MANOFMATCHBITS);
#endif
	}

	void			Read(CDataFile* _pDataFile)
	{
		m_bitfields.m_Appearances = _pDataFile->ReadBits(APPEARANCESBITS);
		m_bitfields.m_Goals = _pDataFile->ReadBits(GOALSBITS);
		m_bitfields.m_Bookings = _pDataFile->ReadBits(BOOKINGSBITS);
		m_bitfields.m_SendingOffs = _pDataFile->ReadBits(SENDINGOFFBITS);
		m_bitfields.m_Suspensions = _pDataFile->ReadBits(SUSPENSIONSBITS);
#ifdef ASSISTS
		m_bitfields.m_Assists = _pDataFile->ReadBits(ASSISTBITS);
#endif
#ifdef STOREMANOFMATCH
		m_bitfields.m_ManOfMatchAwards = _pDataFile->ReadBits(MANOFMATCHBITS);
#endif
	}
};
#endif
