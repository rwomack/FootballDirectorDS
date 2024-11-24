
/*

	File:- DivisionPlayOffInfo.h


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __DIVISIONPLAYOFFINFO_H__
#define __DIVISIONPLAYOFFINFO_H__

const ushort				NOPLAYOFF = 0xffff;


class CDivsionPlayOffInfo
{
public:
	ushort					GetID()
	{
		return m_ID;
	}

	byte					GetNumberClubs()
	{
		return m_NumberClubs;
	}

	void					DoInitialiseFromResource(CBinaryReader& _Resource)
	{
		m_ID = _Resource.ReadUInt16();
		m_NumberClubs = _Resource.ReadByte();
	}

	bool				IsInUse()
	{
		if (m_ID != NOPLAYOFF)
		{
			return true;
		}
		return false;
	}

	// Variables
private:
	ushort					m_ID;
	byte					m_NumberClubs;
};
///////////////////////////////////////////////////////////////////////////////////////////////
#endif