//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadtablewidget.cpp
// Description : Table that fills itself with basic squad data. Used for the strategy screens.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdsquadtablewidget.h>
#include <fdbuttonwidget.h>
#include <fdresourcemanager.h>
#include <fdscrollbarwidget.h>
#include <fdsound.h>
#include <fdtext.h>
#include <lemon/math/lmath.h>
#include <lemon/platform/lcore.h>
#include <framework/lsprite.h>
#include <screens/fdsquadscreen.h>

using namespace L_NAMESPACE_NAME;

#include <fdmainmenuui.h>

#include <defines.h>
#include <cstring.h>
#include <stringid.h>
#include <club.h>
#include <player.h>
#include <matchplay.h>

static bool CompareString( const char * str1, const char * str2 )
{
  while( *str1 != '\n' && *str1 != '\0' && *str2 != '\n' && *str2 != '\0')
  {
    char a[2]; a[0] = *str1; a[1] = '\0';
    char b[2]; b[0] = *str2; b[1] = '\0';
    int result = strcmp(a, b);
    if( result > 0 )
    {
      return true;
    }
    else if( result < 0 )
    {
      return false;
    }

    // Keep going till we get an answer
    str2++;
    str1++;
  }

  return false;
}

static int CompareFirstInitialLastName(const char *str1, const char *str2)
{
	const char *iter1 = str1;
	const char *iter2 = str2;

	int i = 0;
	bool bFirstInitialCheck = true;
	char cFI1 = 0;
	char cFI2 = 0;
	do
	{
		SkipTag(iter1);
		SkipTag(iter2);
		if ((*iter1 == 0) && (*iter2 != 0))
			return -1;
		if ((*iter2 == 0) && (*iter1 != 0))
			return 1;
		if ((*iter1 == 0) && (*iter2 == 0))
		{
			if (cFI1 < cFI2)
				return -1;
			if (cFI1 > cFI2)
				return 1;

			return 0;
		}

		// If we haven't processed the first initial yet, do so.
		if (bFirstInitialCheck == true)
		{
			if (*(iter1 + 1) == '.')
			{
				cFI1 = *iter1;
				iter1++;
				iter1++;
			}
			if (*(iter2 + 1) == '.')
			{
				cFI2 = *iter2;
				iter2++;
				iter2++;
			}

			bFirstInitialCheck = false;
			continue;
		}

		if (::toupper(*iter1) > ::toupper(*iter2))
			return 1;
		if (::toupper(*iter2) > ::toupper(*iter1))
			return -1;

		iter1++;
		iter2++;
	} while (true);
}

LUI_IMPLEMENT_RTTI( FDSquadTableWidget, lGUID(0,0,0,0), 0 );

