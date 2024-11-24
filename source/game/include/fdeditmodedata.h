#ifndef FDEDITMODEDATA_H_INCLUDED
#define FDEDITMODEDATA_H_INCLUDED

#include <Defines.h>
#include <SoccerDefines.h>
#include <lemon/containers/lstring.h>
#include <fdpackedbitarray.h>

class CDataFile;

BEGIN_L_NAMESPACE

struct FDEditModeData
{
	enum
	{
    MAX_EDIT_CLUBS = 50,
    MAX_EDIT_PLAYERS = 500,
    MAX_EDIT_CLUB_NAME_LENGTH = 14,
    CLUB_NAME_LENGTH_STORAGE = 15,
    MAX_EDIT_PLAYER_NAME_LENGTH = 16,
    PLAYER_NAME_LENGTH_STORAGE = 17,
	};

	FDPackedBitArray<ushort, 9, MAX_EDIT_CLUBS, 0> m_cClubIDs;
	FDPackedNameCharBitArray<MAX_EDIT_CLUBS, CLUB_NAME_LENGTH_STORAGE> m_cClubNames;
	FDPackedBitArray<ushort, 12, MAX_EDIT_PLAYERS, 0> m_cPlayerIDs;
	FDPackedNameCharBitArray<MAX_EDIT_PLAYERS, PLAYER_NAME_LENGTH_STORAGE> m_cPlayerSurnames;
	FDPackedBitArray<char, 5, MAX_EDIT_PLAYERS, 'A'> m_cPlayerForenames;

  //

	FDEditModeData();
	void Clear();
	bool SaveGameFile(CDataFile *pFile);
	bool LoadGameFile(CDataFile *pFile);

	int GetFreeClubNameIndex() const;
	int GetFreePlayerNameIndex() const;

	bool IsClubIDModified(const ClubID cClubID) const;
	bool IsPlayerIndexModified(const ushort usPlayerIndex) const;

	int GetClubEntryIndex(const ClubID cClubID) const;
	int GetPlayerEntryIndex(const ushort usPlayerIndex) const;

	bool AddClubModification(const ClubID cClubID, const lString &sNewName);
	bool AddPlayerModification(const ushort usPlayerID, const lString &sNewSurname, const char cNewForename);

	bool ModifyClubModification(const int iModificationIndex, const lString &sNewName);
	bool ModifyPlayerModification(const int iModificationIndex, const lString &sNewSurname, const char cNewForename);

	bool RemoveClubModification(const ClubID cClubID);
	bool RemovePlayerModification(const ushort usPlayerIndex);
};

END_L_NAMESPACE

extern L_NAMESPACE_NAME::FDEditModeData &EditModeData();

#endif // #ifndef FDEDITMODEDATA_H_INCLUDED
