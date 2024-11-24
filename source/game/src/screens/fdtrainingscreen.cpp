//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTrainingscreen.cpp
// Description : FDTrainingScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdtrainingscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTrainingScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDTrainingScreen::FDTrainingScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTrainingScreen::~FDTrainingScreen()
{
}

void FDTrainingScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // *** Training ***

  m_trainingScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "training_screen" );
  m_trainingScreen->Hide();


  FDBackingWidget * styleBacking = FD_GAME_NEW FDBackingWidget ( m_trainingScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPointPercent( 8, 61 ), lUIPoint( 238, 40 ) );
  lUISpriteWidget * heading = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey1" ), LUI_LEFT_TOP, lUIPointPercent( 8, 63 ), lUIPoint( 238, 12 ) );
  // TODO LOCALE
  lUIText temp("Tactics Training Formation"); temp.ToUpper();
  m_trainingFormation = FD_GAME_NEW FDComboWidget( m_trainingScreen, 0, "training_formation_combo", temp, LUI_CENTER_CENTER, lUIPointPercent( 54, 75 ) );
  m_trainingFormation->SetAllToUpper( true );

  for(int i = 0; i < WorldData().GetCurrentUser()->GetTacticsList().TotalStandardTactics(); i++)
  {
    CTactic& tactic = WorldData().GetCurrentUser()->GetTacticsList().GetAt(i);
    m_trainingFormation->AddItem( lUIText( tactic.TitleStr() ) );
  }

  FDBackingWidget * styleBacking2 = FD_GAME_NEW FDBackingWidget ( m_trainingScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 37 ), lUIPoint( 238, 72 ) );
  lUISpriteWidget * heading1 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey1" ), LUI_LEFT_TOP, lUIPoint( 20, 41 ), lUIPoint( 238, 12 ) );
  lUISpriteWidget * heading2 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey1" ), LUI_LEFT_TOP, lUIPoint( 20, 76 ), lUIPoint( 238, 12 ) );

  lUISpriteWidget * numberStripe1 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey2" ), LUI_LEFT_TOP, lUIPoint( 118, 41 ), lUIPoint( 15, 12 ) );
  lUISpriteWidget * numberStripe2 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey2" ), LUI_LEFT_TOP, lUIPoint( 234, 41 ), lUIPoint( 15, 12 ) );
  lUISpriteWidget * numberStripe3 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey2" ), LUI_LEFT_TOP, lUIPoint( 118, 76 ), lUIPoint( 15, 12 ) );
  lUISpriteWidget * numberStripe4 = FD_GAME_NEW lUISpriteWidget ( m_trainingScreen, 0, "bacstyle_hdr", FDResourceManager::GetSprite( "uiicon","grad_grey2" ), LUI_LEFT_TOP, lUIPoint( 234, 76 ), lUIPoint( 15, 12 ) );

  temp = lUIText(IDS_STRING3601T); temp.ToUpper();
  m_trainingFiveASideSlider = FD_GAME_NEW FDSliderWidget( m_trainingScreen, 0, "five_a_side_slider", 0, temp,   LUI_CENTER_CENTER, lUIPointPercent( 32, 32 ), lUIPoint(100, 12) );
  temp = lUIText(IDS_STRING3602T); temp.ToUpper();
  m_trainingTacticsSlider   = FD_GAME_NEW FDSliderWidget( m_trainingScreen, 0, "tactics_slider",     0, temp,    LUI_CENTER_CENTER, lUIPointPercent( 77, 32 ), lUIPoint(100, 12) );
  temp = lUIText(IDS_STRING3603T); temp.ToUpper();
  m_trainingSetPiecesSlider = FD_GAME_NEW FDSliderWidget( m_trainingScreen, 0, "set_pieces_slider",  0, temp, LUI_CENTER_CENTER, lUIPointPercent( 32, 50 ), lUIPoint(100, 12) );
  temp = lUIText(IDS_STRING3604T); temp.ToUpper();
  m_trainingFreeTimeSlider  = FD_GAME_NEW FDSliderWidget( m_trainingScreen, 0, "free_time_slider",   0, temp,  LUI_CENTER_CENTER, lUIPointPercent( 77, 50 ), lUIPoint(100, 12) );

  m_trainingFiveASideSlider->SignalSliderChange.Connect( this, &FDTrainingScreen::OnSlider );
  m_trainingTacticsSlider->SignalSliderChange.Connect( this, &FDTrainingScreen::OnSlider );
  m_trainingSetPiecesSlider->SignalSliderChange.Connect( this, &FDTrainingScreen::OnSlider );
  m_trainingFreeTimeSlider->SignalSliderChange.Connect( this, &FDTrainingScreen::OnSlider );

  m_trainingFiveASideSlider->SetValueRange(0.0f, 100.0f);
  m_trainingTacticsSlider->SetValueRange(0.0f, 100.0f);
  m_trainingSetPiecesSlider->SetValueRange(0.0f, 100.0f);
  m_trainingFreeTimeSlider->SetValueRange(0.0f, 100.0f);

  m_trainingFiveASideButton = FD_GAME_NEW FDCheckBoxWidget( m_trainingScreen, 0, "five_lock", FDResourceManager::GetSprite("uiicon","unlocked"), FDResourceManager::GetSprite("uiicon","locked"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 96, 42));
  m_trainingFiveASideButton->SetChecked( false );
  m_trainingFiveASideButton->SignalControlCommand.Connect( this, &FDTrainingScreen::OnLocks );

  m_trainingTacticsButton = FD_GAME_NEW FDCheckBoxWidget( m_trainingScreen, 0, "tactics_lock", FDResourceManager::GetSprite("uiicon","unlocked"), FDResourceManager::GetSprite("uiicon","locked"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 212, 42));
  m_trainingTacticsButton->SetChecked( false );
  m_trainingTacticsButton->SignalControlCommand.Connect( this, &FDTrainingScreen::OnLocks );

  m_trainingSetPiecesButton = FD_GAME_NEW FDCheckBoxWidget( m_trainingScreen, 0, "pieces_lock", FDResourceManager::GetSprite("uiicon","unlocked"), FDResourceManager::GetSprite("uiicon","locked"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 96, 77));
  m_trainingSetPiecesButton->SetChecked( false );
  m_trainingSetPiecesButton->SignalControlCommand.Connect( this, &FDTrainingScreen::OnLocks );

  m_trainingFreeTimeButton = FD_GAME_NEW FDCheckBoxWidget( m_trainingScreen, 0, "ftime_lock", FDResourceManager::GetSprite("uiicon","unlocked"), FDResourceManager::GetSprite("uiicon","locked"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_TOP, lUIPoint( 212, 77));
  m_trainingFreeTimeButton->SetChecked( false );
  m_trainingFreeTimeButton->SignalControlCommand.Connect( this, &FDTrainingScreen::OnLocks );

  // *** Training Sub ***

  m_trainingScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "training_screen_sub" );
  m_trainingScreenSub->Hide();

  lUIPoint p( 50,42 );

  lUISpriteWidget * spriteBar = FD_GAME_NEW lUISpriteWidget(m_trainingScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_white"), LUI_TOP_LEFT,
    lUIPoint(0, p.y), lUIPoint(256, 12));
  spriteBar->GetSprite()->SetWidth( 256 );
  spriteBar->GetSprite()->SetColour( L_ARGB(128, 0, 0, 0 ) );

  lUITextWidget * title1 = FD_GAME_NEW lUITextWidget( m_trainingScreenSub, 0, "", lUIText( IDS_SCREEN27FORMATIONS), LUI_TOP_CENTER, lUIPoint(p.x + 16, p.y)  );
  lUITextWidget * title2 = FD_GAME_NEW lUITextWidget( m_trainingScreenSub, 0, "", lUIText( IDS_RATING ), LUI_TOP_CENTER, lUIPoint(p.x + 126, p.y) );
  p.y += 13;

  title1->SetTextToUpper();
  title2->SetTextToUpper();

  for( int i=0;i< m_trainingFormation->GetNumItems(); i++ )
  {
	  m_backgroundStripes.Push( FD_GAME_NEW lUISpriteWidget(m_trainingScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		  lUIPoint(0, p.y), lUIPoint(256, 12)));
	  m_backgroundStripes.Last()->GetSprite()->SetWidth( 256 );

	  FD_GAME_NEW lUISpriteWidget(m_trainingScreenSub, 0, "stripeDark", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
		  lUIPoint(p.x - 17, p.y), lUIPoint(65, 12));

    m_trainingFormationBars.Push( FD_GAME_NEW FDBarWidget(m_trainingScreenSub, 0, "bar", LUI_TOP_LEFT,
		  lUIPoint(p.x + 110, p.y), lUIPoint(35, 12)));

    FD_GAME_NEW lUITextWidget( m_trainingScreenSub, 0, "", m_trainingFormation->GetItem( i ), LUI_TOP_LEFT, lUIPoint(p.x + 5, p.y)  );

    m_trainingFormationRatings.Push( FD_GAME_NEW lUITextWidget( m_trainingScreenSub, 0, "",
                                     m_trainingFormation->GetItem( i ), LUI_TOP_LEFT, lUIPoint(p.x + 120, p.y) ) );
    p.y += 13;
  }
}

