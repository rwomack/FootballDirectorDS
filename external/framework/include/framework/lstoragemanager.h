//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lstoragemanager.h
// Description : Storage Manager
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2006 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LSTORAGEMANAGER_H
#define __LSTORAGEMANAGER_H

#include <lemon/object/lsharedptr.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE
struct lDialog;
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lStorageManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lStorageManager
{
public:
  static void Init( int dataSize, const char* saveName, const char* saveLocation );
  static void Deinit();

  // read existing data
  static void LoadData( void *data );

  // write new data over current data
  #ifdef L_PLATFORM_PSP
  static void SaveData( void *data, uint32 saveID );
  #else
  static void SaveData( void *data );
  #endif

  static void SetAutoSave( bool autosaveOn );

  static bool IsSaving();

  #ifdef L_PLATFORM_PS2
  static void SetSaveIcon( const char* filename );
  #endif

  #ifdef L_PLATFORM_NDS
  static bool ConfirmBackupMemory();
  static void DS_SetEEPROMType( CARDBackupType type );
  #endif

  static lSignal1<lDialog*> DisplayDialogSignal;
  static lSignal1<bool> SaveCompleteSignal; //Send true for success
  static lSignal3<bool,bool,bool> LoadCompleteSignal;  // bool bSuccess, bool bCorruptSave, bool bNoSaveExisted

  static void StorageDialogDisplayed( lDialog* dialog ); //need to know loading.. etc dialogs are onscreen before resuming process!!
  static void StorageDialogClosed( lDialog* dialog );

  #if defined(L_PLATFORM_WII) || defined(L_PLATFORM_PSP)
  static bool IsContinueWithoutSaving();
  static void SetContinueWithoutSaving(bool);
  static const char* GetLog();
  #endif

  #if defined(L_PLATFORM_WII)
  static void SetConfirmSystemMenuWarning(bool);
  #endif
  
  #ifdef L_PLATFORM_PSP
  static void SaveLoadComplete();
  static void ShowAutosaveWarning();
  static bool IsDeleteListOpen();
  static bool IsActive();
  static void DeleteData();
  static uint32 GetSaveID(void*);
  static uint32 GetCurrentSaveID();
  static void CheckSaveID();
  static void ClearSaveIDFlags();
  static void InjectSaveID(void*,uint32);
  static bool IsNewCardWithData();
  static void DisplayLoadNewCardMessage();
  static void FlagMemoryStickInserted(bool);
  #endif
};

END_L_NAMESPACE
#endif