void FDSquadTableWidget::ClassInitialiser()
{
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDSquadTableWidget::FDSquadTableWidget() : m_table(NULL), m_pClub(NULL)
{
	Init(lUIPoint(0, 0));
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDSquadTableWidget::FDSquadTableWidget(lUIWidget * parent, luint32_t widgetFlags, const char * id,
									   const lUIWidgetPosition anchor, const lUIWidgetPosition pos, const lUIWidgetSize size) :
	lUIWidget(parent, widgetFlags, id, anchor, pos, size), m_table(NULL), m_pClub(NULL)
{
	Init(lUIPoint(GetRect().w, GetRect().h));
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDSquadTableWidget::~FDSquadTableWidget()
{
}

//////////////////////////////////////////////////////////////////////////
// Called to initialize the widget.
//////////////////////////////////////////////////////////////////////////
void FDSquadTableWidget::Init(const lUIPoint cSize)
{
	m_eInfoShown = INFOTOSHOW_SKILLS;
	m_sortType = SORT_NUMBER;
	m_ascendingSort = true;
  m_bLimitTo11 = false;
  m_pLastSelectedPlayer = NULL;
  m_matchPlay = NULL;

	m_table = FD_GAME_NEW FDTableWidget(this, 0, "squadtable_table",
        //FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|
        FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_CELL_SELECT|
        FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
        FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
        LUI_LEFT_TOP, lUIPoint(0, 0), lUIPoint( cSize.x - GetDefaultScrollBarWidth(), cSize.y ));
	m_table->SignalCellDropped.Connect(this, &FDSquadTableWidget::OnSquadPlayerDropped);
	m_table->AllowSlideSelection(false);

	m_scroll = FD_GAME_NEW FDScrollBarWidget(this, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(cSize.x - GetDefaultScrollBarWidth(), 0), lUIPoint(GetDefaultScrollBarWidth(), cSize.y));
	m_table->SetScrollBar(m_scroll);

	m_table->SignalRowSelect.Connect( this, &FDSquadTableWidget::OnSquadRowSelect );
	m_table->SignalCellSelect.Connect( this, &FDSquadTableWidget::OnSquadCellSelect );

	m_table->SignalCellRendering.Connect( this,  &FDSquadTableWidget::RenderFaceCallback );

	m_faces = FDResourceManager::GetSprite( "uifaces", "default" );
}

//////////////////////////////////////////////////////////////////////////
// Returns the necessary size for the widget.
//////////////////////////////////////////////////////////////////////////
lUIPoint FDSquadTableWidget::GetIdealSize() const
{
	lUIPoint cSize = m_table->GetIdealSize();
  cSize.x += GetDefaultScrollBarWidth(); // scroll bar
	return cSize;
}

//////////////////////////////////////////////////////////////////////////
// Interface for setting the club displayed by the widget.
//////////////////////////////////////////////////////////////////////////
void FDSquadTableWidget::SetClub(CClub *pClub)
{
	m_pClub = pClub;
  m_pLastSelectedPlayer = NULL;

	// Update interface.
	if (m_table->GetRowCount() == 0)
		SetupTableColumns();

	RefreshTable();
}

void FDSquadTableWidget::SetInfoToShow(const SquadTableInfoType eInfo, bool bLimitTo11 )
{
	m_eInfoShown = eInfo;
	m_selectedColumnHeader = 0;
  m_bLimitTo11 = bLimitTo11;

  if( bLimitTo11 )
  {
    // hide scroll bar if only 11
    m_table->GetScrollBar()->Hide();
  }

    SetupTableColumns();
	RefreshTable();
}


void FDSquadTableWidget::RefreshTable()
{
	m_table->RemoveRows(1, m_table->GetRowCount() - 1);
	SetupPlayerData();
}

//////////////////////////////////////////////////////////////////////////
// Sets up the columns of the table and the table header, according to the
// mode that we're in.
//////////////////////////////////////////////////////////////////////////
void FDSquadTableWidget::SetupTableColumns()
{
	const float fFullTableWidth = 256.0f - 20.0f - 11.0f;

	m_table->ClearAll();

  m_table->AddColumn( lUICoord( 16.0f ) );  // squad number
  m_table->AddColumn( lUICoord( 82.0f ), LUI_LEFT_CENTER );   // player name

	// 101 pixels used by the first two columns. (101.25)

	switch( m_eInfoShown )
	{
		case INFOTOSHOW_SKILLS: // Skills
			// 20 pixels used by the spacers. 111 pixels remaining.
			m_table->AddColumn( lUICoord( 27.0f ) ); // Overall Skill
      m_table->AddColumn( lUICoord(5.0f) );
			m_table->AddColumn( lUICoord( 20.0f ) ); // Handling
			m_table->AddColumn( lUICoord(5.0f) );
			m_table->AddColumn( lUICoord( 20.0f ) ); // Tackling
			m_table->AddColumn( lUICoord(5.0f) );
			m_table->AddColumn( lUICoord( 20.0f ) ); // Passing
			m_table->AddColumn( lUICoord(5.0f) );
			m_table->AddColumn( lUICoord( 20.0f ) ); // Shooting
			break;

		case INFOTOSHOW_STATUS: // Status
			m_table->AddColumn( lUICoord( 127.0f ) );  // status
			break;

		case INFOTOSHOW_CONTRACT: // Contract
			m_table->AddColumn( lUICoord( 61.0f ) );  // wage
			m_table->AddColumn( lUICoord( 5.0f ) );  // gap
      m_table->AddColumn( lUICoord( 61.0f ) );  // contract end date
			break;

		case INFOTOSHOW_STATS: // Stats
			m_table->AddColumn( lUICoord( 29.0f ) ); // Age
			m_table->AddColumn( lUICoord( 5.0f ) ); // gap
			m_table->AddColumn( lUICoord( 28.0f ) ); // Appearances
			m_table->AddColumn( lUICoord( 5.0f ) ); // gap
			m_table->AddColumn( lUICoord( 28.0f ) ); // Goals
			m_table->AddColumn( lUICoord( 5.0f ) ); // gap
			m_table->AddColumn( lUICoord( 28.0f ) ); // Discipline
			break;

		case INFOTOSHOW_WELLBEING: // Stats
      m_table->AddColumn( lUICoord( 48.0f ) ); // Morale
			m_table->AddColumn( lUICoord( 5.0f ) ); // gap
      m_table->AddColumn( lUICoord( 74.0f ) ); // Fitness
			break;

		case INFOTOSHOW_MINIMAL:
			break;

    case INFOTOSHOW_ROLE:
			m_table->AddColumn( lUICoord( 27 ) );  // status
			break;

		default:
			lAssert(false);
			break;
	}

	// set headers
	m_table->AddRows( 1 );           // header

	m_table->SetRowTextColour( 0, L_ARGB(255,255,255,255) );
	m_table->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

	//TODO LOCALE
  m_table->SetText( 0, 0, lUIText( "NO" ) );
  m_table->SetText( 0, 1, lUIText( "      NAME" ) );

	switch( m_eInfoShown )
	{
		case INFOTOSHOW_SKILLS: // Skills
      m_table->SetText( 0, 2, lUIText( IDS_SKILL_ABBR ) );
      m_table->SetText( 0, 4, lUIText( IDS_HANDLING_ABBR ) );
      m_table->SetText( 0, 6, lUIText( IDS_TACKLING_ABBR ) );
      m_table->SetText( 0, 8, lUIText( IDS_PASSING_ABBR ) );
      m_table->SetText( 0, 10, lUIText( IDS_SHOOTING_ABBR ) );
			break;

		case INFOTOSHOW_STATUS: // Status
      m_table->SetText( 0, 2, lUIText( IDS_TAB0502 ) );
			break;

		case INFOTOSHOW_CONTRACT: // Contract
      m_table->SetText( 0, 2, lUIText( IDS_TAB0801 ) );
      m_table->SetText( 0, 4, lUIText( IDS_CONTRACTENDDATE ) );
			break;

		case INFOTOSHOW_STATS: // Stats
      m_table->SetText( 0, 2, lUIText( IDS_AGE ) );
      m_table->SetText( 0, 4, lUIText( IDS_APPEARANCES_ABBR ) );
      m_table->SetText( 0, 6, lUIText( IDS_GOALS ) );
      m_table->SetText( 0, 8, lUIText( IDS_TAB5005 ) );
			break;

		case INFOTOSHOW_WELLBEING: // Well being, morale and fitness
      m_table->SetText( 0, 2, lUIText( "MORALE" ) );
			m_table->SetText( 0, 4, lUIText( "FITNESS" ) );
			break;

		case INFOTOSHOW_MINIMAL:
			break;

    case INFOTOSHOW_ROLE:
      //TODO LOCALE
      m_table->SetText( 0, 2, lUIText( "ROLE" ) );
      break;

		default:
			lAssert(false);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Populates the table with player data.
//////////////////////////////////////////////////////////////////////////
void FDSquadTableWidget::SetupPlayerData()
{
	m_playerList.Empty();
	if (m_pClub == NULL)
		return;

  int count = lmMin(m_pClub->GetFirstTeamSquad().TotalSquadSize(), (ushort)PLAYERSINSQUAD);

  if( m_bLimitTo11 )
  {
    // limit to top 11
    count = lmMin( 11, count );
  }

	// set player info up
  lLogFmt("SetupPlayerData - col%d - %s\n", m_selectedColumnHeader, m_ascendingSort ? "ASC" : "DEC" );

	// create player list
	for (int ListPos = 0; ListPos < count; ListPos ++)
	{
		if( m_pClub->GetPlayerID(ListPos) == NOPLAYER )
			continue;

		CPlayer* pPlayer = &m_pClub->GetPlayer(ListPos);
		m_playerList.Push( FDPlayerTemp( pPlayer, m_playerList.Size() ) );
    lLogFmt("Player, %d %s - %d\n", m_playerList.Size(),
                               m_playerList.Last().player->GetName().CStr(),
                               m_playerList.Last().player->GetSkills().GetHandling());
	}

  // Sort player list
  for (int loopcount = 0 ; loopcount < m_playerList.Size(); loopcount++)
  {
    int changes = 0;
    for (int i1 = 0 ; i1 < m_playerList.Size()-1; i1++)
    {
      int i2 = i1+1;

      switch( m_sortType )
      {
        case SORT_NUMBER:
        {
          int number1 = 0;
          int number2 = 0;

            if( m_selectedColumnHeader == 0 )
            {
              number1 = m_playerList(i1).origNumber;
              number2 = m_playerList(i2).origNumber;
            }
            else
            {
              switch( m_eInfoShown )
              {
                case INFOTOSHOW_SKILLS: // Skills
                {
                  if(m_selectedColumnHeader == 2 )
                  {
                    number1 = m_playerList(i1).player->GetSkills().GetOverallSkill();
                    number2 = m_playerList(i2).player->GetSkills().GetOverallSkill();
                  }
                  else if(m_selectedColumnHeader == 4 )
                  {
                    number1 = m_playerList(i1).player->GetSkills().GetHandling();
                    number2 = m_playerList(i2).player->GetSkills().GetHandling();
                  }
                  else if(m_selectedColumnHeader == 6 )
                  {
                    number1 = m_playerList(i1).player->GetSkills().GetTackling();
                    number2 = m_playerList(i2).player->GetSkills().GetTackling();
                  }
                  else if(m_selectedColumnHeader == 8 )
                  {
                    number1 = m_playerList(i1).player->GetSkills().GetPassing();
                    number2 = m_playerList(i2).player->GetSkills().GetPassing();
                  }
                  else if(m_selectedColumnHeader == 10 )
                  {
                    number1 = m_playerList(i1).player->GetSkills().GetShooting();
                    number2 = m_playerList(i2).player->GetSkills().GetShooting();
                  }
                }
                break;
                case INFOTOSHOW_STATUS: // Status
                  if(m_selectedColumnHeader == 2 )
                  {
                    CString str1 = m_playerList(i1).player->MatchAvailabilityStr();
                    CString str2 = m_playerList(i2).player->MatchAvailabilityStr();

                    if( m_ascendingSort )
                    {
                      if( CompareString( str1, str2 ) )
                      {
                        number1++;
                      }
                    } else
                    {
                      if( !CompareString( str1, str2 ) )
                      {
                        number2++;
                      }
                    }
                  }
                  break;
                case INFOTOSHOW_WELLBEING: // Well being
                  if(m_selectedColumnHeader == 2 )
                  {
                    // morale
                    number1 = m_playerList(i1).player->GetMorale();
                    number2 = m_playerList(i2).player->GetMorale();
                  } else if(m_selectedColumnHeader == 4 )
                  {
                    // fitness
                    number1 = m_playerList(i1).player->GetSkills().GetFitness();
                    number2 = m_playerList(i2).player->GetSkills().GetFitness();
                  }

                  break;
                case INFOTOSHOW_CONTRACT: // Contract
                  if( m_pClub->IsUserControlled() == false )
                  {
                    number1 = number2 = 0;
                  } else
                  {
                    if(m_selectedColumnHeader == 2 )
                    {
                      number1 = m_playerList(i1).player->GetWage();
                      number2 = m_playerList(i2).player->GetWage();
                    } else if(m_selectedColumnHeader == 4 )
                    {
                      const CString &str1 = m_playerList(i1).player->ContractMediumStr();
                      const CString &str2 = m_playerList(i2).player->ContractMediumStr();

                      if( m_ascendingSort )
                      {
                        if( CompareString( str1.CStr(), str2.CStr() ) )
                        {
                          number1++;
                        }
                      } else
                      {
                        if( !CompareString( str1.CStr(), str2.CStr() ) )
                        {
                          number2++;
                        }
                      }
                    }
                  }
                  break;
                case INFOTOSHOW_STATS: // Stats
                  {
                    if(m_selectedColumnHeader == 2 )
                    {
                      number1 = m_playerList(i1).player->GetAge();
                      number2 = m_playerList(i2).player->GetAge();
                    }
                    else if(m_selectedColumnHeader == 4 )
                    {
                      number1 = m_playerList(i1).player->GetNumberAppearancesThisSeason();
                      number2 = m_playerList(i2).player->GetNumberAppearancesThisSeason();
                    }
                    else if(m_selectedColumnHeader == 6 )
                    {
                      number1 = m_playerList(i1).player->GetNumberGoalsThisSeason();
                      number2 = m_playerList(i2).player->GetNumberGoalsThisSeason();
                    }
                    else if(m_selectedColumnHeader == 8 )
                    {
                      number1 = m_playerList(i1).player->GetDiscipline().GetDisciplinaryPoints();
                      number2 = m_playerList(i2).player->GetDiscipline().GetDisciplinaryPoints();
                    }
                  }
                  break;
                default:
                  break;
              }
            }

          if( m_ascendingSort )
          {
            if( number1 > number2 )
            {
              lLogFmt("AscSwap %d, %d\n", i1, i2);
              lSwap( m_playerList(i1), m_playerList(i2) );
              changes++;
            }
          }
          else
          {
            if( number2 > number1 )
            {
              lLogFmt("DecSwap %d, %d\n", i1, i2);
              lSwap( m_playerList(i1), m_playerList(i2) );
              changes++;
            }
          }

          break;
        }
        case SORT_NAME:
        {
          const CString &str1 = m_playerList(i1).player->GetName();
          const CString &str2 = m_playerList(i2).player->GetName();
          lLogFmt("P1, %d %s, P2, %d %s\n", i1, str1, i2, str2);

          if( m_ascendingSort )
          {
            if( CompareFirstInitialLastName( str1.CStr(), str2.CStr() ) > 0 )
            {
              lSwap( m_playerList(i1), m_playerList(i2) );
              changes++;
            }
          } else
          {
            if( CompareFirstInitialLastName( str1.CStr(), str2.CStr() ) < 0 )
            {
              lSwap( m_playerList(i1), m_playerList(i2) );
              changes++;
            }
          }
          break;
        }
        default:
          break;
      }
    }

    if(!changes) break;
  }

	for (int ListPos = 0; ListPos < count; ListPos ++)
	{
    if( m_pClub->GetPlayerID(ListPos) == NOPLAYER )
			continue;
		CPlayer* pPlayer = m_playerList(ListPos).player;
		AddPlayerToTable(pPlayer, ListPos);
	}
}

void FDSquadTableWidget::AddPlayerToTable(CPlayer *pPlayer, const int ListPos)
{
	// idx is the row being added
	const int idx = (int)m_table->GetRowCount() - 1;

    m_table->AddRows( 1 );

    lLogFmt("%d: %s - %d\n", ListPos, pPlayer->GetName(), pPlayer->GetSkills().GetHandling());

    // work out colour to use
    COLORREF ColourToUse = 0;

    if( m_playerList(ListPos).origNumber < PLAYERSINTEAM )
	{
		if( m_playerList(ListPos).origNumber == 0 )
		{
			ColourToUse = PosnColour[GOALKEEPER];
		}
		else
		{
		    if( m_matchPlay != NULL )
		    {
          CTactic * curTactic = m_matchPlay->GetTactics( m_matchPlay->IsUserControlledClub( HOME ) ? HOME : AWAY );

		      ColourToUse = PosnColour[curTactic->PlayerPosition(m_playerList(ListPos).origNumber - 1)];
		    }
		    else
		    {
		      ColourToUse = PosnColour[WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics().PlayerPosition(m_playerList(ListPos).origNumber - 1)];
		    }
		}
	}
	else if( m_playerList(ListPos).origNumber < 16 )
	{
	  ColourToUse = YELLOW;
	}
	else
	{
		ColourToUse = CONCRETEGREY;
	}

	if (pPlayer->IsAvailableForMatch() == false)
	{
		// Unavailable
		lLogFmt("  Unavailable\n");
		ColourToUse = PALERED;
		m_table->SetRowFlash( idx+1, true );

    CString str("INJ");
    if( !pPlayer->IsInjured() ) // Suspended
      str  = "SUS";
		m_table->SetText( idx+1, 0, lUIText( str ) );
	}
  else if (m_playerList(ListPos).origNumber < PLAYERSINTEAM + 5)
	{
		// set player number
		CString str;
		//str.Format("%d", ListPos+1);
		str.Format("%d", m_playerList(ListPos).origNumber+1);

		m_table->SetText( idx+1, 0, lUIText( str ) );
	}

    // set position colour, which indicates first team/subs/availability.
	m_table->SetBkgColour(idx + 1, 0, ColourToUse);


  ColourToUse = PosnColour[m_playerList(ListPos).player->GetSkills().GetPosition()];

    // set name
    m_table->SetText( idx+1, 1, lUIText( FDMakePlayerNameLink(pPlayer) ) );

	// Set name background colour, to indicate position.
	m_table->SetBkgColour(idx + 1, 1, ColourToUse);

    // set skills
    CString str;

    switch( m_eInfoShown )
	{
	case INFOTOSHOW_SKILLS: // Skills
		str.Format("%d", pPlayer->GetSkills().GetOverallSkill());
		m_table->SetText( idx+1, 2, lUIText( str ) );

		str.Format("%d", pPlayer->GetSkills().GetHandling());
		m_table->SetText( idx+1, 4, lUIText( str ) );

		str.Format("%d", pPlayer->GetSkills().GetTackling());
		m_table->SetText( idx+1, 6, lUIText( str ) );

		str.Format("%d", pPlayer->GetSkills().GetPassing());
		m_table->SetText( idx+1, 8, lUIText( str ) );

		str.Format("%d", pPlayer->GetSkills().GetShooting());
		m_table->SetText( idx+1, 10, lUIText( str ) );

		for (int i = 0; i < 5; i++)
		{
			m_table->SetBkgColour(idx + 1, 2 + (i * 2), L_ARGB(0xFF, 0x50, 0x50, 0x50));
			m_table->SetTextColour(idx + 1, 2 + (i * 2), L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
		}

		// Spacer columns
		for (int i = 0; i < 4; i++)
		{
			m_table->SetBkgColour(idx + 1, 3 + (i * 2), CONCRETEGREY);
		}
		break;

	case INFOTOSHOW_STATUS: // Status
		m_table->SetText( idx+1, 2, lUIText( pPlayer->MatchAvailabilityStr() ) );
		m_table->SetBkgColour(idx + 1, 2, L_ARGB(0xFF, 0x50, 0x50, 0x50));
		m_table->SetTextColour(idx + 1, 2, L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
		break;

	case INFOTOSHOW_CONTRACT: // Contract

    if( pPlayer->GetClubPtr()->IsUserControlled() == true )
    {
      m_table->SetText( idx+1, 2, lUIText( WorldData().GetCurrentUserClub()->GetForeignStr(pPlayer->GetWage()) ) );
      m_table->SetText( idx+1, 4, lUIText( pPlayer->ContractMediumStr() ) );
    } else
    {
      m_table->SetText( idx+1, 2, lUIText( "n/a" ) );
      m_table->SetText( idx+1, 4, lUIText( "n/a" ) );
    }

    m_table->SetBkgColour(idx + 1, 2, L_ARGB(0xFF, 0x50, 0x50, 0x50));
    m_table->SetTextColour(idx + 1, 2, L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
    m_table->SetBkgColour(idx + 1, 4, L_ARGB(0xFF, 0x50, 0x50, 0x50));
    m_table->SetTextColour(idx + 1, 4, L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));

		// Spacer columns
    m_table->SetBkgColour(idx + 1, 3 , CONCRETEGREY);

		break;

	case INFOTOSHOW_STATS: // Stats
		str.Format("%d", pPlayer->GetAge());
		m_table->SetText( idx+1, 2, lUIText( str ) );

		str.Format("%d", pPlayer->GetNumberAppearancesThisSeason());
		m_table->SetText( idx+1, 4, lUIText( str ) );

		str.Format("%d", pPlayer->GetNumberGoalsThisSeason());
		m_table->SetText( idx+1, 6, lUIText( str ) );

		str.Format("%d", pPlayer->GetDiscipline().GetDisciplinaryPoints()); // pPlayer->GetDisciplinaryPointsThisSeason());
		m_table->SetText( idx+1, 8, lUIText( str ) );

		for (int i = 2; i <= 8; i+=2)
		{
			m_table->SetBkgColour(idx + 1, i, L_ARGB(0xFF, 0x50, 0x50, 0x50));
			m_table->SetTextColour(idx + 1, i, L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
		}

		// Spacer columns
		for (int i = 2; i < 8; i+=2)
      m_table->SetBkgColour(idx + 1, i + 1, CONCRETEGREY);
		break;

	case INFOTOSHOW_WELLBEING: // well being

	  // Morale uses graphics col 2

		//str.Format("%d", pPlayer->GetFitness());
		m_table->SetText( idx+1, 4, lUIText( pPlayer->GetSkills().GetFitnessStrID()  ) );

		for (int i = 2; i <= 4; i+=2)
		{
			m_table->SetBkgColour(idx + 1, i, L_ARGB(0xFF, 0x50, 0x50, 0x50));
			m_table->SetTextColour(idx + 1, i, L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
		}

		// Spacer columns
    m_table->SetBkgColour(idx + 1, 3, CONCRETEGREY);

		break;

	case INFOTOSHOW_MINIMAL:
		break;

  case INFOTOSHOW_ROLE:
    {
      m_table->SetBkgColour(idx + 1, 2, L_ARGB(0xFF, 0x80, 0x80, 0x80));

      CPlayerSquad& cSquad = pPlayer->GetClub().GetFirstTeamSquad();
      int roleCount = 0;

      if( idx == cSquad.GetCaptainSquadNum() ) roleCount++;
      if( idx ==	cSquad.GetPenaltyTakerSquadNum() ) roleCount++;
      if( idx == cSquad.GetCornerTakerSquadNum() ) roleCount++;
      if( idx == cSquad.GetFreeKickTakerSquadNum() ) roleCount++;

      if( roleCount == 1 )
      {
        if( idx == cSquad.GetCaptainSquadNum() )
        {
          m_table->SetText( idx+1, 2, lUIText( "Cap" ) );
        }
        else if( idx ==	cSquad.GetPenaltyTakerSquadNum() )
        {
          m_table->SetText( idx+1, 2, lUIText( "Pen" ) );
        }
        else if( idx == cSquad.GetCornerTakerSquadNum() )
        {
          m_table->SetText( idx+1, 2, lUIText( "Cnr" ) );
        }
        else if( idx == cSquad.GetFreeKickTakerSquadNum() )
        {
          m_table->SetText( idx+1, 2, lUIText( "Free" ) );
        }
      }
      else
      {
        CString shortRole;
        if( idx == cSquad.GetCaptainSquadNum() ) shortRole +="C";
        if( idx ==	cSquad.GetPenaltyTakerSquadNum() ) shortRole +="P";
        if( idx == cSquad.GetCornerTakerSquadNum() ) shortRole +="c";
        if( idx == cSquad.GetFreeKickTakerSquadNum() ) shortRole +="F";
        m_table->SetText( idx+1, 2, lUIText( shortRole ) );
      }
    }
		break;

	default:
		lAssert(false);
		break;
	}
}

void FDSquadTableWidget::OnSquadCellSelect( FDTableWidget * widget, int row, int col )
{
  lLogFmt("Row: %d, Column: %d\n", row, col);
  // Check for header select
  if( row == 0 )
  {
    // check whether to ignore
    bool bIgnoreClick = false;
    switch( m_eInfoShown )
    {
      case INFOTOSHOW_SKILLS: // Skills
        if( col == 3 ) bIgnoreClick = true;
        if( col == 5 ) bIgnoreClick = true;
        if( col == 7 ) bIgnoreClick = true;
        if( col == 9 ) bIgnoreClick = true;
        break;

      case INFOTOSHOW_CONTRACT: // Contract
        if( col == 3 ) bIgnoreClick = true;
        break;

      case INFOTOSHOW_STATS: // Stats
        if( col == 3 ) bIgnoreClick = true;
        if( col == 5 ) bIgnoreClick = true;
        if( col == 7 ) bIgnoreClick = true;
        break;

      case INFOTOSHOW_WELLBEING: // Stats
        if( col == 3 ) bIgnoreClick = true;
        break;

      default:
        break;
    }

    if( bIgnoreClick )
    {
      for(int i = 0; i < m_playerList.Size(); i++)
      {
        if( m_playerList(i).player == m_pLastSelectedPlayer )
        {
          // select row
          widget->SelectRow( i+1, false );
          return;
        }
      }
      return;
    }

    //
    int prevCol = m_selectedColumnHeader;
    if( col != 1 )
    {
      // Numbers
      m_sortType = SORT_NUMBER;
      m_selectedColumnHeader = col;
    }
    else
    {
      // Names
      m_sortType = SORT_NAME;
    }

    if( prevCol == m_selectedColumnHeader )
    {
      if( m_ascendingSort )
        m_ascendingSort = false;
      else
        m_ascendingSort = true;
    }

    // get index of currently selected player
    CPlayer* pLastPlayer = m_pLastSelectedPlayer;

    // flash column header and refresh
    m_table->FlashColumnHeader(col);
    RefreshTable();

    // reselect player
    SelectPlayer( pLastPlayer );
  }

  // Call normal row selection
  if( row > 0 )
    widget->SelectRow( row, false );
}

void FDSquadTableWidget::SelectPlayer(CPlayer *player, bool bAdjustScroll)
{
  for(int i = 0; i < m_playerList.Size(); i++)
  {
    if( m_playerList(i).player == player )
    {
      // select row
      m_table->SelectRow( i+1, false );

      // set scroll position
      if( bAdjustScroll )
      {
        int scrollPos = (i+1) - 5;
        if( scrollPos < 0 ) scrollPos = 0;

        int numShown = m_scroll->GetNumberOfItems() - scrollPos;

        if( numShown < m_scroll->GetItemsViewedAtOnce() )
        {
          scrollPos = m_scroll->GetNumberOfItems()-m_scroll->GetItemsViewedAtOnce();
          if( scrollPos < 0 ) scrollPos = 0;
        }

        m_table->SetScrollPosition(scrollPos, false);
        m_scroll->SetCurrentPosition(scrollPos, false);
      }

      break;
    }
  }
}


void FDSquadTableWidget::OnSquadRowSelect( FDTableWidget *table, int row )
{
	int playeridx = row-1;
	lLogFmt("%d playeridx selected\n", playeridx);

	// Set selection as current player
	CPlayer *pPlayer = NULL;
	if( playeridx >= 0 && playeridx < m_playerList.Size() )
  {
		pPlayer = m_playerList( playeridx ).player;
  }

  m_pLastSelectedPlayer = pPlayer;

  if( pPlayer != NULL )
  {
    SignalRowSelect.Emit(table, pPlayer, playeridx);
  } else
  {
    lLogFmt("NoSignalRowSelect\n");
  }
}

void FDSquadTableWidget::ClearSelection()
{
  m_pLastSelectedPlayer = NULL;
	m_table->SelectRow(0);
}

void FDSquadTableWidget::OnSquadPlayerDropped(FDTableWidget *, int row, int col, lUIPoint cPos)
{
	if (row == 0)
		return;

  if( m_eInfoShown == INFOTOSHOW_ROLE && col > 1)
  {
    return;
  }

	// Determine which player was dragged and dropped, based on the row, and trigger our event.
	lUIPoint cActualPos = cPos;
	cActualPos.x += m_table->GetRect().x;
	cActualPos.y += m_table->GetRect().y;

	SignalPlayerDropped.Emit(m_playerList(row - 1).player, cActualPos);
}

FDTableWidget *FDSquadTableWidget::GetUnderlyingTable()
{
	return m_table;
}

void FDSquadTableWidget::RenderFaceCallback(FDTableWidget * table, luint32_t row, luint32_t col, lUIRect rect )
{
  if( row == 0 )
    return;

  if( col == 2 && GetInfoToShow() == INFOTOSHOW_WELLBEING )
  {
    int spriteIndex = m_playerList( row-1 ).player->GetMorale() / 20;

    m_faces->SetFrame( spriteIndex );

    lUIPoint point;
    point.x = rect.x + ( rect.w - m_faces->GetWidth() ) * 0.5f;
    point.y = rect.y;

    m_faces->SetPosition( point.x, point.y );
    m_faces->Render();
  }
}

FDScrollBarWidget *FDSquadTableWidget::GetScrollBar()
{
	return m_scroll;
}

int FDSquadTableWidget::GetPlayerSquadIndex( int row )
{
  if( row >= 0 || row < m_playerList.Size() )
    return m_playerList(row).origNumber;

  return -1;
}

int FDSquadTableWidget::GetInfoToShow()
{
  return m_eInfoShown;
}