void FDTrainingScreen::DoShow()
{
  m_trainingScreen->Show();
  m_trainingScreenSub->Show();
}

void FDTrainingScreen::DoHide()
{
  m_trainingScreen->Hide();
  m_trainingScreenSub->Hide();
}

void FDTrainingScreen::Update()
{
  CSquadTraining&  rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();

  // update values
  m_trainingFiveASideSlider->SetValue( rTraining.Get5aSidePercent(), true);
  m_trainingTacticsSlider->SetValue(rTraining.GetTactics().GetPercentage(), true);
  m_trainingSetPiecesSlider->SetValue(rTraining.GetSetPiece().GetPercentage(), true);
  m_trainingFreeTimeSlider->SetValue(rTraining.GetRestingPercent(), true);

  // update ratins of formations
  for( int i=0;i< m_trainingFormation->GetNumItems(); i++ )
  {
    CTactic& tactic = WorldData().GetCurrentUser()->GetTacticsList().GetAt(i);

    lString num;
    num.Format("%d", tactic.GetFamiliarity() );

    m_trainingFormationRatings(i)->SetText( lUIText( num ) );

    /*if( tactic.GetFamiliarity() < 25 )
    {
      m_trainingFormationRatings(i)->SetTextColour( L_ARGB(255,238,14,14) );
    }
    else
    {
      m_trainingFormationRatings(i)->SetTextColour( L_ARGB(255,20,238,4) );
    }*/

    float fPercent = tactic.GetFamiliarity() / 100.0f;
    m_trainingFormationBars(i)->SetPctA( fPercent );

    if ( fPercent < 0.3f)
      m_trainingFormationBars(i)->SetColourA(L_ARGB(0xFF, 0xFF, 0, 0));
    else if (fPercent < 0.7f)
      m_trainingFormationBars(i)->SetColourA(L_ARGB(0xFF, 0xFF, 0xFF, 0));
    else
      m_trainingFormationBars(i)->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

    m_trainingFormationBars(i)->SetColourB(L_ARGB(0xFF, 0x30, 0x30, 0x30));
  }
}

