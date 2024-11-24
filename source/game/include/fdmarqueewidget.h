//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmarqueewidget.h
// Description : A flashy widget used to display special messages like goal scores.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDMARQUEEWIDGET_H_INCLUDED
#define FDMARQUEEWIDGET_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <framework/lsprite.h>
#include <cstring.h>

class CClub;
class CPlayer;

BEGIN_L_NAMESPACE

class lUISpriteWidget;

class FDMarqueeWidget : public lUIWidget, public lHasSlots<>
{
	LUI_RTTI( FDMarqueeWidget, lUIWidget );
public:
	static void ClassInitialiser();

	FDMarqueeWidget();
	FDMarqueeWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
             const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
             const lUIWidgetPosition pos = LUI_POS_DEFAULT,
             const lUIWidgetSize size = LUI_SIZE_DEFAULT);

	virtual ~FDMarqueeWidget();

	virtual void Render();
	virtual void Tick(float fTimeInSeconds);

	void SetPause(const bool bPause);

	void StartGoalMessage(CClub *pScoringClub);
	void StartOwnGoalMessage(CClub *pVictimClub);
  void StartRedCardMessage(CClub *pVictimClub, CString pPlayerName);
  void StartYellowCardMessage(CClub *pVictimClub, CString pPlayerName);

  void StartPenaltyGoalMessage(CClub *pScoringClub);
  void StartPenaltySaveMessage(CClub *pScoringClub);
  void StartPenaltyMissMessage(CClub *pScoringClub);

  void StartCustomMessage(const char * message);

	void StartInjuryMessage(CClub *pClub, CPlayer *pPlayer);
	void StartSubstitutionMessage(CClub *pClub, CPlayer *pPlayer, CPlayer *pSub);

	lSignal1<FDMarqueeWidget *> SignalMessageComplete;
private:
	void Init();
	void SetupKit();

	void RenderSingleBarMessage();
	void RenderSubstituteMessage();

	enum MarqueeMessageType
	{
		MMT_GOAL,
		MMT_OWN_GOAL,
		MMT_REDCARD,
    MMT_YELLOWCARD,
		MMT_PENALTYGOAL,
		MMT_PENALTYSAVE,
		MMT_PENALTYMISS,
		MMT_INJURY,
		MMT_SUBSTITUTE,
		MMT_CUSTOMMESSAGE,
		MMT_INVALID
	};

	struct QueuedMessage
	{
    QueuedMessage() : m_eType(MMT_INVALID), m_pClub(NULL), m_pPlayer(NULL), m_pSub(NULL), m_fTime(0.0f) {};

		MarqueeMessageType m_eType;
		CClub *m_pClub;
		CPlayer *m_pPlayer;
		CPlayer *m_pSub;
    CString m_message;
		float m_fTime;
	};

	lArray<QueuedMessage> m_queue;
	QueuedMessage m_cCurrentMessage;
	lUISpriteWidget *m_pKit;
	lUISpriteWidget *m_messageBar;
	float m_fTimeRemaining;
	bool m_bPause;
};

END_L_NAMESPACE

#endif // #ifndef FDMARQUEEWIDGET_H_INCLUDED
