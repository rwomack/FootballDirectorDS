//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : CSpinCtrl.h
// Description : CSpinCtrl class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CSPINCTRL_H_INCLUDED
#define EDITOR_UI_SUPPORT_CSPINCTRL_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>
#include <framework/lsigslot.h>

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CSpinCtrlT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T>
class CSpinCtrlT : public CWindowImpl<CSpinCtrlT<T>,CWindow,CControlWinTraits>
{
  enum { IDM_LABEL = 1000, IDM_EDIT, IDM_UPDOWN };
  CStatic      m_label;
  CUpDownCtrl  m_upDownCtrl;
  char         m_labelText[64];
  T            m_step;
  T            m_rangeLower, m_rangeUpper;
  bool         m_bRangeEnable;

public:
  T            m_value;
  CEdit        m_edit;

  DECLARE_WND_CLASS_EX(NULL, 0, COLOR_BTNFACE)
  BEGIN_MSG_MAP(CSpinCtrl)
    MESSAGE_HANDLER(WM_CREATE,    OnCreate)
    MESSAGE_HANDLER(WM_SIZE,      OnSize)
    COMMAND_ID_HANDLER( IDM_EDIT, OnEdit )
    NOTIFY_HANDLER(IDM_UPDOWN,  UDN_DELTAPOS, OnDeltaPos)
  END_MSG_MAP()

  Lemon::lSignal1<T> SignalValueChange;

  CSpinCtrlT() :
    m_step(1),
    m_value(0),
    m_rangeLower( -10000 ),
    m_rangeUpper( 10000 ),
    m_bRangeEnable(false)
  {
    m_labelText[0] = '\0';
  }

  T     GetValue() const         { return m_value; }
  void  SetStep(T s)             { m_step = s; }
  T     GetStep() const          { return m_step; }
  void  SetRangeEnable( bool b ) { m_bRangeEnable = b; }
  bool  GetRangeEnable()         { return m_bRangeEnable; }
  void  SetRangeLower(T l)       { SetRange( l, m_rangeUpper ); }
  void  SetRangeUpper(T u)       { SetRange( m_rangeLower, u ); }
  T     GetRangeLower() const    { return m_rangeLower; }
  T     GetRangeUpper() const    { return m_rangeUpper; }

  void  SetRange(T l, T u)
  {
    m_rangeLower = l;
    m_rangeUpper = u;

    if ( ClampValue() )
    {
      UpdateEditFromValue();
    }
  }

  void SetValue(T v)
  {
    m_value = v;
    ClampValue();
    UpdateEditFromValue();
  }


  void Disable()
  {
    m_label.ModifyStyle( 0, WS_DISABLED, 0 );
    m_edit.ModifyStyle( 0, WS_DISABLED, 0 );
    m_upDownCtrl.ModifyStyle( 0, WS_DISABLED, 0 );
    m_label.RedrawWindow();
    m_edit.RedrawWindow();
    m_upDownCtrl.RedrawWindow();
  }

  void Enable()
  {
    m_label.ModifyStyle( WS_DISABLED, 0, 0 );
    m_edit.ModifyStyle( WS_DISABLED, 0, 0 );
    m_upDownCtrl.ModifyStyle( WS_DISABLED, 0, 0 );
    m_label.RedrawWindow();
    m_edit.RedrawWindow();
    m_upDownCtrl.RedrawWindow();
  }



  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    m_label.Create( m_hWnd, NULL, "", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE, 0, IDM_LABEL );
    m_edit.Create( m_hWnd, NULL, "", WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE, WS_EX_CLIENTEDGE, IDM_EDIT );
    m_upDownCtrl.Create( m_hWnd, NULL, "", WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_ARROWKEYS, 0, IDM_UPDOWN );

    m_edit.SetFont( AtlGetDefaultGuiFont() );
    m_label.SetFont( AtlGetDefaultGuiFont() );

    GetWindowText( m_labelText, sizeof(m_labelText)-1 );
    m_labelText[sizeof(m_labelText)-1] = '\0';
    m_label.SetWindowText(m_labelText);