//////////////////////////////////////////////////////////////////////////
// Populates the provided vector with a list of sliders that are unlocked.
//////////////////////////////////////////////////////////////////////////
void FDTrainingScreen::GetUnlockedSliderIndices(Vector<FDTrainingScreen::TrainingSliderType> &cResult)
{
	if (m_trainingFiveASideButton->IsChecked() == false)
		cResult.Add(FDTS_FIVEASIDE);
	if (m_trainingTacticsButton->IsChecked() == false)
		cResult.Add(FDTS_TACTICS);
	if (m_trainingSetPiecesButton->IsChecked() == false)
		cResult.Add(FDTS_SETPIECES);
	if (m_trainingFreeTimeButton->IsChecked() == false)
		cResult.Add(FDTS_FREETIME);
}

//////////////////////////////////////////////////////////////////////////
// Given a particular enumerated value, returns the slider associated with it.
//////////////////////////////////////////////////////////////////////////
FDSliderWidget *FDTrainingScreen::GetSlider(const TrainingSliderType eSlider)
{
	switch (eSlider)
	{
	case FDTS_FIVEASIDE:
		return m_trainingFiveASideSlider;
	case FDTS_TACTICS:
		return m_trainingTacticsSlider;
	case FDTS_SETPIECES:
		return m_trainingSetPiecesSlider;
	case FDTS_FREETIME:
		return m_trainingFreeTimeSlider;
	};

	EXT_ASSERT(false && "invalid slider type requested");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Returns the slider associated with the given checkbox.
//////////////////////////////////////////////////////////////////////////
FDSliderWidget *FDTrainingScreen::GetSliderForCheckBox(FDCheckBoxWidget *pCheck)
{
	if (pCheck == m_trainingFiveASideButton)
		return m_trainingFiveASideSlider;
	if (pCheck == m_trainingTacticsButton)
		return m_trainingTacticsSlider;
	if (pCheck == m_trainingSetPiecesButton)
		return m_trainingSetPiecesSlider;
	if (pCheck == m_trainingFreeTimeButton)
		return m_trainingFreeTimeSlider;

	EXT_ASSERT(false && "unknown check box widget");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Determines how much the specified sliders can actually change (respecting the upper and lower bounds of the slider).
//////////////////////////////////////////////////////////////////////////
int FDTrainingScreen::GetChangeableValueOfSliders(Vector<TrainingSliderType> &cSliders, const bool bWantToRemovePoints)
{
	int iReturn = 0;
	for (int i = 0; i < cSliders.GetSize(); i++)
	{
		if (bWantToRemovePoints == true)
			iReturn += (int)GetSlider(cSliders[i])->GetValue();
		else
			iReturn += (int)(100.0f - GetSlider(cSliders[i])->GetValue());
	}

	return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// Given a set of sliders to modify, and an amount of points to distribute/take away, modifies the
// values of the sliders accordingly.
//////////////////////////////////////////////////////////////////////////
void FDTrainingScreen::DistributeChangeToSliders(Vector<TrainingSliderType> &cSliders, int *values, const int iPoints, const bool bWantToRemovePoints)
{
	int iSliderTotal = 0;
	for (int i = 0; i < cSliders.GetSize(); i++)
		iSliderTotal += values[cSliders[i]];

	float fSliderProportions[FDTS_MAX];

	if (bWantToRemovePoints == true)
	{
		// If we're adding points to the active slider, remove the excess points from the
		// modifiable sliders in proportion to their current value.
		for (int i = 0; i < FDTS_MAX; i++)
		{
			if (cSliders.contains((TrainingSliderType)i) == true)
			{
				if (iSliderTotal != 0)
					fSliderProportions[i] = (float)values[i] / (float)iSliderTotal;
				else
					fSliderProportions[i] = 1.0f / (float)cSliders.GetSize();
			}
			else
			{
				fSliderProportions[i] = 0.0f;
			}
		}
	}
	else
	{
		// If we're removing points from the active slider, add them equally to each of the
		// modifiable sliders.
		for (int i = 0; i < FDTS_MAX; i++)
		{
			if (cSliders.contains((TrainingSliderType)i) == true)
			{
				fSliderProportions[i] = 1.0f / (float)cSliders.GetSize();
			}
			else
			{
				fSliderProportions[i] = 0.0f;
			}
		}
	}

	// Come up with an order in which to modify the sliders. Sort them by
	// the proportions calculated above.
	TrainingSliderType iModifyOrder[FDTS_MAX];
	for (int i = 0; i < (int)FDTS_MAX; i++)
		iModifyOrder[i] = (TrainingSliderType)i;

	for (int i = 0; i < (int)FDTS_MAX; i++)
	{
		for (int j = i + 1; j < (int)FDTS_MAX; j++)
		{
			if (fSliderProportions[iModifyOrder[i]] < fSliderProportions[iModifyOrder[j]])
			{
				lSwap(iModifyOrder[i], iModifyOrder[j]);
			}
		}
	}

	// Finally, distribute the points according to the order calculated above.
	int iPointsRemaining = iPoints;
	while (iPointsRemaining > 0)
	{
		for (int i = 0; i < cSliders.GetSize(); i++)
		{
			const float fCurrentProportion = fSliderProportions[iModifyOrder[i]];
			int &iCurrentValue = values[iModifyOrder[i]];

			if (fCurrentProportion == 0.0f)
				continue;

			if (bWantToRemovePoints)
			{
				const int iPointsDisbursed = lmMin(iCurrentValue, (int)lmMax(1.0f, fCurrentProportion * (float)iPointsRemaining));
				iCurrentValue -= iPointsDisbursed;
				iPointsRemaining -= iPointsDisbursed;
			}
			else
			{
				const int iPointsDisbursed = lmMin(100 - iCurrentValue, (int)lmMax(1.0f, fCurrentProportion * (float)iPointsRemaining));
				iCurrentValue += iPointsDisbursed;
				iPointsRemaining -= iPointsDisbursed;
			}

			if (iPointsRemaining <= 0)
				break;
		}
	};
}

//////////////////////////////////////////////////////////////////////////
// Event handler called when a slider's value is adjusted.
//////////////////////////////////////////////////////////////////////////
void FDTrainingScreen::OnSlider( lUIWidget * widget, float newValue)
{
  lLogFmt("OnSlider\n");
  int value[FDTS_MAX];

  value[FDTS_FIVEASIDE] = m_trainingFiveASideSlider->GetValue();
  value[FDTS_TACTICS] = m_trainingTacticsSlider->GetValue();
  value[FDTS_SETPIECES] = m_trainingSetPiecesSlider->GetValue();
  value[FDTS_FREETIME] = m_trainingFreeTimeSlider->GetValue();

  TrainingSliderType changedValue = (TrainingSliderType)-1;
  if( widget == m_trainingFiveASideSlider )
    changedValue = FDTS_FIVEASIDE;
  if( widget == m_trainingTacticsSlider )
    changedValue = FDTS_TACTICS;
  if( widget == m_trainingSetPiecesSlider )
    changedValue = FDTS_SETPIECES;
  if( widget == m_trainingFreeTimeSlider )
    changedValue = FDTS_FREETIME;

  const int iPointsAdded = lmMax(0, value[0] + value[1] + value[2] + value[3] - 100);
  const int iPointsRemoved = lmMax(0, 100 - value[0] - value[1] - value[2] - value[3]);
  if ((iPointsAdded == 0) && (iPointsRemoved == 0))
	  return;

  Vector<TrainingSliderType> cModifiableSliders;
  GetUnlockedSliderIndices(cModifiableSliders);
  cModifiableSliders.RemoveElement(changedValue);

  // Get the combined values of the modifiable sliders, and ensure that they actually
  // support the desired change (in terms of having enough points to remove, etc.).
  const int iTotalModifiablePoints = GetChangeableValueOfSliders(cModifiableSliders, (iPointsAdded > 0));
  if (iPointsAdded > 0)
  {
	  if (iTotalModifiablePoints < iPointsAdded)
	  {
		  // Couldn't add as many points as desired.
		  for (int i = 0; i < cModifiableSliders.GetSize(); i++)
			  value[cModifiableSliders[i]] = 0;

		  value[changedValue] -= iPointsAdded - iTotalModifiablePoints;
	  }
	  else
	  {
		  DistributeChangeToSliders(cModifiableSliders, value, iPointsAdded, true);
	  }
  }
  else
  {
	  if (iTotalModifiablePoints < iPointsRemoved)
	  {
		  // Couldn't remove as many points as desired.
		  for (int i = 0; i < cModifiableSliders.GetSize(); i++)
			  value[cModifiableSliders[i]] = 100;

		  value[changedValue] += iPointsRemoved - iTotalModifiablePoints;
	  }
	  else
	  {
		  DistributeChangeToSliders(cModifiableSliders, value, iPointsRemoved, false);
	  }
  }

  m_trainingFiveASideSlider->SetValue(value[FDTS_FIVEASIDE], true );
  m_trainingTacticsSlider->SetValue(value[FDTS_TACTICS], true);
  m_trainingSetPiecesSlider->SetValue(value[FDTS_SETPIECES], true);
  m_trainingFreeTimeSlider->SetValue(value[FDTS_FREETIME], true);

  // save values
  CSquadTraining&  rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();

  rTraining.Set5aSidePercent( m_trainingFiveASideSlider->GetValue() );
  rTraining.GetTactics().SetPercentage( m_trainingTacticsSlider->GetValue() );
  rTraining.GetSetPiece().SetPercentage( m_trainingSetPiecesSlider->GetValue() );
}

//////////////////////////////////////////////////////////////////////////
// Event handler called when one of the checkboxes controlling slider locking is clicked.
//////////////////////////////////////////////////////////////////////////
void FDTrainingScreen::OnLocks( lUIControlWidget * widget )
{
	FDCheckBoxWidget *pCheck = (FDCheckBoxWidget *)widget;
	int locked = 0;

	if( m_trainingFiveASideButton->IsChecked() )
		locked++;

	if( m_trainingTacticsButton->IsChecked() )
		locked++;

	if( m_trainingSetPiecesButton->IsChecked() )
		locked++;

	if( m_trainingFreeTimeButton->IsChecked() )
		locked++;

	if( locked > 2 && ((lUIButtonWidget *) widget)->IsChecked() )
	{
		pCheck->SetChecked(false);
	}
	else
	{
		// Disable or enable the slider associated with the checkbox. This
		// prevents it from sending further events.
		if (pCheck->IsChecked())
			GetSliderForCheckBox(pCheck)->Disable();
		else
			GetSliderForCheckBox(pCheck)->Enable();
	}
}
