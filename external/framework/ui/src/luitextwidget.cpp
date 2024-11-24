//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luitextwidget.cpp
// Description : lUITextWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luitextwidget.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUITextWidget, lGUID(0,0,0,0), 0 );

static lString SplitText( lFont* font, lString text, int maxWidth )
{
  lString output = lString("");
  int stringWidth = 0;
  int stringHeight = 0;

  font->GetStringBound( stringWidth, stringHeight, text );

  if( stringWidth < maxWidth )
  {
    return text;
  }
  else
  {
    lString line = lString("");
    lString word = lString("");
    for( int i = 0; i < text.Size(); i++ )
    {
      if( text(i) == ' ' )
      {
        //Check line length
        font->GetStringBound( stringWidth, stringHeight, line );
        int lineWidth = stringWidth;
        font->GetStringBound( stringWidth, stringHeight, word );

        if( lineWidth + stringWidth > maxWidth )
        {
          output += line + lString("\n");
          line = lString("");

		  // Skip leading spaces on the next line.
		  do
		  {
			  i++;
		  } while ((i < text.Size()) && (text(i) == ' '));
		  i--;
        }

        line += word + lString(" ");
        word = lString("");
      }
      else if( text(i) == '\n' )
      {
        //handle new line
        //Check line length
        font->GetStringBound(stringWidth, stringHeight, line.CStr());
        float lineWidth = stringWidth;
        font->GetStringBound(stringWidth, stringHeight, word.CStr());

        if( lineWidth + stringWidth > maxWidth )
        {
		  // Current word spills over, so insert a newline, the current word, and then another newline.
          output += line + lString("\n") + word + lString("\n");
          line = lString("");

		  // Skip leading spaces on the next line.
		  do
		  {
			  i++;
		  } while ((i < text.Size()) && (text(i) == ' '));
		  i--;
        }
		else
			output += line + word + lString("\n");

        line = lString("");
		word = lString("");
      }
      else
      {
        //add letter
        word = word + lString(text(i));
      }
    }
    //Add last word
    font->GetStringBound( stringWidth, stringHeight, line );
    int lineWidth = stringWidth;
    font->GetStringBound( stringWidth, stringHeight, word );

    if( lineWidth + stringWidth > maxWidth )
    {
      line = line + lString("\n");
    }
    output = output + line;
    output = output + word;
  }
  return output;
}


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUITextWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lUITextWidget::ClassInitialiser()
{
}


lUITextWidget::lUITextWidget()
  : m_colour( L_ARGB(255,255,255,255) ),
    m_align( LUI_CENTER_CENTER ),
    m_bShadow( false ),
    m_shadowColour( L_ARGB(255,0,0,0) )
{
}

lUITextWidget::lUITextWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIText & text,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_text( text ),
    m_colour( L_ARGB(255,255,255,255) ),
    m_align( LUI_CENTER_CENTER ),
    m_bShadow( false ),
    m_shadowColour( L_ARGB(255,0,0,0) )
{
}

lUITextWidget::~lUITextWidget()
{
}

void lUITextWidget::Tick(float delta)
{
}

lUIPoint lUITextWidget::GetIdealSize() const
{
  lSharedPtr<lFont> font = GetFont();

  float maxWidth = PixelSizeX( lUICoordPercent(100) );
  lString text = SplitText( font.Get(), m_text.str, int(maxWidth) );

  int width;
  int height;
  font->GetStringBound( width, height, text );

  return lUIPoint( width, height );
}

void lUITextWidget::Render()
{
  lSharedPtr<lFont> font = GetFont();

  lUIRect rect = WidgetToScreen( GetRect() );

  float maxWidth = PixelSizeX( lUICoord(rect.w) );
  lString text = SplitText( font.Get(), m_text.str, int(maxWidth) );

  font->SetColour( m_colour );

  lUIAlignAxis xAlign = lUIAlignAxis( (m_align>>4) & 0xF );
  lUIAlignAxis yAlign = lUIAlignAxis( m_align & 0xF );

  // count lines

  int numLines = 1;
  const char * line = text.CStr();

  while( *line )
  {
    if( *line == '\n' )
    {
      numLines++;
    }

    line++;
  }

  float yOffset = lUICalcAlignAxis( yAlign, rect.h, font->DefaultPixelHeight() * numLines );

  // process each line.

  lUIPoint pos = lUIPoint( rect.x, rect.y + yOffset );

  char * s = const_cast<char *>(text.CStr()); // HACK
  line = s;

  if( m_bShadow) font->SetShadow( true, m_shadowColour );

  while( 1 )
  {
    if( *s == '\n' || *s == '\0' )
    {
      bool bDone = false;
      if( *s == '\0' ) bDone = true;

      char tmp = *s;
      *s = '\0';

      int width;
      int height;
      font->GetStringBound( width, height, line );

      float xOffset = lUICalcAlignAxis( xAlign, rect.w, width );

      font->Print( int(pos.x+xOffset), int(pos.y), line );
      pos.y += font->DefaultPixelHeight();
      pos.x = rect.x;

      if( bDone ) break;

      *s = tmp;
      s++;
      line = s;
    }
    else
    {
      s++;
    }
  }

  if( m_bShadow) font->SetShadow( false, m_shadowColour );
}

void lUITextWidget::SetText( const lUIText & text )
{
  m_text = text;
}

void lUITextWidget::SetText( const lUIText & text, bool bClip )
{

  lString str = text.str;

  if(bClip)
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    lSharedPtr<lFont> font = GetFont();

    int Width = 0;
	  int Height = 0;

	  font->GetStringBound( Width, Height, str.CStr() );

	  //lLogFmt("Text Width: %d\nTable width: %d\n", (int) Width, (int) m_columnWidth(col) );


	  if( Width > rect .w)
	  {
	    bool bDone = false;

	    while( !bDone )
	    {
	      font->GetStringBound( Width, Height, str.CStr() );

	      if( Width > rect .w - 5 )
	      {
	        str = str.Left( str.Size()-1 );
	        //lLogFmt("%s\n", str.CStr() );
	        if( str.Size()  == 0 )
	          bDone = true;
	      }
	      else
	      {
	        bDone = true;
        }
	    }
	  }
  }

  m_text = lUIText( str .CStr() );
}

const lUIText & lUITextWidget::GetText()
{
  return m_text;
}

void lUITextWidget::SetTextColour( luint32_t colour )
{
  m_colour = colour;
}

luint32_t lUITextWidget::GetTextColour()
{
  return m_colour;
}

void lUITextWidget::SetAlign( lUIAlign align )
{
  m_align = align;
}

lUIAlign lUITextWidget::GetAlign() const
{
  return m_align;
}

void lUITextWidget::SetShadow( bool bShadow, luint32_t colour )
{
  m_bShadow = bShadow;
  m_shadowColour = colour;
}
