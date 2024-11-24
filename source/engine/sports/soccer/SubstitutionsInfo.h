
/*

    File:- SubstitutionsInfo.h

    Log of all substitutions within the match


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __SUBSTITUTIONSINFO_H__
#define __SUBSTITUTIONSINFO_H__


class CSubstitutionsInfo
{
public:

	void					DoInitialise()
	{
		m_HomePlayersOffStr.clear();
		m_AwayPlayersOffStr.clear();
	}
	void					DoAdd(const byte aTimePlayed, const eWhichTeam aeWho, const ushort aPlayer1ID, const ushort aPlayer2ID)
	{
		CString str;
    str.Format(IDS_PLAYCOMINGON, aTimePlayed, WorldData().GetPlayer(aPlayer1ID)->GetName(), WorldData()->pPlayer(aPlayer2ID)->GetName());
		if (aeWho == HOME)
		{
			m_HomePlayersOffStr.push_back(str);
		}
		else
		{
			m_AwayPlayersOffStr.push_back(str);
		}
	}
	CString&				HomeSubOffStr(ushort x)
	{
		EXT_ASSERT(x < m_HomePlayersOffStr.size());
		return m_HomePlayersOffStr.at(x);
	}
	CString&				AwaySubOffStr(ushort x)
	{
		EXT_ASSERT(x < m_AwayPlayersOffStr.size());
		return m_AwayPlayersOffStr.at(x);
	}


protected:
//	std::vector<CString>	m_HomePlayersOffStr;
//	std::vector<CString>	m_AwayPlayersOffStr;
}
#endif
