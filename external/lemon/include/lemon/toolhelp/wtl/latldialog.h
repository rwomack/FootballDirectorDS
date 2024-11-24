//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : latldialog.h
// Description : Lemon ATL inline template dialog routines
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef WTL_LATLDIALOG_H_INCLUDED
#define WTL_LATLDIALOG_H_INCLUDED

// Note you should include the atl headers, or latl.h before this file.

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Additional ATL classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

namespace ATL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CDialogIndirectImpl
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
/*
 Use as CDialogImpl.
 CDialogIndirectImpl is based on the ATL class CDialogImpl except that it uses
 the function GetDlgTemplate() to create the dialog rather than the IDD enum.
 Designed the be used in conjuction with CDialogTemplate.

 Usage:
 class CMyDialog : public CDialogIndirectImpl<CMyDialog>
 {
 public:
   BEGIN_MSG_MAP(CMyDialog)
   END_MSG_MAP()

   LPCDLGTEMPLATE GetDlgTemplate()
   {
     return myDialogTemplate;
   }
 };
*/

template <class T, class TBase = CWindow>
class ATL_NO_VTABLE CDialogIndirectImpl : public CDialogImplBaseT< TBase >
{
public:

  // NOTE: user class must define LPCDLGTEMPLATE GetDlgTemplate() {};

#ifdef _DEBUG
	bool m_bModal;
  CDialogIndirectImpl() : m_bModal(false) { }
#endif //_DEBUG
	// modal dialogs
	INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
	{
		ATLASSERT(m_hWnd == NULL);
		_AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT< TBase >*)this);
#ifdef _DEBUG
		m_bModal = true;
#endif //_DEBUG
    return ::DialogBoxIndirectParam(_AtlBaseModule.GetResourceInstance(), static_cast<T*>(this)->GetDlgTemplate(),
					hWndParent, T::StartDialogProc, dwInitParam);
	}
	BOOL EndDialog(int nRetCode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_bModal);	// must be a modal dialog
		return ::EndDialog(m_hWnd, nRetCode);
	}
	// modeless dialogs
	HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL)
	{
		ATLASSERT(m_hWnd == NULL);
		_AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT< TBase >*)this);
#ifdef _DEBUG
		m_bModal = false;
#endif //_DEBUG
    HWND hWnd = ::CreateDialogIndirectParam(_AtlBaseModule.GetResourceInstance(), static_cast<T*>(this)->GetDlgTemplate(),
					hWndParent, T::StartDialogProc, dwInitParam);
		ATLASSERT(m_hWnd == hWnd);
		return hWnd;
	}
	// for CComControl
	HWND Create(HWND hWndParent, RECT&, LPARAM dwInitParam = NULL)
	{
		return Create(hWndParent, dwInitParam);
	}
	BOOL DestroyWindow()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(!m_bModal);	// must not be a modal dialog
		return ::DestroyWindow(m_hWnd);
	}
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CDialogTemplate
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
/*
  CDialogTemplate class can be used for building dialog templates in memory.
  Based on code originally posted on www.flipcode.com by Max McGuire.

  Usage:
  Can standalone or in conjuction with CDialogIndirectImpl

  class CMyDialog : public CDialogIndirectImpl<CMyDialog>
  {
  public:

    BEGIN_MSG_MAP(CMyDialog)
    END_MSG_MAP()

    CDialogTemplate dialogTemplate;

    CMyDialog() : dialogTemplate(_T("Test!"),
      DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU, 10, 10, 257, 80, _T("Tahoma"))
    {
      dialogTemplate.AddButton(_T("Exit"), WS_VISIBLE, 0, 192 + 3, 62, 56, 13, IDEXIT);
    }

    LPCDLGTEMPLATE GetDlgTemplate() { return dialogTemplate; }
  };
*/