    UpdateEditFromValue();
    return 0;
  }

  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    CRect rc;
    GetClientRect(rc);

    if( m_labelText[0] != '\0' )
    {
      HDC hdc = ::GetDC(m_hWnd);
      CSize size;
      GetTextExtentPoint32( hdc, m_labelText, strlen(m_labelText), &size );
      ::ReleaseDC(NULL, hdc);

      CRect labelRc = rc;
      labelRc.right = labelRc.left + size.cx;
      labelRc.top += (rc.Height() - size.cy) / 2;
      m_label.SetWindowPos( 0, labelRc.left, labelRc.top, labelRc.Width(), labelRc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );

      rc.left = labelRc.right;
    }

    CRect nameRc = rc;
    m_edit.SetWindowPos( 0, nameRc.left, nameRc.top, nameRc.Width(), nameRc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );
    m_upDownCtrl.SetBuddy( m_edit );
    return 0;
  }

  LRESULT OnEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
  {
    if( wNotifyCode == EN_CHANGE )
    {
      if( m_edit.GetLineCount() > 1 )
      {
        // user pressed return

        // get value
        UpdateValueFromEdit();

	      // clamp to within range
	      ClampValue();

        // reformed the edit
        // UpdateEditFromValue() will fire a EN_CHANGE, but will not cause in infinite loop
        // since it doesn't append a newline onto the edit, so GetLineCount() will be <=1

        UpdateEditFromValue();

        // fire the signal

        SignalValueChange( m_value );
      }
    }
    return 0;
  }

  LRESULT OnDeltaPos(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
  {
    LPNMUPDOWN lpNmUpDown = (LPNMUPDOWN)pnmh;
    m_value += T(-lpNmUpDown->iDelta) * m_step;
    ClampValue();
    UpdateEditFromValue();

    // fire the signal
    SignalValueChange( m_value );

    return 0;
  }

  bool ClampValue()
  {
    if( !m_bRangeEnable ) return false;
    bool clampedFlag = false;

    if ( m_value < m_rangeLower )
    {
      m_value = m_rangeLower;
      clampedFlag = true;
    }

    if ( m_value > m_rangeUpper )
    {
      m_value = m_rangeUpper;
      clampedFlag = true;
    }

    return clampedFlag;
  }

  virtual void UpdateEditFromValue()
  {
    char buf[64];
    sprintf(buf,"%.3f",m_value);
    m_edit.SetWindowText(buf);
  }

  virtual void UpdateValueFromEdit()
  {
    char buf[1024];
    int len = m_edit.GetLine(0,buf,sizeof(buf)-1);
    buf[len] = NULL;
    m_value = static_cast<T>(atof( buf ));
  }

  void Refresh()
  {
    // get value
    UpdateValueFromEdit();

    // clamp to within range
    ClampValue();

    // reformed the edit
    // UpdateEditFromValue() will fire a EN_CHANGE, but will not cause in infinite loop
    // since it doesn't append a newline onto the edit, so GetLineCount() will be <=1
    UpdateEditFromValue();
  }
};

class CSpinCtrlFloat : public CSpinCtrlT<float>
{
public:
  virtual void UpdateEditFromValue()
  {
    char buf[64];
    sprintf(buf,"%.3f",m_value);
    m_edit.SetWindowText(buf);
  }

  virtual void UpdateValueFromEdit()
  {
    char buf[1024];
    int len = m_edit.GetLine(0,buf,sizeof(buf)-1);
    buf[len] = NULL;
    m_value = atof( buf );
  }
};

class CSpinCtrlInt : public CSpinCtrlT<int>
{
public:
  virtual void UpdateEditFromValue()
  {
    char buf[64];
    sprintf(buf,"%d",m_value);
    m_edit.SetWindowText(buf);
  }

  virtual void UpdateValueFromEdit()
  {
    char buf[1024];
    int len = m_edit.GetLine(0,buf,sizeof(buf)-1);
    buf[len] = NULL;
    m_value = atoi( buf );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CSpinCtrlDoubleT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T,class CTRL>
class CSpinCtrlDoubleT : public CWindowImpl<CSpinCtrlDoubleT<T,CTRL>,CWindow,CControlWinTraits>, public Lemon::lHasSlots<>
{
  CStatic     m_label;
  CTRL        m_spinX;
  CTRL        m_spinY;

public:

  Lemon::lSignal2<T,T> SignalValueChange;

  DECLARE_WND_CLASS_EX(NULL, 0, COLOR_BTNFACE)
  BEGIN_MSG_MAP(CPropertyBoxGeomEditor)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SIZE,   OnSize)
  END_MSG_MAP()

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    m_label.Create( m_hWnd, NULL, "", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE, 0 );
    m_label.SetFont( AtlGetDefaultGuiFont() );
    m_spinX.Create( m_hWnd, NULL, "X", WS_VISIBLE | WS_CHILD, 0 );
    m_spinY.Create( m_hWnd, NULL, "Y", WS_VISIBLE | WS_CHILD, 0 );

    m_spinX.SignalValueChange.Connect( this, &CSpinCtrlDoubleT::OnValueChange );
    m_spinY.SignalValueChange.Connect( this, &CSpinCtrlDoubleT::OnValueChange );

    return 0;
  }

  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    CRect rc;
    GetClientRect(rc);

    char labelText[64];

    GetWindowText( labelText, sizeof(labelText)-1 );
    labelText[sizeof(labelText)-1] = '\0';
    m_label.SetWindowText(labelText);

    if( labelText[0] != '\0' )
    {
      HDC hdc = ::GetDC(m_hWnd);
      CSize size;
      GetTextExtentPoint32( hdc, labelText, strlen(labelText), &size );
      ::ReleaseDC(NULL, hdc);

      CRect labelRc = rc;
      labelRc.right = labelRc.left + size.cx;
      labelRc.top += (rc.Height() - size.cy) / 2;
      m_label.SetWindowPos( 0, labelRc.left, labelRc.top, labelRc.Width(), labelRc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );

      rc.left = labelRc.right;
    }

    int x = 0;
    int width = rc.Width()/2;

    m_spinX.SetWindowPos( 0, rc.left+x, rc.top, width, rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );
    x += width;
    m_spinY.SetWindowPos( 0, rc.left+x, rc.top, width, rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );

