//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmarqueewidget.cpp
// Description : A flashy widget used to display special messages like goal scores.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmarqueewidget.h>
#include <fdresourcemanager.h>
#include <framework/lspritemanager.h>
#include <fdtext.h>
#include <lemon/math/lmath.h>
#include <lemon/platform/lcore.h>
#include <ui/luispritewidget.h>
#include <framework/lsprite.h>

using namespace L_NAMESPACE_NAME;

#include <fdmainmenuui.h>

#include <defines.h>
#include <cstring.h>
#include <stringid.h>
#include <club.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI(FDMarqueeWidget, lGUID(0, 0, 0, 0), 0);

//////////////////////////////////////////////////////////////////////////
// Static method required by the factory system.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::ClassInitialiser()
{
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDMarqueeWidget::FDMarqueeWidget()
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDMarqueeWidget::FDMarqueeWidget(lUIWidget * parent, luint32_t widgetFlags, const char * id,
								 const lUIWidgetPosition anchor, const lUIWidgetPosition pos,
								 const lUIWidgetSize size) : lUIWidget(parent, widgetFlags, id, anchor, pos, size)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDMarqueeWidget::~FDMarqueeWidget()
{
}

//////////////////////////////////////////////////////////////////////////
// Initialization method. Called by all constructors.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::Init()
{
	m_fTimeRemaining = 0.0f;
	m_bPause = false;

	m_pKit = FD_GAME_NEW lUISpriteWidget( this, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_CENTER_LEFT, lUIPoint( 5,  GetRect().h / 2.0f) );
	m_pKit->Hide();

	m_messageBar = FD_GAME_NEW lUISpriteWidget( this, 0, "bar", FDResourceManager::GetSprite("message_box_small","message_box_small"), LUI_CENTER_CENTER, lUIPoint( GetRect().w / 2.0f,  GetRect().h / 2.0f) );
	m_messageBar->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Renders the marquee.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::Render()
{
	if (m_fTimeRemaining <= 0.0f)
		return;

	switch (m_cCurrentMessage.m_eType)
	{
	case MMT_GOAL:
	case MMT_OWN_GOAL:
	case MMT_REDCARD:
  case MMT_YELLOWCARD:
	case MMT_PENALTYGOAL:
	case MMT_PENALTYSAVE:
	case MMT_PENALTYMISS:
	case MMT_INJURY:
	case MMT_CUSTOMMESSAGE:
		RenderSingleBarMessage();
		break;
	case MMT_SUBSTITUTE:
		RenderSubstituteMessage();
		break;
	default:
		lAssert(false && "Unknown message type");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Used to render any messages that are a single bar (text and kit).
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::RenderSingleBarMessage()
{
	lUIRect cDrawRect = WidgetToScreen(GetRect());
	luint32_t uiColourIndex1 = 1;
	luint32_t uiColourIndex2 = 0;
	luint32_t uiColour1 = 0xff000000;
	luint32_t uiColour2 = 0xffffffff;

	if( m_cCurrentMessage.m_eType != MMT_CUSTOMMESSAGE )
	{
		uiColourIndex1 = m_cCurrentMessage.m_pClub->GetHomeKit().GetPrimaryColour() - 1;
		uiColourIndex2 = m_cCurrentMessage.m_pClub->GetHomeKit().GetSecondaryColour() - 1;
		uiColour1 = GetKitColour(uiColourIndex1);
		uiColour2 = GetKitColour(uiColourIndex2);
	}

	if ((m_cCurrentMessage.m_eType == MMT_REDCARD) || (m_cCurrentMessage.m_eType == MMT_INJURY))
	{
		uiColour1 = 0xffff0000;
		uiColour2 = 0xffffffff;
	}

  if (m_cCurrentMessage.m_eType == MMT_YELLOWCARD)
	{
    uiColour1 = 0xffffff00;
    uiColour2 = 0xff000000;
	}


	const int iRoundedTime = (int)(m_fTimeRemaining + 0.5f);
	const luint32_t uiColourIndexToUse = (iRoundedTime % 2 == 1) ? uiColourIndex2 : uiColourIndex1;
	const luint32_t uiColourToUse = (iRoundedTime % 2 == 1) ? uiColour2 : uiColour1;
	const luint32_t uiColourToUseOpposite = (iRoundedTime % 2 == 1) ? uiColour1 : uiColour2;

  if( m_cCurrentMessage.m_eType != MMT_CUSTOMMESSAGE )
	  lSpriteManager::DrawRect2(cDrawRect.x, cDrawRect.y, cDrawRect.w, cDrawRect.h, uiColourToUse);

	lSharedPtr<lFont> cFont;
	lUIText cString;

	switch (m_cCurrentMessage.m_eType)
	{
	case MMT_GOAL:
	case MMT_PENALTYGOAL:
		cFont = FDResourceManager::GetFont("font26", "font26");
    cString = lUIText(IDS_GOALMARQUEE);
		break;
	case MMT_OWN_GOAL:
		cFont = FDResourceManager::GetFont("font26", "font26");
    cString = lUIText(IDS_OWNGOALMARQUEE);
		break;
  case MMT_YELLOWCARD:
  {
		// TODO LOCALE
		cFont = FDResourceManager::GetFont("font26", "font26");
    cString = lUIText(m_cCurrentMessage.m_message);
		break;
  }
	case MMT_REDCARD:
  {
		// TODO LOCALE
		cFont = FDResourceManager::GetFont("font26", "font26");
    cString = lUIText(m_cCurrentMessage.m_message);
		break;
  }
	case MMT_PENALTYMISS:
		// TODO LOCALE
		cFont = FDResourceManager::GetFont("font26", "font26");
		cString = lUIText("MISS!");
		break;
	case MMT_PENALTYSAVE:
		// TODO LOCALE
		cFont = FDResourceManager::GetFont("font26", "font26");
    cString = lUIText("SAVED!");
		break;
	case MMT_INJURY:
		{
			cFont = FDResourceManager::GetFont("font16", "font16");

			CString cInjuryMessage;
			cInjuryMessage.Format("INJURED - %s", m_cCurrentMessage.m_pPlayer->GetName().CStr());
			cString = lUIText(cInjuryMessage.CStr());
			break;
		}
  case MMT_CUSTOMMESSAGE:
    {
      m_messageBar->Render();
      cFont = FDResourceManager::GetFont("font26", "font26");
      cString = lUIText( m_cCurrentMessage.m_message );
      break;
    }
	default:
		lAssert(false && "unknown message type");
		break;
	};

	float fStringWidth, fStringHeight;
	FDTextGetStringBound(cFont.Get(), fStringWidth, fStringHeight, cString.str.CStr());
	fStringWidth *= lSpriteManager::GetDisplayWidth();
	fStringHeight *= lSpriteManager::GetDisplayHeight();

	lUIRect cTextRect = cDrawRect;
	if (m_pKit->IsVisible())
	{
		cTextRect.x = m_pKit->GetRect().x + m_pKit->GetRect().w + 5;
		cTextRect.w -= cTextRect.x;

		m_pKit->Move(lUIPoint( 5,  GetRect().h / 2.0f));
		m_pKit->Render();
	}

	lUIPoint cOffset = lUICalcAlign(LUI_CENTER_CENTER, lUIPoint(cTextRect.w, cTextRect.h), lUIPoint(fStringWidth, fStringHeight));
  if ((m_cCurrentMessage.m_eType == MMT_REDCARD) || (m_cCurrentMessage.m_eType == MMT_YELLOWCARD) || (m_cCurrentMessage.m_eType == MMT_INJURY))
  {
    // redcard cycles red/white
    cFont->SetColour(uiColourToUseOpposite);
  } else if( m_cCurrentMessage.m_eType == MMT_CUSTOMMESSAGE )
  {
    cFont->SetColour( BLACK );
  } else
  {
    cFont->SetColour(GetInverseColour(uiColourIndexToUse));
  }
	cFont->SetAutoShadow(true);
	FDTextPrint(cFont.Get(), cTextRect.x + cOffset.x, cTextRect.y + cOffset.y, cString.str.CStr());
	cFont->SetAutoShadow(false);
}

//////////////////////////////////////////////////////////////////////////
// Used to render the substitution message.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::RenderSubstituteMessage()
{
	lAssert(m_cCurrentMessage.m_eType == MMT_SUBSTITUTE);

	lUIRect cDrawRect = WidgetToScreen(GetRect());
	const luint32_t uiColourIndex1 = m_cCurrentMessage.m_pClub->GetHomeKit().GetPrimaryColour() - 1;
	const luint32_t uiColourIndex2 = m_cCurrentMessage.m_pClub->GetHomeKit().GetSecondaryColour() - 1;
	luint32_t uiColour1 = GetKitColour(uiColourIndex1);
	luint32_t uiColour2 = GetKitColour(uiColourIndex2);

	const luint32_t uiColourIndexToUse = uiColourIndex2;
	const luint32_t uiColourToUse = uiColour2;
	const luint32_t uiColourToUseOpposite = uiColour1;
	const luint32_t uiColourIndexToUseOpposite = uiColourIndex1;

	// Initial state:
	// OFF: blah
	// ON: blah
	lUIRect cComingOffRect = cDrawRect;
	lUIRect cComingOnRect = cDrawRect;
	cComingOnRect.y += cComingOnRect.h;

	if (m_fTimeRemaining > 3.5f)
	{
		// Do nothing.
	}
	else if (m_fTimeRemaining > 1.5f)
	{
		// Animate.
		float fYDelta = (1.0f - ((m_fTimeRemaining - 1.5f) / 2.0f)) * cComingOffRect.h;
		cComingOffRect.y += fYDelta;
		cComingOnRect.y -= fYDelta;
	}
	else
	{
		lUIRect cSwap = cComingOffRect;
		cComingOffRect = cComingOnRect;
		cComingOnRect = cSwap;
	}


	lSpriteManager::DrawRect2(cComingOffRect.x, cComingOffRect.y, cComingOffRect.w, cComingOffRect.h, uiColourToUse);

	lSharedPtr<lFont> cFont = FDResourceManager::GetFont("font16", "font16");
	lUIText cString;

	// TODO: construct strings to render
	// TODO LOCALE
	lUIRect cComingOffTextRect = cComingOffRect;
	lUIRect cComingOnTextRect = cComingOnRect;
	if (m_pKit->IsVisible())
	{
		cComingOffTextRect.x = m_pKit->GetRect().x + m_pKit->GetRect().w + 5;
		cComingOnTextRect.x = cComingOffTextRect.x;

		cComingOffTextRect.w -= cComingOffTextRect.x;
		cComingOnTextRect.w -= cComingOnTextRect.x;
	}

	cFont->SetAutoShadow(true);

	//////////////////////////////////////////////////////////////////////////
	// First, draw the text for the player coming off.
	//////////////////////////////////////////////////////////////////////////
	CString cFormatString;
	float fStringWidth, fStringHeight;

	{
		cFormatString.Format("OFF: %s", m_cCurrentMessage.m_pPlayer->GetName().CStr());
		cString = lUIText(cFormatString.CStr());

		FDTextGetStringBound(cFont.Get(), fStringWidth, fStringHeight, cString.str.CStr());
		fStringWidth *= lSpriteManager::GetDisplayWidth();
		fStringHeight *= lSpriteManager::GetDisplayHeight();

		lUIPoint cOffsetComingOff = lUICalcAlign(LUI_CENTER_CENTER, lUIPoint(cComingOffTextRect.w, cComingOffTextRect.h), lUIPoint(fStringWidth, fStringHeight));
		cFont->SetColour(GetInverseColour(uiColourIndexToUse));
		FDTextPrint(cFont.Get(), cComingOffTextRect.x + cOffsetComingOff.x, cComingOffTextRect.y + cOffsetComingOff.y, cString.str.CStr());
	}

	//////////////////////////////////////////////////////////////////////////
	// Now, draw the rect and text for the player coming on.
	//////////////////////////////////////////////////////////////////////////
	{
		lSpriteManager::DrawRect2(cComingOnRect.x, cComingOnRect.y, cComingOnRect.w, cComingOnRect.h, uiColourToUseOpposite);
		cFormatString.Format("ON: %s", m_cCurrentMessage.m_pSub->GetName().CStr());
		cString = lUIText(cFormatString.CStr());

		FDTextGetStringBound(cFont.Get(), fStringWidth, fStringHeight, cString.str.CStr());
		fStringWidth *= lSpriteManager::GetDisplayWidth();
		fStringHeight *= lSpriteManager::GetDisplayHeight();

		lUIPoint cOffsetComingOn = lUICalcAlign(LUI_CENTER_CENTER, lUIPoint(cComingOnTextRect.w, cComingOnTextRect.h), lUIPoint(fStringWidth, fStringHeight));
		cFont->SetColour(GetInverseColour(uiColourIndexToUseOpposite));
		FDTextPrint(cFont.Get(), cComingOnTextRect.x + cOffsetComingOn.x, cComingOnTextRect.y + cOffsetComingOn.y, cString.str.CStr());
	}

	cFont->SetAutoShadow(false);

	//////////////////////////////////////////////////////////////////////////
	// Finally, draw the kit on top of everything else.
	//////////////////////////////////////////////////////////////////////////
	if (m_pKit->IsVisible())
	{
		m_pKit->Move(lUIPoint( 5,  cDrawRect.h));
		m_pKit->Render();
	}
}

//////////////////////////////////////////////////////////////////////////
// Used to update the state of the marquee.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::Tick(const float fTimeInSeconds)
{
	if ((m_bPause == true) && (m_fTimeRemaining <= 0.0f))
		return;

	if (m_fTimeRemaining <= 0.0f)
	{
		if (m_queue.Size() > 0)
		{
			m_cCurrentMessage = m_queue(0);
			m_fTimeRemaining = m_cCurrentMessage.m_fTime;
			m_queue.Remove(0);
		}
		else
		{
			return;
		}

		if (m_cCurrentMessage.m_eType != MMT_CUSTOMMESSAGE)
		{
			SetupKit();
			m_pKit->Show();
		}
	}

	if (m_fTimeRemaining > 0.0f)
	{
		m_fTimeRemaining -= fTimeInSeconds;

		if (m_fTimeRemaining <= 0.0f)
		{
			m_pKit->Hide();
			m_messageBar->Hide();
			SignalMessageComplete.Emit(this);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Starts a message for a goal score.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::StartGoalMessage(CClub *pScoringClub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_GOAL;
	cMessage.m_pClub = pScoringClub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

//////////////////////////////////////////////////////////////////////////
// Starts a message for an own goal.
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::StartOwnGoalMessage(CClub *pVictimClub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_OWN_GOAL;
	cMessage.m_pClub = pVictimClub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

//////////////////////////////////////////////////////////////////////////
// Starts a message for a yellow card
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::StartYellowCardMessage(CClub *pScoringClub, CString pPlayerName)
{
	QueuedMessage cMessage;
  cMessage.m_eType = MMT_YELLOWCARD;
	cMessage.m_pClub = pScoringClub;
  //cMessage.m_message.Format("YELLOW CARD - %s", pPlayerName);
  cMessage.m_message.Format("YELLOW CARD");
  cMessage.m_fTime = 3.0f;

	m_queue.AddElement(cMessage);
}

//////////////////////////////////////////////////////////////////////////
// Starts a message for a red card
//////////////////////////////////////////////////////////////////////////
void FDMarqueeWidget::StartRedCardMessage(CClub *pScoringClub, CString pPlayerName)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_REDCARD;
	cMessage.m_pClub = pScoringClub;
  //cMessage.m_message.Format("RED CARD - %s", pPlayerName);
  cMessage.m_message.Format("RED CARD");
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartPenaltyGoalMessage(CClub *pScoringClub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_PENALTYGOAL;
	cMessage.m_pClub = pScoringClub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartPenaltySaveMessage(CClub *pScoringClub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_PENALTYSAVE;
	cMessage.m_pClub = pScoringClub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartPenaltyMissMessage(CClub *pScoringClub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_PENALTYMISS;
	cMessage.m_pClub = pScoringClub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartInjuryMessage(CClub *pClub, CPlayer *pPlayer)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_INJURY;
	cMessage.m_pClub = pClub;
	cMessage.m_pPlayer = pPlayer;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartSubstitutionMessage(CClub *pClub, CPlayer *pPlayer, CPlayer *pSub)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_SUBSTITUTE;
	cMessage.m_pClub = pClub;
	cMessage.m_pPlayer = pPlayer;
	cMessage.m_pSub = pSub;
	cMessage.m_fTime = 5.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::StartCustomMessage(const char * message)
{
	QueuedMessage cMessage;
	cMessage.m_eType = MMT_CUSTOMMESSAGE;
	cMessage.m_message = message;
	cMessage.m_fTime = 2.0f;

	m_queue.AddElement(cMessage);
}

void FDMarqueeWidget::SetupKit()
{
	if (m_cCurrentMessage.m_pClub == NULL)
		return;

  m_pKit->GetSprite().Get()->SetFrame( m_cCurrentMessage.m_pClub->GetHomeKitID() );
}

void FDMarqueeWidget::SetPause(const bool bPause)
{
	m_bPause = bPause;
}

