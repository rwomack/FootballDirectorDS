//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : linputdx8.h
// Description : DirectX8 input system
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LINPUTDX8_H_INCLUDED
#define LINPUTDX8_H_INCLUDED

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.H>

BEGIN_L_NAMESPACE

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// DirectX8 input device
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LINPUT_API lInputSystemDX8 : public lInputSystemBase
{
  public:

  lInputSystemDX8();
  ~lInputSystemDX8();

  // lInputSystemBase interfaces

  bool                Init(void * osInfo = NULL);
  void                Deinit();
  void                Update();
  void                GlobalFocus(bool);
  const int         * GetKeyScanCodeBuffer(int *events);
  const char        * GetKeyAsciiBuffer(int *events);
  unsigned char       ScanToAscii(unsigned char scan,bool shift);
  void                ClearKeyBuffers();
  void                Vibrate(uint8,uint8);

  virtual lInputStatus        CheckStatus( uint32 deviceID );
  virtual lInputDeviceType    CheckType( uint32 deviceID );

  virtual unsigned int       NumDevices();
  virtual lInputDevice     * GetDevice( unsigned int );

  // lInputSystemDX8 interface

  // lInputSystemDX8 implementation

  private:
    struct  DeviceDX8Base { LPDIRECTINPUTDEVICE8 lpDevice; int deviceID; };
    typedef lList<DeviceDX8Base> DeviceDX8;

    LPDIRECTINPUT8         _lpDI;                                     // Direct input object
    HWND                   _hwndFocus;                                // Focus window
    LPDIRECTINPUTDEVICE8   _mouseDevice;                              // Mouse device (or NULL)
    LPDIRECTINPUTDEVICE8   _keyboardDevice;                           // Keyboard device (or NULL)
    DeviceDX8            * _deviceList;                               // Other device list
    int                    _curDeviceID;                              // current device id

    char                   _scanASCII[512];                           // Scan code to ascii code table

    int                    _lshiftState;                              // Left shift state
    int                    _rshiftState;                              // Right shift state
    int                    _shiftState;                               // General shift state

    enum                   { KEYBOARD_BUFFER_SIZE = 256 };
    enum                   { MOUSE_BUFFER_SIZE = 256 };

    DIDEVICEOBJECTDATA     _dataKey[KEYBOARD_BUFFER_SIZE];            // Buffer key data buffer
    int                    _userScanBuf[KEYBOARD_BUFFER_SIZE];        // User scan code buffer
    char                   _userASCIIBuf[KEYBOARD_BUFFER_SIZE];       // User ascii buffer
    int                    _keyEvents;                                // Number of scan code events in scan code buffer
    int                    _asciiEvents;                              // Number of ascii events in ascii buffer

    bool                   _joyButtons[LI_MAX_INPUT_DEVICES * 32];

    DIDEVICEOBJECTDATA     _dataMouse[MOUSE_BUFFER_SIZE];             // Buffer mouse data buffer
    //int                    _userMouseBuf[MOUSE_BUFFER_SIZE*4];        // User
    int                    _mouseEvents;

    int                    _lastKey;                                  // Last key scan code
    unsigned int           _lastKeyTime;                              // Last key down time
    char                   _lastKeyAscii;                             // Last key down ascii code
    unsigned int           _lastKeyRepeat;                            // Last key repeat rate
    unsigned int           _lastKeyShift;                             // Lasy key shift state


    void updateMouse();
    void updateKeyboard();
    void updateDevice( LPDIRECTINPUTDEVICE8 device, int deviceID );
    void updateXInput();

    static BOOL CALLBACK diEnumCallback(LPCDIDEVICEINSTANCE dev,LPVOID pvRef);
};

END_L_NAMESPACE

#endif
