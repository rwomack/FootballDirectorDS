//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : fdsliderwidget.cpp
// Description : FDSliderWidget implementation
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdsliderwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>
#include <cstring.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

static CString GetForeignStr(const int _PoundsAmount)
{
	CString Temp;
	CString Newstr;
	Temp.Format("%d", _PoundsAmount);
	Temp.MakeReverse();
	int Deci = Temp.Find(".");
	int Total = 0;
	int Lenth = Temp.GetLength();
	for (int Count = 0; Count < Lenth; Count ++)
	{
		if (Count >= Deci)
		{
			if (Total == 3)
			{
				Total = 1;
				Newstr += ",";
			}
			else
			{
				Total++;
			}
		}
		Newstr += Temp.GetAt(Count);
	}
	Newstr.MakeReverse();

	// If a ridiculously large amount truncate and show as millions
	/*if (Newstr.GetLength() > 7 )
	{
		/*if (Newstr.Right(7) != (CString)"000,000")
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 4) + "t";
		}
		else
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 8) + "m";
		}
	}*/
	CString szNumBuff;

	szNumBuff.Format("%s%s", "£", Newstr);

	if (_PoundsAmount < 0)
	{
    szNumBuff = CString("-") + szNumBuff;
	}
	return szNumBuff;
}


LUI_IMPLEMENT_RTTI( FDSliderWidget, lGUID(0,0,0,0), 0 );

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// FDSliderWidget
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void FDSliderWidget::ClassInitialiser()
{
}

FDSliderWidget::FDSliderWidget()
  : m_state( 0 ),
    m_value( 0.0f ),
    m_valuescale( 1.0f ),
    m_valueoffset( 0.0f ),
    m_sliderflags( 0 ),
    m_increment( 0.01f ),
    m_sliderOffset(0)
{
}

FDSliderWidget::FDSliderWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                luint32_t sliderFlags, const lUIText & label,
                                const lUIWidgetPosition anchor,
                                const lUIWidgetPosition pos,
                                const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_label( label ),
    m_state( 0 ),
    m_value( 0.5f ),
    m_valuescale( 1.0f ),
    m_valueoffset( 0.0f ),
    m_sliderflags( sliderFlags ),
    m_increment( 0.01f ),
    m_sliderOffset(0)
{
  m_slider = FDResourceManager::GetSprite("uiicon","slider_bar");
  m_button = FDResourceManager::GetSprite("uiicon","slider_bar_button");
}

FDSliderWidget::~FDSliderWidget()
{
}

lUIPoint FDSliderWidget::GetIdealSize() const
{
  return lUIPoint( m_slider->GetWidth(), m_slider->GetHeight() );
}

void FDSliderWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );

  m_slider->SetPosition( rect.x + m_sliderOffset, rect.y );

  lUIRect sliderRect = lUIRect(m_slider->GetX(), m_slider->GetY(), m_slider->GetWidth(), m_slider->GetHeight());
  lUIRect sliderFillRect = sliderRect;
  sliderFillRect.Shrink( 12, 2 );
  lSpriteManager::DrawRect2( luint32_t(sliderFillRect.x), luint32_t(sliderFillRect.y), luint32_t(sliderFillRect.w), luint32_t(sliderFillRect.h), L_ARGB(255,13,153,215) );

  m_button->SetPosition( (sliderFillRect.x + ( sliderFillRect.w * m_value )) - (m_button->GetWidth() * 0.5f),
                         sliderRect.y + (sliderRect.h - m_button->GetHeight()) * 0.5f );

  lSharedPtr<lFont> font = GetFont();
  luint32_t colour = L_ARGB(255,255,255,255);

  if( m_state & SLIDER_DOWN )
  {
    colour = L_ARGB(255,128,128,128);
  }

  if( IsEnabled() )
  {
    m_slider->SetColour( colour );
    m_button->SetColour( colour );
    font->SetColour( L_ARGB(255,255,255,255) );
  }
  else
  {
    m_slider->SetColour( L_ARGB(255,80,80,80));
    m_button->SetColour( L_ARGB(255,80,80,80));
    font->SetColour( L_ARGB(255,100,100,100) );
  }

  m_slider->Render();
  m_button->Render();



  // HACK: label is outside of our rect.

  int width;
  int height;

  font->GetStringBound( width, height, m_label.str );
  //lUIPoint labelOffset = lUICalcAlign( LUI_CENTER_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );  // label in centre
  lUIPoint labelOffset = lUICalcAlign( LUI_LEFT_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );  // label in centre

  labelOffset.y = -(height+1);

  font->Print( int(rect.x+labelOffset.x), int(rect.y+labelOffset.y), m_label.str );

  if( m_valuescale != 1.0f )
  {
    lString valuestr;
    if( m_sliderflags & SLIDER_FLAG_PERCENTAGE )
    {
      int val = GetValue();
      valuestr.Format("%dpct", val);
    }
    else if( m_sliderflags & SLIDER_FLAG_POUND )
    {
      int val = GetValue();
      //valuestr.Format("£%d", int(GetValue()));
      valuestr = GetForeignStr( val);
    }
    else if( m_sliderflags & SLIDER_FLAG_X )
    {
      int val = GetValue();
      valuestr.Format("x%d", val);
    }
    else
    {
      int val = GetValue();
      valuestr.Format("%d", val);
    }
    font->GetStringBound( width, height, valuestr.CStr() );
    //lUIPoint labelOffset = lUICalcAlign( LUI_CENTER_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );  // label in centre
    labelOffset = lUICalcAlign( LUI_RIGHT_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );  // label in centre

    labelOffset.y = -(height+1);

    font->Print( int(rect.x+labelOffset.x), int(rect.y+labelOffset.y), valuestr.CStr() );
  }
}

void FDSliderWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  SuperClass::OnMouse( p, mouseDownFlags, mouseUpFlags );

  if (IsEnabled() == false)
	  return;

  if( m_state & SLIDER_DOWN )
  {
    lUIRect sliderRect = lUIRect(m_slider->GetX(), m_slider->GetY(), m_slider->GetWidth(), m_slider->GetHeight());
    lUIRect sliderFillRect = sliderRect;
    sliderFillRect.Shrink( 12, 2 );
    sliderFillRect.x -= GetRect().x;
    sliderFillRect.y -= GetRect().y;

    if (p.x < sliderFillRect.x)
    {
      m_value = 0.0f;
    }
    else if (p.x > (sliderFillRect.x + sliderFillRect.w))
    {
      m_value = 1.0f;
    }
    else
    {
      if( m_valuescale != 0.0f )
      {
        float t = (p.x - sliderFillRect.x) / sliderFillRect.w;
        t = (float) ( ( lmRound ( t / (m_increment / m_valuescale) ) * (m_increment / m_valuescale) ) ) ;
        m_value = t;
      }
    }

    if( m_value < 0.0f ) m_value = 0.0f;
    if( m_value > 1.0f ) m_value = 1.0f;

    SignalSliderChange( this, GetValue() );
  }
}

void FDSliderWidget::OnMouseOver()
{
  SuperClass::OnMouseOver();

  if (IsEnabled() == false)
	  return;

  m_state |= SLIDER_HOT;
}

void FDSliderWidget::OnMouseLeave()
{
  SuperClass::OnMouseLeave();

  if (IsEnabled() == false)
	  return;

  m_state &= ~SLIDER_HOT;
}

bool FDSliderWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);

  if (IsEnabled() == false)
	  return true;

  lUIRect sliderRect = lUIRect(m_slider->GetX(), m_slider->GetY(), m_slider->GetWidth(), m_slider->GetHeight());
  lUIRect sliderFillRect = sliderRect;
  sliderFillRect.Shrink( 12, 2 );
  sliderFillRect.x -= GetRect().x;
  sliderFillRect.y -= GetRect().y;

  if (pos.InRect(sliderFillRect) == true)
  {
    CaptureMouse();
    m_state |= SLIDER_DOWN;
  }

  return true;
}

bool FDSliderWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);

  if (IsEnabled() == false)
	  return true;

  if (m_state & SLIDER_DOWN)
  {
    ReleaseMouse();
    m_state &= ~SLIDER_DOWN;
  }
  else
  {
    lUIPoint screenPos = WidgetClientToScreen( pos );

    lUIRect sliderRect = lUIRect(m_slider->GetX(), m_slider->GetY(), m_slider->GetWidth(), m_slider->GetHeight());
    lUIRect sliderFillRect = sliderRect;
    sliderFillRect.Shrink( 12, 2 );
    sliderFillRect.x -= GetRect().x;
    sliderFillRect.y -= GetRect().y;

    if( m_valuescale != 0.0f )
    {
      if( pos.x < sliderFillRect.x )
      {
        m_value -= (m_increment / m_valuescale);
      }
      else if( pos.x > sliderFillRect.x + sliderFillRect.w)
      {
        m_value += (m_increment / m_valuescale);
      }
    }

    if( m_value < 0.0f ) { m_value = 0.0f; }
    if( m_value > 1.0f ) { m_value = 1.0f; }
  }

  FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
  SignalSliderChange( this, GetValue() );

  return true;
}

