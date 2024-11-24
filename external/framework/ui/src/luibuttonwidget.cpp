//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luibuttonwidget.cpp
// Description : lUIButtonWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luibuttonwidget.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUIButtonWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIButtonWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lUIButtonWidget::ClassInitialiser()
{
}

lUIButtonWidget::lUIButtonWidget()
  : m_state(0),
    m_buttonFlags(0),
    m_textAlign( LUI_CENTER_CENTER )
{
}

lUIButtonWidget::lUIButtonWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                  const lUIText & text,
                                  luint32_t buttonFlags,
                                  const lUIWidgetPosition anchor,
                                  const lUIWidgetPosition pos,
                                  const lUIWidgetSize size )
  : lUIControlWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_state( 0 ),
    m_buttonFlags( buttonFlags ),
    m_textAlign( LUI_CENTER_CENTER ),
    m_text( text )
{
}

lUIButtonWidget::~lUIButtonWidget()
{
}

lUIPoint lUIButtonWidget::GetIdealSize() const
{
  int width;
  int height;

  GetFont()->GetStringBound( width, height, m_text.str );

  return lUIPoint( width, height );
}

const lUIText & lUIButtonWidget::GetText()
{
  return m_text;
}

lUIAlign lUIButtonWidget::GetTextAlign()
{
  return m_textAlign;
}

void lUIButtonWidget::SetTextAlign( lUIAlign align )
{
  m_textAlign = align;
}

void lUIButtonWidget::SetText( const lUIText & t )
{
  m_text = t;
}

luint32_t lUIButtonWidget::GetState()
{
  return m_state;
}

luint32_t lUIButtonWidget::GetButtonFlags()
{
  return m_buttonFlags;
}

void lUIButtonWidget::SetButtonFlags( luint32_t flags )
{
  m_buttonFlags = flags;
}

bool lUIButtonWidget::IsChecked()
{
  return (GetState() & BUTTON_STATE_CHECKED);
}

bool lUIButtonWidget::IsDown()
{
  return (GetState() & BUTTON_STATE_DOWN);
}

bool lUIButtonWidget::IsClicked()
{
  return (GetState() & BUTTON_STATE_CLICKED);
}

bool lUIButtonWidget::IsHover()
{
  return (GetState() & BUTTON_STATE_HOVER);
}

bool lUIButtonWidget::IsActive()
{
  return (GetState() & BUTTON_STATE_ACTIVE);
}

void lUIButtonWidget::Render()
{
  lSharedPtr<lFont> font = GetFont();
  lString           text = m_text.str;
  lUIRect           rect = WidgetToScreen( GetRect() );

  lUIPoint offset(0,0);

  int width;
  int height;
  font->GetStringBound( width, height, text );

  offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );

  if( IsChecked() )
  {
    font->SetColour( L_ARGB(255,0,255,0) );
  }
  else if( IsDown() )
  {
    font->SetColour( L_ARGB(255,0,0,255) );
  }
  else
  {
    font->SetColour( L_ARGB(255,255,255,255) );
  }

  font->Print( int(rect.x+offset.x),int(rect.y+offset.y), text );
}

void lUIButtonWidget::OnMouseOver()
{
  SuperClass::OnMouseOver();
  m_state |= BUTTON_STATE_HOVER;
}

void lUIButtonWidget::OnMouseLeave()
{
  SuperClass::OnMouseLeave();
  m_state &= ~BUTTON_STATE_HOVER;
}

bool lUIButtonWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);

  m_state |= BUTTON_STATE_DOWN;
  CaptureMouse();

  if( !( m_buttonFlags & BUTTON_FLAG_STYLUS_ONECLICK ) && !(m_state & BUTTON_STATE_ACTIVE) )
  {
    m_state |= BUTTON_STATE_DEBOUNCE;
  }

  SetFocus();

  return true;
}

void lUIButtonWidget::OnGainFocus()
{
  m_state |= BUTTON_STATE_ACTIVE;

  SignalControlHot( this );
}

void lUIButtonWidget::OnLoseFocus()
{
  m_state &= ~BUTTON_STATE_ACTIVE;
  m_state &= ~BUTTON_STATE_DEBOUNCE;
}

bool lUIButtonWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);

  m_state &= ~BUTTON_STATE_DOWN;
  ReleaseMouse();

  if( m_state & BUTTON_STATE_DEBOUNCE )
  {
    m_state &= ~BUTTON_STATE_DEBOUNCE;
    return false;
  }

  if( m_state & BUTTON_STATE_HOVER )
  {
    if( m_buttonFlags & BUTTON_FLAG_CHECKBOX )
    {
      if( m_state & BUTTON_STATE_CHECKED ) m_state &= ~BUTTON_STATE_CHECKED;
      else m_state |= BUTTON_STATE_CHECKED;
    }

    m_state |= BUTTON_STATE_CLICKED;

    SignalControlCommand( this );

    m_state &= ~BUTTON_STATE_CLICKED;
  }

  return true;
}


