//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : latl.h
// Description : Convenience ATL/WTL include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 Use 'using namespace WTL' to include the WTL namespace

*/

#ifndef WTL_LATL_H_INCLUDED
#define WTL_LATL_H_INCLUDED

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// ATL/WTL
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#if defined(_WIN32)
#  include <atlbase.h>                 // ATL
#  include <atlwin.h>                  // ATL
#  define _WTL_NO_AUTOMATIC_NAMESPACE
#  include <atlapp.h>                  // WTL (for CAppModule)
   extern WTL::CAppModule _Module;     // You must declare this in your app
#  include <atlwin.h>                  // ATL
#  include <atlmisc.h>                 // WTL
#  include <atlctrls.h>                // WTL
#  include <atlframe.h>                // WTL
#  include <atlctrlw.h>
#  include <atlctrlx.h>
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Additional ATL classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "./latldialog.h"

#endif
