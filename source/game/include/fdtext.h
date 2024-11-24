//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtext.h
// Description : FDText interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTEXT_H_INCLUDED
#define FDTEXT_H_INCLUDED

class CString;
class CPlayer;
class CClub;
class CCalendar;

BEGIN_L_NAMESPACE

const char *GetUnclosedTag(const char *pcString);
void SkipTag(const char *&iter);
const char *StripTags( const char *str );
lString SplitText( lFont* font, const lString &text, float maxWidth );

void    FDTextGetStringBound( lFont *, int& outWidth, int& outHeight, const char* text );
void    FDTextGetStringBound( lFont *, float& outWidth, float& outHeight, const char* text );
void    FDTextPrint( lFont *, int x, int y, const char* text );
void    FDTextUpdate( float x, float y, bool bClick );

lString FDMakePlayerNameLink(CPlayer *pPlayer);
lString FDMakePlayerNameLink(CPlayer *pPlayer, const char *pcString);
lString FDMakeClubNameLink(CClub *pClub);
lString FDMakeClubNameLink(CClub *pClub, const char *pcString);

CString FDEngineMakePlayerNameLink(CPlayer *pPlayer);
CString FDEngineMakeClubNameLink(CClub *pClub);

CString FDEngineMakeMatchLink( CCalendar * pCalendar, const char *pcString);
CString FDEngineMakeCupLink( int cupIdx, int roundIdx, int matchIdx, const char *pcString);
CString FDMakeLeagueLink( int divIdx, int matchIdx, CCalendar * pCalendar, const char *pcString);

void FDTextEnabled( bool bEnabled );
bool FDTextIsEnabled();

void SetLinkEnabledOverride(bool bState);
bool FDTextIsEnabledOverride();


END_L_NAMESPACE

#endif


