//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ltext.h
// Description : Script Text
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


//System messages & languages defined in seperate .h file
//system messages for all languages need to be loaded
//in case of system error before locale select

//script entries have $(NUMBER) $(TEXT) for int / string
//Text(): parse script string for $() and replace with %
//  Check va_arg for correct number/types
//  lplatform_vsprintf args into string & return

#ifndef LTEXT_H_INCLUDED
#define LTEXT_H_INCLUDED

#include <lemon/platform/lcore.h>
#include <lemon/io/lfileiostream.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

//Language ids
enum
{
  LTEXT_LANGUAGE_NONE = 0,
  LTEXT_LANGUAGE_UK_ENGLISH,
  LTEXT_LANGUAGE_US_ENGLISH,
  LTEXT_LANGUAGE_CANADIAN_ENGLISH,
  LTEXT_LANGUAGE_FRENCH,
  LTEXT_LANGUAGE_ITALIAN,
  LTEXT_LANGUAGE_GERMAN,
  LTEXT_LANGUAGE_SPANISH,
  LTEXT_LANGUAGE_DUTCH,
  LTEXT_LANGUAGE_PORTUGUESE,
  LTEXT_NUM_LANGUAGES,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lText
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lText
{
public:
  // reset any loaded data
  static void Reset();

  static void Load( lIOStreamSeekable* stream );

  static const char*  Text( int scriptId /*, ...*/ );
  static const char* SystemText( int scriptId, int language, ... );


  static const char*  Id( int scriptId );

  static int GetNumIds();

  static void               SetSupportedLanguages( const lArray<int> languageIdList );
  static const lArray<int>  GetSupportedLanguages( );

  static void               SetLanguage(int languageId);
  static int                GetLanguage();

  static const char      *  GetLanguageName(int language);
  static int                GetLanguageId(const char * languageName);
};

END_L_NAMESPACE

#endif