class CDialogTemplate
{
public:
    CDialogTemplate(LPCSTR caption, DWORD style, int x, int y, int w, int h, LPCSTR font = NULL, WORD fontSize = 8)
    {
      usedBufferLength = sizeof(DLGTEMPLATE );
      totalBufferLength = usedBufferLength;

      dialogTemplate = (DLGTEMPLATE*)malloc(totalBufferLength);

      dialogTemplate->style = style;

      if (font != NULL)
      {
        dialogTemplate->style |= DS_SETFONT;
      }

      dialogTemplate->x     = x;
      dialogTemplate->y     = y;
      dialogTemplate->cx    = w;
      dialogTemplate->cy    = h;
      dialogTemplate->cdit  = 0;

      dialogTemplate->dwExtendedStyle = 0;

      // The dialog box doesn't have a menu or a special class

      AppendData(_T("\0"), 2);
      AppendData(_T("\0"), 2);

      // Add the dialog's caption to the template

      AppendString(caption);

      if (font != NULL)
      {
        AppendData(&fontSize, sizeof(WORD));
        AppendString(font);
      }
    }

    void AddControl(LPCSTR type, LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      // DWORD algin the beginning of the component data

      AlignData(sizeof(DWORD));

      DLGITEMTEMPLATE item;

      item.style = style | WS_CHILD;
      item.x     = x;
      item.y     = y;
      item.cx    = w;
      item.cy    = h;
      item.id    = id;

      item.dwExtendedStyle = exStyle;

      AppendData(&item, sizeof(DLGITEMTEMPLATE));

      AppendString(type);
      AppendString(caption);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));

      // Increment the component count

      dialogTemplate->cdit++;
    }

    void AddButton(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0080, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));

    }

    void AddEditBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0081, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));
    }

    void AddStatic(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0082, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));
    }

    void AddListBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0083, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));
    }

    void AddScrollBar(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0084, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));
    }

    void AddComboBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {
      AddStandardComponent(0x0085, caption, style, exStyle, x, y, w, h, id);

      WORD creationDataLength = 0;
      AppendData(&creationDataLength, sizeof(WORD));
    }

    /**
     * Returns a pointer to the Win32 dialog template which the object
     * represents. This pointer may become invalid if additional
     * components are added to the template.
     */

    operator const DLGTEMPLATE*() const
    {
      return dialogTemplate;
    }

    virtual ~CDialogTemplate()
    {
      free(dialogTemplate);
    }

protected:

    void AddStandardComponent(WORD type, LPCSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id)
    {

      DLGITEMTEMPLATE item;

      // DWORD algin the beginning of the component data

      AlignData(sizeof(DWORD));

      item.style = style;
      item.x     = x;
      item.y     = y;
      item.cx    = w;
      item.cy    = h;
      item.id    = id;

      item.dwExtendedStyle = exStyle;

      AppendData(&item, sizeof(DLGITEMTEMPLATE));

      WORD preType = 0xFFFF;

      AppendData(&preType, sizeof(WORD));
      AppendData(&type, sizeof(WORD));

      AppendString(caption);

      // Increment the component count

      dialogTemplate->cdit++;
    }

    void AlignData(int size)
    {
      int paddingSize = usedBufferLength % size;

      if (paddingSize != 0)
      {
        EnsureSpace(paddingSize);
        usedBufferLength += paddingSize;
      }

    }

    void AppendString(LPCSTR string)
    {
      int length = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, 0);

      WCHAR* wideString = (WCHAR*)malloc(sizeof(WCHAR) * length);
      MultiByteToWideChar(CP_ACP, 0, string, -1, wideString, length);

      AppendData(wideString, length * sizeof(WCHAR));
      free(wideString);

    }

    void AppendData(void* data, int dataLength)
    {
      EnsureSpace(dataLength);

      memcpy((char*)dialogTemplate + usedBufferLength, data, dataLength);
      usedBufferLength += dataLength;
    }

    void EnsureSpace(int length)
    {
      if (length + usedBufferLength > totalBufferLength)
      {
        totalBufferLength += length * 2;

        void* newBuffer = malloc(totalBufferLength);
        memcpy(newBuffer, dialogTemplate, usedBufferLength);

        free(dialogTemplate);
        dialogTemplate = (DLGTEMPLATE*)newBuffer;
      }
    }

private:

    DLGTEMPLATE* dialogTemplate;

    int totalBufferLength;
    int usedBufferLength;

};
}

#endif
