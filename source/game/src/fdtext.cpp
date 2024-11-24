//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// File        : fdtext.cpp
// Description : FD text functions
// Notes       :
//
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>

#include <lemon/lapi.h>

#include "stdafx.h"
#include <Club.h>
#include <Player.h>
#include <Calendar.h>

#include <lemon/object/lsharedptr.h>
#include <framework/lsprite.h>
#include <framework/lspritebank.h>
#include <framework/lfont.h>
#include <ui/luiwidget.h>
#include <fdtext.h>
#include <fdresourcemanager.h>

namespace L_NAMESPACE_NAME
{

static bool s_bLinksEnabled = false;
static bool s_bLinksEnabledOverride = false;

//////////////////////////////////////////////////////////////////////////
// Utility function used by the generic string sorting function.
//////////////////////////////////////////////////////////////////////////
void SkipTag(const char *&iter)
{
	if (*iter == 0)
		return;

	if (*iter == '#')
	{
		iter++;
		if (*iter == 0)
			return;

		if (*iter == '#')
		{
			// Color tag.
			iter += 7;
		}
		else if (*iter == '@')
		{
			// Link tag.
			iter++;
			while ((*iter != '@') && (*iter != 0))
				iter++;

			if (*iter == 0)
				return;

			iter++;
		}
		else
		{
			lAssert(false && "Malformed tag");
		}
	}

	if (*iter == '@')
	{
		// Assume we got here after a player/club link open.
		iter++;
	}
}

static char s_tmp[2048];
static char s_striptags_tmp[2048];
static int  s_tmpidx = 0;

const char *GetUnclosedTag(const char *pcString)
{
	const char *s = pcString;
	bool bInsideTag = false;
	int iAtCount = 0;
	int iTagCopyIndex = 0;

	// Testing
	::memset(s_tmp, 0, 48);

	while( *s != NULL )
	{
		// parse current character
		if( *s == '#' )
		{
			if (bInsideTag == false)
			{
				if (*(s + 1) == '@')
				{
					// Starting a tag.
					bInsideTag = true;
					iTagCopyIndex = 0;
					s_tmp[iTagCopyIndex++] = *s;

          if( *(s + 2) == 'N' || *(s + 2) == 'L' )
            iAtCount = 4;
          else
          	iAtCount = 2; // need to see 2 more at signs
					s++;
					s_tmp[iTagCopyIndex++] = *s;
				}
				else if (*(s + 1) == '#')
				{
					// Skip the color tag.
					s += 6;
				}
			}
		}
		else if (*s == '@')
		{
			if (bInsideTag == true)
			{
				s_tmp[iTagCopyIndex++] = *s;
				iAtCount--;
				if (iAtCount == 0)
				{
					bInsideTag = false;
				}
			}
		}
		else
		{
			if (bInsideTag == true)
			{
				s_tmp[iTagCopyIndex++] = *s;
			}
		}

		s++;
	}

	if (bInsideTag == false)
		return NULL;

	s_tmp[iTagCopyIndex] = 0;

	// Trim everything after the last @ to the end.
	const int iTagLength = ::strlen(s_tmp);
	if (s_tmp[iTagLength - 1] != '@')
	{
		int iIter = iTagLength - 1;
		while (iIter > 0)
		{
			if (s_tmp[iIter] == '@')
				break;
			iIter--;
		}

		s_tmp[iIter + 1] = 0;
	}

	return s_tmp;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Strip tags from text
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
const char *StripTags( const char *str )
{
  const char *s = str;
  char *out = s_striptags_tmp;

  while( *s != NULL )
  {
    // parse current character
    if( *s == '@' )
    {
      s ++;
    } else if( *s == '#' )
    {
      // parse tag
      s++;
      if( *s == '#' )
      {
        s += 7;
      } else if( *s == '@' )
      {
        int numterm = 2;

        if( *(s + 1) == 'N' || *(s + 1) == 'L' )
          numterm = 4;

        while( *s != NULL )
        {
          if( *s == '@' )
          {
            numterm--;
            if( numterm == 0 )
              break;
          }
          s++;
        }

		if (*s != NULL)
			s++;
      } else
      {
        s++;
      }
    } else
    {
      *out++ = *s++;
    }
  }

  *out++ = '\0';

  return s_striptags_tmp;
}


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Function for splitting text across multiple lines
// TODO: Ignore tagged text
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
lString SplitText( lFont* font, const lString &cText, float maxWidth )
{
  lString output = lString("");
  float stringWidth = 0;
  float stringHeight = 0;

  font->SetScale(1.0f);
  FDTextGetStringBound( font, stringWidth, stringHeight, cText.CStr() );
  //lLogFmt("SplitText: %s (%.2f, %.2f) - max %.2f\n", cText.CStr(), stringWidth, stringHeight, maxWidth);

  if( stringWidth < maxWidth )
  {
    return cText;
  }
  else
  {
    lString line = lString("");
    lString word = lString("");

    for( int i = 0; i < cText.Size(); i++ )
    {
      if(( cText(i) == ' ' ) && (GetUnclosedTag(word.CStr()) == NULL))
      {
        //Check line length
        FDTextGetStringBound( font, stringWidth, stringHeight, line.CStr() );
        float lineWidth = stringWidth;
        FDTextGetStringBound( font, stringWidth, stringHeight, word.CStr() );

        //lLogFmt("  line: %s (%.2f, %.2f)\n", line.CStr(), lineWidth, stringHeight);
        //lLogFmt("  word: %s (%.2f, %.2f)\n", word.CStr(), stringWidth, stringHeight);

        if( lineWidth + stringWidth > maxWidth )
        {
          output += line + lString("\n");
          line = lString("");

		  // Skip leading spaces on the next line.
		  do
		  {
			  i++;
		  } while ((i < cText.Size()) && (cText(i) == ' '));
		  i--;
        }

        line += word + lString(" ");
        word = lString("");
      }
      else if( cText(i) == '\n' )
      {
        //handle new line
        //Check line length
        FDTextGetStringBound( font, stringWidth, stringHeight, line.CStr() );
        float lineWidth = stringWidth;
        FDTextGetStringBound( font, stringWidth, stringHeight, word.CStr() );

        if( lineWidth + stringWidth > maxWidth )
        {
		  // Current word spills over, so insert a newline, the current word, and then another newline.
          output += line + lString("\n") + word + lString("\n");
          line = lString("");

		  // Skip leading spaces on the next line.
		  do
		  {
			  i++;
		  } while ((i < cText.Size()) && (cText(i) == ' '));
		  i--;
        }
		else
			output += line + word + lString("\n");

        line = lString("");
		word = lString("");
      }
	  else if (cText(i) != '\r')
      {
        //add letter
        word += lString(cText(i));
      }
    }

    //Add last word
    FDTextGetStringBound( font, stringWidth, stringHeight, line.CStr() );
    float lineWidth = stringWidth;
    FDTextGetStringBound( font, stringWidth, stringHeight, word.CStr() );

    //lLogFmt("  line: %s (%.2f, %.2f)\n", line.CStr(), lineWidth, stringHeight);
    //lLogFmt("  word: %s (%.2f, %.2f)\n", word.CStr(), stringWidth, stringHeight);

    if( lineWidth + stringWidth > maxWidth )
    {
      line += lString("\n");
    }
    output += line + word;
  }
  return output;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Render special text (extract links, colour and possibly font/size changes)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void    FDTextGetStringBound( lFont *font, int& outWidth, int& outHeight, const char* text )
{
  // return bounds of 'text'
  const char *newText = StripTags( text );
  font->GetStringBound( outWidth, outHeight, newText );
  //lLogFmt("  GetStringBound: %s (%dp, %dp)\n", newText, outWidth, outHeight);
}

void FDTextGetStringBound( lFont *font, float& outWidth, float& outHeight, const char* text )
{
  int width = 0;
  int height = 0;

  FDTextGetStringBound(font,width,height,text);

  outWidth = (float)width / float(lSpriteManager::GetDisplayWidth());
  outHeight = (float)height / float(lSpriteManager::GetDisplayHeight());
}


//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Render special text (extract links, colour and possibly font/size changes)
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#define FDTEXTLOG //lLogFmt

static int hexcolstrtoi(const char *hexstr)
{
  struct hexMap
  {
    char chr;
    int value;
  };

  hexMap map[16] =
  {
    {'0', 0}, {'1', 1},    {'2', 2}, {'3', 3},
    {'4', 4}, {'5', 5},    {'6', 6}, {'7', 7},
    {'8', 8}, {'9', 9},    {'A', 10}, {'B', 11},
    {'C', 12}, {'D', 13},  {'E', 14}, {'F', 15}
  };

  // only read 6 values for colour
  int result = 0;
  for(int idx = 0; idx < 6; idx++)
  {
    bool found = false;
    for (int i = 0; i < 16; i++)
    {
      if (hexstr[ idx ]  == map[i].chr)
      {
        if(idx>0) result <<= 4;
        result |= map[i].value;
        found = true;
        break;
      }
    }

    if (!found) break;
  }

  return result;
}

struct linkDef
{
  int  x, y;
  int  w, h;
  bool bPlayer;
  bool bMatch;
  bool bCup;
  bool bLeague;
  int  idx;
  int  roundIdx;
  int  matchIdx;
};

static int s_origx = 0;
static lArray<linkDef> s_links;

static void DoProcessing( lFont *font, int &curx, int &cury, bool bNextLine, linkDef *link = NULL )
{
  int origColour = font->GetColour();

  if( link != NULL && (s_bLinksEnabled || s_bLinksEnabledOverride))
  {
    //font->SetColour( L_ARGB( 255, 111, 49, 152 ) );
	  font->SetColour( L_ARGB(255, 255, 170, 0 ) );
  }

  // terminate string
  s_tmp[ s_tmpidx ] = '\0';

  // get bound of string to render
  int width;
  int height;
  FDTextGetStringBound(font, width, height, s_tmp );

  if( link != NULL )
  {
    FDTEXTLOG("LINK: %s - %d, %d -> %d, %d\n", s_tmp, curx, cury, curx+width, cury+height);
    link->x = curx;
    link->y = cury;
    link->w = width;
    link->h = height;
  }

  // render string
  FDTEXTLOG("  Printing '%s' at %d, %d\n", s_tmp, curx, cury);
  font->Print( int(curx), int(cury), s_tmp );

  // advance position
  if( bNextLine )
  {
    // next line
    FDTEXTLOG(" next line\n");
    curx = s_origx;
    cury += font->DefaultPixelHeight();
  } else
  {
    // advance on this line
    FDTEXTLOG(" same line\n");
    curx += width;
  }

  s_tmpidx = 0;
  font->SetColour( origColour );
}

void    FDTextPrint( lFont *font, int x, int y, const char* text )
{
  s_origx = x;

  // preserve original colour
  uint32 origColour = font->GetColour();

  // process each line.

  FDTEXTLOG("Input str: %s\n", text);

  char * s = const_cast<char *>(text);

  uint32 nextCol = origColour;
  int curx = x;
  int cury = y;

  while( 1 )
  {
    // parse current character
    if( *s == '#' )
    {
      // parse tag
      if( s[1] == '#' )
      {
        // render current text before colour change
        DoProcessing( font, curx, cury, false );

        // parse colour
        int col = 0xff000000 | hexcolstrtoi( &s[2] );
        FDTEXTLOG("Got colour %08x\n", col);
        if( s_bLinksEnabled || s_bLinksEnabledOverride )
        {
          font->SetColour( col );
        }

        // skip tag
        s += 8;
      } else if( s[1] == '@' )
      {
        // parse link
        if( s[2] == 'P' || s[2] == 'C' || s[2] == 'M' || s[2] == 'N' || s[2] == 'L' )
        {
          // render current text before link
          DoProcessing( font, curx, cury, false );

          // get link
          bool bPlayer = (s[2] == 'P') ? true : false;
          bool bMatch = (s[2] == 'M') ? true : false;
          bool bCup = (s[2] == 'N') ? true : false;
          bool bLeague = (s[2] == 'L') ? true : false;

          s += 3;

          // valid link
          int idx = 0;
          while(1)
          {
            int digit = *s - '0';
            if (digit < 0 || digit > 9)
              break;

            idx *= 10;
            idx += digit;

            s++;
          }

          int roundIdx = 0;
          int matchIdx = 0;
          if( bCup || bLeague )
          {
            s++;
            while(1)
            {
              int digit = *s - '0';
              if (digit < 0 || digit > 9)
                break;

              roundIdx *= 10;
              roundIdx += digit;

              s++;
            }

            s++;
            while(1)
            {
              int digit = *s - '0';
              if (digit < 0 || digit > 9)
                break;

              matchIdx *= 10;
              matchIdx += digit;

              s++;
            }
          } else
          {
            roundIdx  = -1;
            matchIdx  = -1;
          }

          if( s[0] != '@' )
          {
            FDTEXTLOG("Error no number termination char\n");
            continue;
          }

          s++;

          // Check if there's a newline in the link text. If so, then
          // go to the next line immediately before trying to render it.
          CheckSplitLink:
          bool bSplitLink = false;
          char *iter_s = s;
          while (*iter_s != NULL)
          {
            if (*iter_s == '\n')
            {
              bSplitLink = true;
              break;
            }
            if (*iter_s == '@')
              break;

            iter_s++;
          }

          // get label
          while( *s != NULL )
          {
            if( *s == '@' )
              break;

            if (*s == '\n')
            {
              // If we have a split link, then render each part of the link
              // separately.
              break;
            }
            else
				    s_tmp[ s_tmpidx++ ] = *s++;
          }

          s_tmp[ s_tmpidx ] = '\0';
          s++;

          // Grab coordinates of text and add to array for checking
          FDTEXTLOG("Got link to %s %d\n", bPlayer ? "Player" : "Club", idx);
          FDTEXTLOG("Label: '%s'\n", s_tmp);

          s_links.Add(1);
          s_links.Last().bPlayer = bPlayer;
          s_links.Last().bMatch = bMatch;
          s_links.Last().bCup = bCup;
          s_links.Last().bLeague = bLeague;
          s_links.Last().idx     = idx;
          s_links.Last().roundIdx     = roundIdx;
          s_links.Last().matchIdx     = matchIdx;

          // render link
          DoProcessing( font, curx, cury, bSplitLink, &s_links.Last() );

          // This is not the easiest-to-understand code in the world, but essentially
          // what happens here is that if the link text is split across several different
          // lines, we want to add the same link for each fragment.
          if (bSplitLink == true)
            goto CheckSplitLink;
        }
      } else
      {
        s++;
      }
    } else
    if( *s == '\n' || *s == '\0' )
    {
      FDTEXTLOG("Got EOL or EOS\n");

      if( *s == '\n' )
      {
        DoProcessing( font, curx, cury, true );
      } else
      {
        DoProcessing( font, curx, cury, false );
      }

      if( *s == '\0' )
      {
        // done
        break;
      }

      s++;
    }
    else
    {
      // copy character
      s_tmp[ s_tmpidx++ ] = *s++;

      if( s_tmpidx == 127 )
      {
        // about to run out of space
        FDTEXTLOG("Out of temp space\n");
        DoProcessing( font, curx, cury, false );
      }
    }
  }

  // restore original colour
  font->SetColour( origColour );
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Checks for links being pressed and make callback
// Function should be called at top of tick after input update and before new renders
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

// set up by fdmainmenuui.cpp
void FDClubLinkClick( int idx );
void FDPlayerLinkClick( int idx );
void FDMatchLinkClick( unsigned int idx );
void FDCupLinkClick( unsigned int cupIdx, unsigned int roundIdx, unsigned int matchIdx );
void FDLeagueLinkClick( unsigned int divIdx, unsigned int matchIdx, unsigned int date );

void    FDTextUpdate( float x, float y, bool bClick )
{
//  lLogFmt("Mouse %.2f, %.2f - %d\n", x, y, bClick);

  if( !(s_bLinksEnabled || s_bLinksEnabledOverride) )
  {
    // clear data for next frame of rendering
    s_links.Empty( s_links.Size() );

    return;
  }

  // process clicks if any
  if (bClick && (lUIWidget::ms_bLastMouseClickHandled == false))
  {
	for(int i = 0; i < s_links.Size(); i++)
	{
		linkDef &cur = s_links(i);
		if( cur.x <= x && cur.x+cur.w > x &&
			cur.y <= y && cur.y+cur.h > y )
		{
	//      lLogFmt("Mouse over %s %d\n", cur.bPlayer ? "Player" : "Club", cur.idx);

			if( bClick )
			{
				// Process click
				if( cur.bPlayer )
				{
					lLogFmt("Player %d link clicked\n", cur.idx);
					FDPlayerLinkClick( cur.idx );
					break;
				} else if( cur.bMatch )
				{
          lLogFmt("Match %d link clicked\n", cur.idx);
					FDMatchLinkClick( cur.idx );
					break;
				} else if( cur.bCup )
				{
          lLogFmt("cup %d link clicked, round %d, match %d\n", cur.idx, cur.roundIdx, cur.matchIdx);
					FDCupLinkClick( cur.idx, cur.roundIdx, cur.matchIdx );
					break;
				} else if( cur.bLeague )
				{
          lLogFmt("league %d link clicked, match %d\n", cur.idx, cur.roundIdx, cur.matchIdx);
					FDLeagueLinkClick( cur.idx, cur.roundIdx, cur.matchIdx );
					break;
				}
				else
				{
					lLogFmt("Club %d link clicked\n", cur.idx);
					FDClubLinkClick( cur.idx );
					break;
				}
			}
		}
	}
  }

  // clear data for next frame of rendering
  s_links.Empty( s_links.Size() );
}

lString FDMakePlayerNameLink(CPlayer *pPlayer)
{
	lString cStorage;
	cStorage.Format("#@P%u@%s@", (unsigned int)pPlayer->DoFindID(), pPlayer->GetName().CStr());
	return cStorage;
}

lString FDMakePlayerNameLink(CPlayer *pPlayer, const char *pcString)
{
	lString cStorage;
	cStorage.Format("#@P%u@%s@", (unsigned int)pPlayer->DoFindID(), pcString);
	return cStorage;
}

lString FDMakeClubNameLink(CClub *pClub)
{
	lString cStorage;
	cStorage.Format("#@C%u@%s@", (unsigned int)pClub->DoFindID().id, pClub->GetName().CStr());
	return cStorage;
}

lString FDMakeClubNameLink(CClub *pClub, const char *pcString)
{
	lString cStorage;
	cStorage.Format("#@C%u@%s@", (unsigned int)pClub->DoFindID().id, pcString);
	return cStorage;
}

CString FDEngineMakePlayerNameLink(CPlayer *pPlayer)
{
	CString cStorage;
	cStorage.Format("#@P%u@%s@", (unsigned int)pPlayer->DoFindID(), pPlayer->GetName().CStr());
	return cStorage;
}

CString FDEngineMakeClubNameLink(CClub *pClub)
{
	CString cStorage;
	cStorage.Format("#@C%u@%s@", (unsigned int)pClub->DoFindID().id, pClub->GetName().CStr());
	return cStorage;
}

CString FDEngineMakeMatchLink( CCalendar * pCalendar, const char *pcString)
{
	CString cStorage;
	cStorage.Format("#@M%u@%s@", (unsigned int ) pCalendar->GetMatchDay(), pcString);
	return cStorage;
}

CString FDEngineMakeCupLink( int cupIdx, int roundIdx, int matchIdx, const char *pcString)
{
	CString cStorage;
	cStorage.Format("#@N%u@%u@%u@%s@", (unsigned int ) cupIdx, (unsigned int ) roundIdx, (unsigned int) matchIdx, pcString);
	return cStorage;
}

CString FDMakeLeagueLink( int divIdx, int matchIdx, CCalendar * pCalendar, const char *pcString)
{
	CString cStorage;
	cStorage.Format("#@L%u@%u@%u@%s@", (unsigned int ) divIdx, (unsigned int) matchIdx, (unsigned int) pCalendar->GetMatchDay(), pcString);
	return cStorage;
}

void FDTextEnabled( bool bEnabled )
{
  s_bLinksEnabled = bEnabled;
}

bool FDTextIsEnabled()
{
  return s_bLinksEnabled;
}

bool FDTextIsEnabledOverride()
{
  return s_bLinksEnabledOverride;

}

void SetLinkEnabledOverride(bool bState)
{
	s_bLinksEnabledOverride = bState;
}


};
