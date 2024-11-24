

/*
    Class - CInjuryType


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



#ifndef __INJURYTYPE_H__
#define __INJURYTYPE_H__



class CInjuryType
{
public:
	enum eInjurySeverity
	{
		KNOCK,
		MINOR,
		MAJOR,
		SERIOUS,
		MAXINJURYSEVERITIES
	};

    CInjuryType()
		: m_MaxRnd(0)
		, m_Severity(KNOCK)
		, m_BasePeriod(0)
		, m_RndPeriod(0)
		, m_Plural(false)
		, m_MatchPerformanceEffect(0)
		, m_RndMatchPerformanceEffect(0)
		, m_CarryOff(false)
		, m_PhysioActive(false) 
	 {};

	void					DoInitialiseFromResource(CBinaryReader& _Resource);

	byte					GetMaxRnd() const
	{
		return 	m_MaxRnd;
	}
    byte					GetSeverity() const
	{
		return 	m_Severity;
	}
    byte					GetBasePeriod() const
	{
		return 	m_BasePeriod;
	}
	byte					GetRandomPeriod() const
	{
		return 	m_RndPeriod;
	}
    bool					GetPlural() const
	{
		return m_Plural;
	}
    byte					GetMatchPerformanceEffect() const
	{
		return 	m_MatchPerformanceEffect;
	}
    byte					GetRandomMatchPerformanceEffect() const
	{
		return 	m_RndMatchPerformanceEffect;
	}
    CStringID&				GetDescription()
	{
		return 	m_Description;
	}
	bool					GetCarryOff() const;

protected:
	byte					m_MaxRnd;
    byte					m_Severity;
    byte					m_BasePeriod;
    byte					m_RndPeriod;
    bool					m_Plural;
    byte					m_MatchPerformanceEffect;
    byte					m_RndMatchPerformanceEffect;
    CStringID				m_Description;
 	bool					m_CarryOff;
    bool					m_PhysioActive;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CInjuryTypeList : public CExtArray
{
public:
    ~CInjuryTypeList();

	void					DoInitialise();
	CInjuryType& GetAt(const ushort _ElementNumber);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);

	// Variables
private:
	CInjuryType*				m_pList;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CInjuryEventType : public CInjuryType
{
public:
    CInjuryEventType()
	{
		CInjuryType();
	}

	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	CStringID& GetPluralDescription()
	{
		return 	m_PluralDescription;
	}

protected:
    CStringID				m_PluralDescription;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CInjuryEventTypeList : public CExtArray
{
public:
    ~CInjuryEventTypeList();
	
	void					DoInitialise();
	CInjuryEventType& GetAt(const ushort _ElementNumber);
	
	// Variables
private:
	CInjuryEventType*				m_pList;
};

#endif