void    FDSliderWidget::SetValuePct( float val )
{
  m_value = val;

  if( m_value < 0.0f ) m_value = 0.0f;
  if( m_value > 1.0f ) m_value = 1.0f;

  SignalSliderChange( this, GetValue() );
}

float   FDSliderWidget::GetValuePct()
{
  return m_value;
}

void             FDSliderWidget::SetValueRange( float min, float max )
{
  lLogFmt("SetValueRange: %.2f -> %.2f\n", min, max);
  m_valueoffset = min;
  m_valuescale  = max - min;

  lLogFmt(" offset: %.2f, scale: %.2f\n", m_valueoffset, m_valuescale);

  // increase increment based on min-max
  //m_increment = m_valuescale / 20;
  if( m_valuescale > 1.0f )
    m_increment = 1.0f;
  if( m_valuescale > 100.0f )
    m_increment = 50.0f;
  if( m_valuescale > 1000.0f )
    m_increment = 250.0f;
  if( m_valuescale > 10000.0f )
    m_increment = 250.0f;
  if( m_valuescale > 100000.0f )
    m_increment = 2500.0f;
  if( m_valuescale > 1000000.0f)
    m_increment = 25000.0f;
  if( m_valuescale > 10000000.0f)
    m_increment = 250000.0f;
  if( m_valuescale > 100000000.0f)
    m_increment = 2500000.0f;
}

void             FDSliderWidget::SetValueScale( float scale )
{
  m_valuescale = scale;
}

float            FDSliderWidget::GetValueScale()
{
  return m_valuescale;
}

void             FDSliderWidget::SetValueInc( float value )
{
  m_increment = value;
}

float            FDSliderWidget::GetValueInc()
{
  return m_increment;
}


void             FDSliderWidget::SetValueOffset( float offset )
{
  m_valueoffset = offset;
}

float            FDSliderWidget::GetValueOffset()
{
  return m_valueoffset;
}

void             FDSliderWidget::SetValue( float val, bool bDontSignal)
{
  if( m_valuescale == 0.0f ) m_value = 0.0f;
  else  m_value = (val-m_valueoffset) / m_valuescale;

  if( m_value < 0.0f ) m_value = 0.0f;
  if( m_value > 1.0f ) m_value = 1.0f;

  if( !bDontSignal )
    SignalSliderChange( this, GetValue() );
}

float            FDSliderWidget::GetValue()
{
  float value = ((m_value * m_valuescale) + m_valueoffset);

  // round to nearest increment
  if( m_increment > 1.0f )
  {
    value = int ( value ); // round up

    int remainder = int( value ) % int(m_increment);
    value -= (float) remainder;
    if( remainder > m_increment * 0.5f )
      value += m_increment;

    //lLogFmt("%d, %f - remainder = %d\n", (int) value, value, remainder);
  }

  // limit to min
  if( value < m_valueoffset ) value = m_valueoffset;
  //if( value > (m_valueoffset+m_valuescale) ) value = (m_valueoffset+m_valuescale);

  return value;
}

void             FDSliderWidget::SetLabel( lUIText text )
{
  m_label = text;
}

void             FDSliderWidget::SetTextToUpper()
{
  m_label.str.ToUpper();
}

lUIText          FDSliderWidget::GetLabel()
{
  return m_label;
}

void FDSliderWidget::SetSliderSprite( lSharedPtr<lSprite> slider)
{
  m_slider = slider;
}

luint32_t FDSliderWidget::GetSliderFlags()
{
  return m_sliderflags;
}

void FDSliderWidget::SetSliderFlags( luint32_t flag )
{
  m_sliderflags = flag;
}

void FDSliderWidget::SetSliderOffset( int value )
{
  m_sliderOffset = value;
}