    return 0;
  }

  void OnValueChange(T val)
  {
    SignalValueChange( m_spinX.GetValue(), m_spinY.GetValue() );
  }

  void SetValue( T x, T y )
  {
    m_spinX.SetValue( x );
    m_spinY.SetValue( y );
  }

  void GetValue( T &x, T &y )
  {
    x = m_spinX.GetValue();
    y = m_spinY.GetValue();
  }


  void Disable()
  {
    m_label.ModifyStyle( 0, WS_DISABLED, 0 );
    m_label.RedrawWindow();
    m_spinX.Disable();
    m_spinY.Disable();
  }

  void Enable()
  {
    m_label.ModifyStyle( WS_DISABLED, 0, 0 );
    m_label.RedrawWindow();
    m_spinX.Enable();
    m_spinY.Enable();
  }
};

typedef CSpinCtrlDoubleT<float,CSpinCtrlFloat> CSpinCtrlDoubleFloat;
typedef CSpinCtrlDoubleT<int,CSpinCtrlInt>     CSpinCtrlDoubleInt;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CSpinCtrlTripleT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T,class CTRL>
class CSpinCtrlTripleT : public CWindowImpl<CSpinCtrlTripleT<T,CTRL>,CWindow,CControlWinTraits>, public Lemon::lHasSlots<>
{
  CStatic     m_label;
  CTRL        m_spinX;
  CTRL        m_spinY;
  CTRL        m_spinZ;

public:

  Lemon::lSignal3<T,T,T> SignalValueChange;

  DECLARE_WND_CLASS_EX(NULL, 0, COLOR_BTNFACE)
  BEGIN_MSG_MAP(CPropertyBoxGeomEditor)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SIZE,   OnSize)
  END_MSG_MAP()

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    m_label.Create( m_hWnd, NULL, "", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE, 0 );
    m_label.SetFont( AtlGetDefaultGuiFont() );
    m_spinX.Create( m_hWnd, NULL, "X", WS_VISIBLE | WS_CHILD, 0 );
    m_spinY.Create( m_hWnd, NULL, "Y", WS_VISIBLE | WS_CHILD, 0 );
    m_spinZ.Create( m_hWnd, NULL, "Z", WS_VISIBLE | WS_CHILD, 0 );

    m_spinX.SignalValueChange.Connect( this, &CSpinCtrlTripleT::OnValueChange );
    m_spinY.SignalValueChange.Connect( this, &CSpinCtrlTripleT::OnValueChange );
    m_spinZ.SignalValueChange.Connect( this, &CSpinCtrlTripleT::OnValueChange );

    return 0;
  }

  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    CRect rc;
    GetClientRect(rc);

    char labelText[64];

    GetWindowText( labelText, sizeof(labelText)-1 );
    labelText[sizeof(labelText)-1] = '\0';
    m_label.SetWindowText(labelText);

    if( labelText[0] != '\0' )
    {
      HDC hdc = ::GetDC(m_hWnd);
      CSize size;
      GetTextExtentPoint32( hdc, labelText, strlen(labelText), &size );
      ::ReleaseDC(NULL, hdc);

      CRect labelRc = rc;
      labelRc.right = labelRc.left + size.cx;
      labelRc.top += (rc.Height() - size.cy) / 2;
      m_label.SetWindowPos( 0, labelRc.left, labelRc.top, labelRc.Width(), labelRc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );

      rc.left = labelRc.right;
    }

    int x = 0;
    int width = rc.Width()/3;

    m_spinX.SetWindowPos( 0, rc.left+x, rc.top, width, rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );
    x += width;
    m_spinY.SetWindowPos( 0, rc.left+x, rc.top, width, rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );
    x += width;
    m_spinZ.SetWindowPos( 0, rc.left+x, rc.top, width, rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE );

    return 0;
  }

  void OnValueChange(T val)
  {
    SignalValueChange( m_spinX.GetValue(), m_spinY.GetValue(), m_spinZ.GetValue() );
  }

  void SetValue( T x, T y, T z )
  {
    m_spinX.SetValue( x );
    m_spinY.SetValue( y );
    m_spinZ.SetValue( z );
  }

  void GetValue( T &x, T &y, T &z )
  {
    x = m_spinX.GetValue();
    y = m_spinY.GetValue();
    z = m_spinZ.GetValue();
  }


  void Disable()
  {
    m_label.ModifyStyle( 0, WS_DISABLED, 0 );
    m_label.RedrawWindow();
    m_spinX.Disable();
    m_spinY.Disable();
    m_spinZ.Disable();
  }

  void Enable()
  {
    m_label.ModifyStyle( WS_DISABLED, 0, 0 );
    m_label.RedrawWindow();
    m_spinX.Enable();
    m_spinY.Enable();
    m_spinZ.Enable();
  }
};

typedef CSpinCtrlTripleT<float,CSpinCtrlFloat> CSpinCtrlTripleFloat;
typedef CSpinCtrlTripleT<int,CSpinCtrlInt>     CSpinCtrlTripleInt;

}
#endif
