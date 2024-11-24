//����������������������������������������������������������������������������
// File        : linputnds.h
// Description : NDS input system
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LINPUTNDS_H_INCLUDED
#define LINPUTNDS_H_INCLUDED

BEGIN_L_NAMESPACE

//##
//����������������������������������������������������������������������������
// Nintendo DS input device
//����������������������������������������������������������������������������

class LINPUT_API lInputSystemNDS : public lInputSystemBase
{
public:

  lInputSystemNDS();
  ~lInputSystemNDS();

  // lInputSystemBase interfaces

  bool                        Init(void * platformSpecific);
  void                        Deinit();
  void                        Update();
  void                        GlobalFocus(bool);

  virtual unsigned int        NumDevices();
  virtual lInputDevice      * GetDevice( unsigned int );

  virtual lInputStatus        CheckStatus( uint32 deviceID );
  virtual lInputDeviceType    CheckType( uint32 deviceID );

  const int                 * GetKeyScanCodeBuffer(int *events)
  {
    L_UNUSED_PARAM( events );
    return NULL;
  }

  const char                * GetKeyAsciiBuffer(int *events)
  {
    L_UNUSED_PARAM( events );
    return NULL;
  }

  unsigned char               ScanToAscii(unsigned char scan,bool shift)
  {
    L_UNUSED_PARAM( scan );
    L_UNUSED_PARAM( shift );
    return 0;
  }

  void                        ClearKeyBuffers()
  {
  }
  void                        Vibrate(uint8,uint8)
  {
  }

private:
  static bool       s_bInit;

};

END_L_NAMESPACE

#endif
