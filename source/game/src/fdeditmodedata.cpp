#include <fdeditmodedata.h>
#include <lemon/platform/lcore.h>
#include <lemon/math/lmath.h>
#include <SoccerDefines.h>
#include <cdatafile.h>
#include <string.h>

using namespace L_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDEditModeData::FDEditModeData()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
// Returns the edit mode data structure to a default state.
//////////////////////////////////////////////////////////////////////////
void FDEditModeData::Clear()
{
  // blank all, clear any garbage as we'll save these later and they won't compress with garbage!
  m_cClubIDs.ClearAll();
  m_cClubNames.ClearAll();
  m_cPlayerIDs.ClearAll();
  m_cPlayerSurnames.ClearAll();
  m_cPlayerForenames.ClearAll();

  // actual initialisation
	for (int i = 0; i < MAX_EDIT_CLUBS; i++)
	{
		m_cClubIDs.Set(i, NOCLUB);
		m_cClubNames.SetString(i, NULL);
	}

	for (int i = 0; i < MAX_EDIT_PLAYERS; i++)
	{
		m_cPlayerIDs.Set(i, NOPLAYER);
		m_cPlayerSurnames.SetString(i, NULL);
		m_cPlayerForenames.Set(i, 'A');
	}
}

//////////////////////////////////////////////////////////////////////////
// Called during the saved-game saving process to write out the edit data
// for the saved game.
//////////////////////////////////////////////////////////////////////////
bool FDEditModeData::SaveGameFile(CDataFile *pFile)
{
	if (pFile->Write(m_cClubIDs.GetRawStorage(), m_cClubIDs.GetRawStorageSize()) != m_cClubIDs.GetRawStorageSize())
		return false;

	if (pFile->Write(m_cClubNames.GetRawStorage(), m_cClubNames.GetRawStorageSize()) != m_cClubNames.GetRawStorageSize())
		return false;

	if (pFile->Write(m_cPlayerIDs.GetRawStorage(), m_cPlayerIDs.GetRawStorageSize()) != m_cPlayerIDs.GetRawStorageSize())
		return false;

	if (pFile->Write(m_cPlayerForenames.GetRawStorage(), m_cPlayerForenames.GetRawStorageSize()) != m_cPlayerForenames.GetRawStorageSize())
		return false;

	if (pFile->Write(m_cPlayerSurnames.GetRawStorage(), m_cPlayerSurnames.GetRawStorageSize()) != m_cPlayerSurnames.GetRawStorageSize())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Called during the saved-game loading process to load the edit data for
// this saved game.
//////////////////////////////////////////////////////////////////////////
bool FDEditModeData::LoadGameFile(CDataFile *pFile)
{
	if (pFile->Read(m_cClubIDs.GetRawStorage(), m_cClubIDs.GetRawStorageSize()) != m_cClubIDs.GetRawStorageSize())
		return false;

	if (pFile->Read(m_cClubNames.GetRawStorage(), m_cClubNames.GetRawStorageSize()) != m_cClubNames.GetRawStorageSize())
		return false;

	if (pFile->Read(m_cPlayerIDs.GetRawStorage(), m_cPlayerIDs.GetRawStorageSize()) != m_cPlayerIDs.GetRawStorageSize())
		return false;

	if (pFile->Read(m_cPlayerForenames.GetRawStorage(), m_cPlayerForenames.GetRawStorageSize()) != m_cPlayerForenames.GetRawStorageSize())
		return false;

	if (pFile->Read(m_cPlayerSurnames.GetRawStorage(), m_cPlayerSurnames.GetRawStorageSize()) != m_cPlayerSurnames.GetRawStorageSize())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Returns the index of a free club name, or a negative value if none are
// available.
//////////////////////////////////////////////////////////////////////////
int FDEditModeData::GetFreeClubNameIndex() const
{
	for (int i = 0; i < MAX_EDIT_CLUBS; i++)
	{
		if (m_cClubIDs.Get(i) == (NOCLUB & (0x1ff)))
			return i;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
// Returns the index of a free player name, or a negative value if none are
// available.
//////////////////////////////////////////////////////////////////////////
int FDEditModeData::GetFreePlayerNameIndex() const
{
	for (int i = 0; i < MAX_EDIT_PLAYERS; i++)
	{
		if (m_cPlayerIDs.Get(i) == (NOPLAYER & 0xFFF))
			return i;
	}

	return -1;
}

bool FDEditModeData::IsClubIDModified(const ClubID cClubID) const
{
	return (GetClubEntryIndex(cClubID) >= 0);
}

bool FDEditModeData::IsPlayerIndexModified(const ushort usPlayerIndex) const
{
	return (GetPlayerEntryIndex(usPlayerIndex) >= 0);
}

int FDEditModeData::GetClubEntryIndex(const ClubID cClubID) const
{
	for (int i = 0; i < MAX_EDIT_CLUBS; i++)
	{
		if (m_cClubIDs.Get(i) == cClubID.id)
			return i;
	}

	return -1;
}

int FDEditModeData::GetPlayerEntryIndex(const ushort usPlayerIndex) const
{
	for (int i = 0; i < MAX_EDIT_PLAYERS; i++)
	{
		if (m_cPlayerIDs.Get(i) == usPlayerIndex)
			return i;
	}

	return -1;
}


bool FDEditModeData::AddClubModification(const ClubID cClubID, const lString &sNewName)
{
	// Find an unused entry.
	int iIndex = GetFreeClubNameIndex();
	if (iIndex < 0)
		return false;

	m_cClubIDs.Set(iIndex, cClubID.id);
	ModifyClubModification(iIndex, sNewName);

	return true;
}

bool FDEditModeData::AddPlayerModification(const ushort usPlayerID, const lString &sNewSurname, const char cNewForename)
{
	// Find an unused entry.
	int iIndex = GetFreePlayerNameIndex();
	if (iIndex < 0)
		return false;

	m_cPlayerIDs.Set(iIndex, usPlayerID);
	ModifyPlayerModification(iIndex, sNewSurname, cNewForename);

	return true;
}

bool FDEditModeData::ModifyClubModification(const int iModificationIndex, const lString &sNewName)
{
	if (m_cClubIDs.Get(iModificationIndex) == (NOCLUB & 0x1FF))
		return false;

	m_cClubNames.SetString(iModificationIndex, sNewName.CStr());
	return true;
}

bool FDEditModeData::ModifyPlayerModification(const int iModificationIndex, const lString &sNewSurname, const char cNewForename)
{
	if (m_cPlayerIDs.Get(iModificationIndex) == (NOPLAYER & 0xFFF))
		return false;

  m_cPlayerSurnames.SetString(iModificationIndex, sNewSurname.CStr());
	m_cPlayerForenames.Set(iModificationIndex, cNewForename);

	return true;
}



bool FDEditModeData::RemoveClubModification(const ClubID cClubID)
{
	const int iIndex = GetClubEntryIndex(cClubID);
	if (iIndex >= 0)
	{
		m_cClubIDs.Set(iIndex, NOCLUB);
		return true;
	}

	return false;
}

bool FDEditModeData::RemovePlayerModification(const ushort usPlayerIndex)
{
	const int iIndex = GetPlayerEntryIndex(usPlayerIndex);
	if (iIndex >= 0)
	{
		m_cPlayerIDs.Set(iIndex, NOPLAYER);
		return true;
	}

	return false;
}

