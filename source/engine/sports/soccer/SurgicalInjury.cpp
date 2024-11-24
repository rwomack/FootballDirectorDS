
 /*

    Class:- CSurgicalInjury


	Football Director

	(c) RW Software 1994 - 2004

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


//---------------------------------------------------------------------------------------
CSurgicalInjury& CSurgicalInjury::operator = (const CSurgicalInjury& p)
{
    m_WeeksToSurgery = p.m_WeeksToSurgery;
	m_PhysioAttention = p.m_PhysioAttention;
	m_SurgeryResult = p.m_SurgeryResult;
	return *this;
}


//----------------------------------------------------------------------------------------------------------------------------
void CSurgicalInjury::Write(CDataFile* File)
{
	ASSERT (File != NULL);
    File->Write(&m_WeeksToSurgery, sizeof(m_WeeksToSurgery));
    File->Write(&m_PhysioAttention, sizeof(m_PhysioAttention));
    File->Write(&m_SurgeryResult, sizeof(m_SurgeryResult));
}


//----------------------------------------------------------------------------------------------------------------------------
void CSurgicalInjury::Read(CDataFile* File)
{
	ASSERT (File != NULL);
    File->Read(&m_WeeksToSurgery, sizeof(m_WeeksToSurgery));
    File->Read(&m_PhysioAttention, sizeof(m_PhysioAttention));
    File->Read(&m_SurgeryResult, sizeof(m_SurgeryResult));
 }


// ---------------------------------------------------------------------------------------------------------------------------
void CSurgicalInjury::DoResetVariables(void)
{
	m_PhysioAttention = true;
	m_SurgeryResult = NOTFOUND;
	m_WeeksToSurgery = 0;               // No surgery event
}


// ---------------------------------------------------------------------------------------------------------------------------
const bool CSurgicalInjury::getRequiresPhysioAttention(void) const
{
	return m_PhysioAttention;
}


// ---------------------------------------------------------------------------------------------------------------------------
const int CSurgicalInjury::getSurgeryResult(void) const
{
	return m_SurgeryResult;
}


// ---------------------------------------------------------------------------------------------------------------------------
CString CSurgicalInjury::SurgeryResultStr(CClub& _Club)
{
	CString str;
	if (m_SurgeryResult != NOTFOUND)
	{
		str.Format(getSurgeryResult(), WorldData().getPlayer(m_PlayerID)->GetName(), _Club.GetName());
	}
	return str;
}


// ---------------------------------------------------------------------------------------------------------------------------
bool CSurgicalInjury::DailyImprove(CClub& _Club)
{
	if (m_WeeksToSurgery > 0)
	{
		m_WeeksToSurgery--;
		if (m_WeeksToSurgery == 0)
		{
			int Effect = RandomIntLessThan(100);
			if (Effect < 5)
			{
        m_SurgeryResult = IDS_SURGERYCOMPLICATIONS;
                m_HowLongRemainingInjured += 4 + RandomIntLessThan(4);
			}

			if (Effect > 4 && Effect < 10)
			{
        m_SurgeryResult = IDS_SURGERYSLIGHTCOMPLICATIONS;
                m_HowLongRemainingInjured += 2 + RandomIntLessThan(2);
			}

			if (Effect > 9 && Effect < 90)
			{
        m_SurgeryResult = IDS_SURGERYOK;
			}

			if (Effect > 89 && Effect < 95)
			{
        m_SurgeryResult = IDS_SURGERYGOOD;
                m_HowLongRemainingInjured -= 2;
			}

			if (Effect > 94)
			{
                m_HowLongRemainingInjured -= 4;
			}

			if (m_SurgeryResult != NOTFOUND)
			{
				CExtString Str;
				Str.Format(getSurgeryResult(), CNewsItem::PreprocessPlayerName(WorldData().getPlayer(CInjury::m_PlayerID)).CStr());
				_pClub.AddToClubNews(Str);
			}
		}
	}
	return CInjury::DailyImprove(_Club);
}


// ---------------------------------------------------------------------------------------------------------------------------
void CSurgicalInjury::DoCreateInjury(CPlayer& _Player, CClub& _Club, const CInjuryType::eInjurySeverity aSeverity, bool bTraining )
{
    DoResetVariables();
	CInjury::DoCreateInjury(_Player, _Club, aSeverity, bTraining );
	m_WeeksToSurgery = WorldData().getInjuryTypeList().GetAt(CInjury::GetType()).getWeeksToSurgery();
}


// ---------------------------------------------------------------------------------------------------------------------------
void CSurgicalInjury::DoCreateEvent(CPlayer& _Player, CClub& _Club)
{
    DoResetVariables();
	CInjury::DoCreateEvent(_Player, _Club);
	m_PhysioAttention = EventTypes[GetType()].PhysioActive;
}
