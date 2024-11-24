//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : linput.h
// Description : Input interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*

 Notes
 -----

 Expose available devices
 Can return device to query
 Device also works through lInput interface which allows bindings to the global set

 Add in 'device' select in getinputstate

 Store info on which input came from which device


 Extensions to input system (idea)

 Parameter will be an integer, will allow us to have a default mode to work as it does now.

 We'll make it used with an offset so that we can change the offset to incorporate new systems

 GetInputState( L_PAD_LEFT, -1 )     - default param, uses default input device for the state

 GetInputState( L_PAD_LEFT, L_INPUT_DEVICE + 0 )      - left on device 0
 GetInputState( L_PAD_LEFT, L_INPUT_DEVICE + 1 )      - left on device 1

 myMapping = CreateMappingSet( "Player 1 mappings" );
 GetInputState( L_PAD_LEFT, myMapping )      - left according to mappings created by player 1 (in setup screen for example)

 Device->GetInputState - just calls getinputstate with the device id

 Multitap support - load IRX, check which ports are multitap and open accordingly (Don抰 open if not multitap)

 ToDo
 ----

 Needs to check for device removal etc

 *** THIS LIBRARY IS BEING REFACTORED FOR THE PS2 ***

 The interface is changing as follows

 The base input system will return pointers to devices attached to the system,
 which can be referenced by a unique GUID that the application can track for
 input mapping. Devices will also support force feedback.
 The global input system will automatically map common input types to common
 devices for ease of use (such as th keyboard, and the first joypad).

 * On platforms without access to certain input devices, the input types should
   be mapped to LI_NONE at compile time.

 * For ascii event, and input stats, keydown is signified by 0x80.
   For the scan code buffer, a key down is signified by 0x100.

 * Joystick and pad interfaces

 * Sort out sticky and non-sticky states,
   currently key states are sticky (they must be reset by the user), and all
   other states are non-sticky.

 * Perhaps add key feeding, and make the keyboard buffers part of this
   base class.

*/

#include "./linputapi.h"

#ifndef LINPUT_H_INCLUDED
#define LINPUT_H_INCLUDED

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Global Variables
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lInputSystemBase;
extern LINPUT_API lInputSystemBase * lInput;

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Input types
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lInputType
{
  LI_NONE             = 0x00 ,

  // Keyboard definitions, these map directly to Win32 scan codes

  LI_KEY_ESCAPE       = 0x01 ,
  LI_KEY_1            = 0x02 ,
  LI_KEY_2            = 0x03 ,
  LI_KEY_3            = 0x04 ,
  LI_KEY_4            = 0x05 ,
  LI_KEY_5            = 0x06 ,
  LI_KEY_6            = 0x07 ,
  LI_KEY_7            = 0x08 ,
  LI_KEY_8            = 0x09 ,
  LI_KEY_9            = 0x0A ,
  LI_KEY_0            = 0x0B ,
  LI_KEY_MINUS        = 0x0C ,  // - on main keyboard
  LI_KEY_EQUALS       = 0x0D ,
  LI_KEY_BACK         = 0x0E ,  // backspace
  LI_KEY_TAB          = 0x0F ,
  LI_KEY_Q            = 0x10 ,
  LI_KEY_W            = 0x11 ,
  LI_KEY_E            = 0x12 ,
  LI_KEY_R            = 0x13 ,
  LI_KEY_T            = 0x14 ,
  LI_KEY_Y            = 0x15 ,
  LI_KEY_U            = 0x16 ,
  LI_KEY_I            = 0x17 ,
  LI_KEY_O            = 0x18 ,
  LI_KEY_P            = 0x19 ,
  LI_KEY_LBRACKET     = 0x1A ,
  LI_KEY_RBRACKET     = 0x1B ,
  LI_KEY_RETURN       = 0x1C ,  // Enter on main keyboard
  LI_KEY_LCONTROL     = 0x1D ,
  LI_KEY_A            = 0x1E ,
  LI_KEY_S            = 0x1F ,
  LI_KEY_D            = 0x20 ,
  LI_KEY_F            = 0x21 ,
  LI_KEY_G            = 0x22 ,
  LI_KEY_H            = 0x23 ,
  LI_KEY_J            = 0x24 ,
  LI_KEY_K            = 0x25 ,
  LI_KEY_L            = 0x26 ,
  LI_KEY_SEMICOLON    = 0x27 ,
  LI_KEY_APOSTROPHE   = 0x28 ,
  LI_KEY_GRAVE        = 0x29 ,  // accent grave
  LI_KEY_LSHIFT       = 0x2A ,
  LI_KEY_BACKSLASH    = 0x2B ,
  LI_KEY_Z            = 0x2C ,
  LI_KEY_X            = 0x2D ,
  LI_KEY_C            = 0x2E ,
  LI_KEY_V            = 0x2F ,
  LI_KEY_B            = 0x30 ,
  LI_KEY_N            = 0x31 ,
  LI_KEY_M            = 0x32 ,
  LI_KEY_COMMA        = 0x33 ,
  LI_KEY_PERIOD       = 0x34 ,  // . on main keyboard
  LI_KEY_SLASH        = 0x35 ,  // / on main keyboard
  LI_KEY_RSHIFT       = 0x36 ,
  LI_KEY_MULTIPLY     = 0x37 ,  // * on numeric keypad
  LI_KEY_LMENU        = 0x38 ,  // left Alt
  LI_KEY_SPACE        = 0x39 ,
  LI_KEY_CAPITAL      = 0x3A ,
  LI_KEY_F1           = 0x3B ,
  LI_KEY_F2           = 0x3C ,
  LI_KEY_F3           = 0x3D ,
  LI_KEY_F4           = 0x3E ,
  LI_KEY_F5           = 0x3F ,
  LI_KEY_F6           = 0x40 ,
  LI_KEY_F7           = 0x41 ,
  LI_KEY_F8           = 0x42 ,
  LI_KEY_F9           = 0x43 ,
  LI_KEY_F10          = 0x44 ,
  LI_KEY_NUMLOCK      = 0x45 ,
  LI_KEY_SCROLL       = 0x46 ,  // Scroll Lock
  LI_KEY_NUMPAD7      = 0x47 ,
  LI_KEY_NUMPAD8      = 0x48 ,
  LI_KEY_NUMPAD9      = 0x49 ,
  LI_KEY_SUBTRACT     = 0x4A ,  // - on numeric keypad
  LI_KEY_NUMPAD4      = 0x4B ,
  LI_KEY_NUMPAD5      = 0x4C ,
  LI_KEY_NUMPAD6      = 0x4D ,
  LI_KEY_ADD          = 0x4E ,  // + on numeric keypad
  LI_KEY_NUMPAD1      = 0x4F ,
  LI_KEY_NUMPAD2      = 0x50 ,
  LI_KEY_NUMPAD3      = 0x51 ,
  LI_KEY_NUMPAD0      = 0x52 ,
  LI_KEY_DECIMAL      = 0x53 ,  // . on numeric keypad
  LI_KEY_F11          = 0x57 ,
  LI_KEY_F12          = 0x58 ,

  LI_KEY_F13          = 0x64 ,  //                     (NEC PC98)
  LI_KEY_F14          = 0x65 ,  //                     (NEC PC98)
  LI_KEY_F15          = 0x66 ,  //                     (NEC PC98)

  LI_KEY_KANA         = 0x70 ,  // (Japanese keyboard)
  LI_KEY_CONVERT      = 0x79 ,  // (Japanese keyboard)
  LI_KEY_NOCONVERT    = 0x7B ,  // (Japanese keyboard)
  LI_KEY_YEN          = 0x7D ,  // (Japanese keyboard)
  LI_KEY_NUMPADEQUALS = 0x8D ,  // = on numeric keypad (NEC PC98)
  LI_KEY_CIRCUMFLEX   = 0x90 ,  // (Japanese keyboard)
  LI_KEY_AT           = 0x91 ,  //                     (NEC PC98)
  LI_KEY_COLON        = 0x92 ,  //                     (NEC PC98)
  LI_KEY_UNDERLINE    = 0x93 ,  //                     (NEC PC98)
  LI_KEY_KANJI        = 0x94 ,  // (Japanese keyboard)
  LI_KEY_STOP         = 0x95 ,  //                     (NEC PC98)
  LI_KEY_AX           = 0x96 ,  //                     (Japan AX)
  LI_KEY_UNLABELED    = 0x97 ,  //                        (J3100)
  LI_KEY_NUMPADENTER  = 0x9C ,  // Enter on numeric keypad
  LI_KEY_RCONTROL     = 0x9D ,
  LI_KEY_NUMPADCOMMA  = 0xB3 ,  // , on numeric keypad (NEC PC98)
  LI_KEY_DIVIDE       = 0xB5 ,  // / on numeric keypad
  LI_KEY_SYSRQ        = 0xB7 ,
  LI_KEY_RMENU        = 0xB8 ,  // right Alt
  LI_KEY_HOME         = 0xC7 ,  // Home on arrow keypad
  LI_KEY_UP           = 0xC8 ,  // UpArrow on arrow keypad
  LI_KEY_PRIOR        = 0xC9 ,  // PgUp on arrow keypad
  LI_KEY_LEFT         = 0xCB ,  // LeftArrow on arrow keypad
  LI_KEY_RIGHT        = 0xCD ,  // RightArrow on arrow keypad
  LI_KEY_END          = 0xCF ,  // End on arrow keypad
  LI_KEY_DOWN         = 0xD0 ,  // DownArrow on arrow keypad
  LI_KEY_NEXT         = 0xD1 ,  // PgDn on arrow keypad
  LI_KEY_INSERT       = 0xD2 ,  // Insert on arrow keypad
  LI_KEY_DELETE       = 0xD3 ,  // Delete on arrow keypad
  LI_KEY_LWIN         = 0xDB ,  // Left Windows key
  LI_KEY_RWIN         = 0xDC ,  // Right Windows key
  LI_KEY_APPS         = 0xDD ,  // AppMenu key

  // Mouse definitions

  LI_MOUSE_X_AXIS     = 0x100 ,
  LI_MOUSE_Y_AXIS     = 0x101 ,
  LI_MOUSE_Z_AXIS     = 0x102 ,
  LI_MOUSE_BUTTON1    = 0x103 ,
  LI_MOUSE_BUTTON2    = 0x104 ,
  LI_MOUSE_BUTTON3    = 0x105 ,
  LI_MOUSE_BUTTON4    = 0x106 ,

  // Pad definitions

  LI_PAD1_THUMBL_X_AXIS = 0x200 ,
  LI_PAD1_THUMBL_Y_AXIS,
  LI_PAD1_THUMBL_Z_AXIS,

  LI_PAD1_THUMBR_X_AXIS,
  LI_PAD1_THUMBR_Y_AXIS,
  LI_PAD1_THUMBR_Z_AXIS,

  LI_PAD1_DPAD_UP,
  LI_PAD1_DPAD_DOWN,
  LI_PAD1_DPAD_LEFT,
  LI_PAD1_DPAD_RIGHT,

  LI_PAD1_POV1_UP,
  LI_PAD1_POV1_DOWN,
  LI_PAD1_POV1_LEFT,
  LI_PAD1_POV1_RIGHT,

  LI_PAD1_POV2_UP,
  LI_PAD1_POV2_DOWN,
  LI_PAD1_POV2_LEFT,
  LI_PAD1_POV2_RIGHT,

  LI_PAD1_POV3_UP,
  LI_PAD1_POV3_DOWN,
  LI_PAD1_POV3_LEFT,
  LI_PAD1_POV3_RIGHT,

  LI_PAD1_POV4_UP,
  LI_PAD1_POV4_DOWN,
  LI_PAD1_POV4_LEFT,
  LI_PAD1_POV4_RIGHT,

  LI_PAD1_BUTTON1,
  LI_PAD1_BUTTON2,
  LI_PAD1_BUTTON3,
  LI_PAD1_BUTTON4,
  LI_PAD1_BUTTON5,
  LI_PAD1_BUTTON6,
  LI_PAD1_BUTTON7,
  LI_PAD1_BUTTON8,
  LI_PAD1_BUTTON9,
  LI_PAD1_BUTTON10,
  LI_PAD1_BUTTON11,
  LI_PAD1_BUTTON12,
  LI_PAD1_BUTTON13,
  LI_PAD1_BUTTON14,
  LI_PAD1_BUTTON15,
  LI_PAD1_BUTTON16,
  LI_PAD1_BUTTON17,
  LI_PAD1_BUTTON18,
  LI_PAD1_BUTTON19,
  LI_PAD1_BUTTON20,
  LI_PAD1_BUTTON21,
  LI_PAD1_BUTTON22,
  LI_PAD1_BUTTON23,
  LI_PAD1_BUTTON24,
  LI_PAD1_BUTTON25,
  LI_PAD1_BUTTON26,
  LI_PAD1_BUTTON27,
  LI_PAD1_BUTTON28,
  LI_PAD1_BUTTON29,
  LI_PAD1_BUTTON30,
  LI_PAD1_BUTTON31,
  LI_PAD1_BUTTON32,

  LI_PAD1_UP,
  LI_PAD1_DOWN,
  LI_PAD1_LEFT,
  LI_PAD1_RIGHT,

  // The number of input types

  LI_NUM_INPUT_TYPES

  // Additional types for specific platforms
  // NOTE: All types after here must be set to an enum before LI_NUM_INPUT_TYPES

  // XInput
  ,
  LI_XINPUT_PAD1_LEFT_TRIGGER              =  LI_PAD1_BUTTON11,
  LI_XINPUT_PAD1_RIGHT_TRIGGER             =  LI_PAD1_BUTTON12,
  LI_XINPUT_PAD1_BUTTON_A                  =  LI_PAD1_BUTTON1,
  LI_XINPUT_PAD1_BUTTON_B                  =  LI_PAD1_BUTTON2,
  LI_XINPUT_PAD1_BUTTON_X                  =  LI_PAD1_BUTTON3,
  LI_XINPUT_PAD1_BUTTON_Y                  =  LI_PAD1_BUTTON4,
  LI_XINPUT_PAD1_BUTTON_START              =  LI_PAD1_BUTTON5,
  LI_XINPUT_PAD1_BUTTON_BACK               =  LI_PAD1_BUTTON6,
  LI_XINPUT_PAD1_BUTTON_LEFT_THUMB         =  LI_PAD1_BUTTON7,
  LI_XINPUT_PAD1_BUTTON_RIGHT_THUMB        =  LI_PAD1_BUTTON8,
  LI_XINPUT_PAD1_BUTTON_LEFT_SHOULDER      =  LI_PAD1_BUTTON9,
  LI_XINPUT_PAD1_BUTTON_RIGHT_SHOULDER     =  LI_PAD1_BUTTON10

  // PS2
  ,
  LI_PS2_PAD1_BUTTON_TRIANGLE           =  LI_PAD1_BUTTON1,
  LI_PS2_PAD1_BUTTON_CIRCLE             =  LI_PAD1_BUTTON2,
  LI_PS2_PAD1_BUTTON_CROSS              =  LI_PAD1_BUTTON3,
  LI_PS2_PAD1_BUTTON_SQUARE             =  LI_PAD1_BUTTON4,
  LI_PS2_PAD1_BUTTON_START              =  LI_PAD1_BUTTON5,
  LI_PS2_PAD1_BUTTON_SELECT             =  LI_PAD1_BUTTON6,
  LI_PS2_PAD1_BUTTON_L1                 =  LI_PAD1_BUTTON7,
  LI_PS2_PAD1_BUTTON_R1                 =  LI_PAD1_BUTTON8,
  LI_PS2_PAD1_BUTTON_L2                 =  LI_PAD1_BUTTON9,
  LI_PS2_PAD1_BUTTON_R2                 =  LI_PAD1_BUTTON10

  // PSP
  ,
  LI_PSP_PAD1_BUTTON_TRIANGLE           =  LI_PAD1_BUTTON1,
  LI_PSP_PAD1_BUTTON_CIRCLE             =  LI_PAD1_BUTTON2,
  LI_PSP_PAD1_BUTTON_CROSS              =  LI_PAD1_BUTTON3,
  LI_PSP_PAD1_BUTTON_SQUARE             =  LI_PAD1_BUTTON4,
  LI_PSP_PAD1_BUTTON_START              =  LI_PAD1_BUTTON5,
  LI_PSP_PAD1_BUTTON_SELECT             =  LI_PAD1_BUTTON6,
  LI_PSP_PAD1_BUTTON_L                  =  LI_PAD1_BUTTON7,
  LI_PSP_PAD1_BUTTON_R                  =  LI_PAD1_BUTTON8,
  LI_PSP_PAD1_BUTTON_HOLD_STATE         =  LI_PAD1_BUTTON9,  //## Hold state is set on PSP, no other inputs will be set
  LI_PSP_PAD1_BUTTON_INTERCEPTED        =  LI_PAD1_BUTTON10  //## Inputs are being intercepted most likely by a popup, the analog may still be returned


  // Xbox 1
  ,
  LI_XBOX_PAD1_BUTTON_A                 =  LI_PAD1_BUTTON1,
  LI_XBOX_PAD1_BUTTON_B                 =  LI_PAD1_BUTTON2,
  LI_XBOX_PAD1_BUTTON_X                 =  LI_PAD1_BUTTON3,
  LI_XBOX_PAD1_BUTTON_Y                 =  LI_PAD1_BUTTON4,
  LI_XBOX_PAD1_BUTTON_BLACK             =  LI_PAD1_BUTTON5,
  LI_XBOX_PAD1_BUTTON_WHITE             =  LI_PAD1_BUTTON6,
  LI_XBOX_PAD1_BUTTON_LEFT_TRIGGER      =  LI_PAD1_BUTTON7,
  LI_XBOX_PAD1_BUTTON_RIGHT_TRIGGER     =  LI_PAD1_BUTTON8,
  LI_XBOX_PAD1_BUTTON_START             =  LI_PAD1_BUTTON9,
  LI_XBOX_PAD1_BUTTON_BACK              =  LI_PAD1_BUTTON10,
  LI_XBOX_PAD1_BUTTON_LEFT_THUMB        =  LI_PAD1_BUTTON11,
  LI_XBOX_PAD1_BUTTON_RIGHT_THUMB       =  LI_PAD1_BUTTON12

  //NDS
  ,
  LI_NDS_PAD1_BUTTON_B                  = LI_PAD1_BUTTON1,
  LI_NDS_PAD1_BUTTON_X                  = LI_PAD1_BUTTON2,
  LI_NDS_PAD1_BUTTON_A                  = LI_PAD1_BUTTON3,
  LI_NDS_PAD1_BUTTON_Y                  = LI_PAD1_BUTTON4,
  LI_NDS_PAD1_BUTTON_START              = LI_PAD1_BUTTON5,
  LI_NDS_PAD1_BUTTON_SELECT             = LI_PAD1_BUTTON6,
  LI_NDS_PAD1_BUTTON_L                  = LI_PAD1_BUTTON7,
  LI_NDS_PAD1_BUTTON_R                  = LI_PAD1_BUTTON8

  //WII
  ,
  LI_WII_PAD1_BUTTON_A                  = LI_PAD1_BUTTON1,
  LI_WII_PAD1_BUTTON_B                  = LI_PAD1_BUTTON2,
  LI_WII_PAD1_BUTTON_HOME               = LI_PAD1_BUTTON3,
  LI_WII_PAD1_BUTTON_PLUS               = LI_PAD1_BUTTON4,
  LI_WII_PAD1_BUTTON_MINUS              = LI_PAD1_BUTTON5,
  LI_WII_PAD1_BUTTON_1                  = LI_PAD1_BUTTON6,
  LI_WII_PAD1_BUTTON_2                  = LI_PAD1_BUTTON7,
  LI_WII_PAD1_BUTTON_Z                  = LI_PAD1_BUTTON8,
  LI_WII_PAD1_BUTTON_C                  = LI_PAD1_BUTTON9,

  LI_WII_PAD1_ACCEL_X                   = LI_PAD1_BUTTON10,
  LI_WII_PAD1_ACCEL_Y                   = LI_PAD1_BUTTON11,
  LI_WII_PAD1_ACCEL_Z                   = LI_PAD1_BUTTON12,

  LI_WII_PAD1_DISTANCE                  = LI_PAD1_BUTTON13,

  LI_WII_PAD1_VERTICAL_X                = LI_PAD1_BUTTON14,
  LI_WII_PAD1_VERTICAL_Y                = LI_PAD1_BUTTON15,

  LI_WII_PAD1_HORIZON_X                = LI_PAD1_BUTTON16,
  LI_WII_PAD1_HORIZON_Y                = LI_PAD1_BUTTON17,
  
  //XBOX 360
  LI_XBOX360_PAD1_BUTTON_A             = LI_PAD1_BUTTON1,
  LI_XBOX360_PAD1_BUTTON_B             = LI_PAD1_BUTTON2,
  LI_XBOX360_PAD1_BUTTON_X             = LI_PAD1_BUTTON3,
  LI_XBOX360_PAD1_BUTTON_Y             = LI_PAD1_BUTTON4,
  LI_XBOX360_PAD1_BUTTON_START         = LI_PAD1_BUTTON5,
  LI_XBOX360_PAD1_BUTTON_BACK          = LI_PAD1_BUTTON6,
  LI_XBOX360_PAD1_BUTTON_LEFT_THUMB    = LI_PAD1_BUTTON7,
  LI_XBOX360_PAD1_BUTTON_RIGHT_THUMB   = LI_PAD1_BUTTON8,
  LI_XBOX360_PAD1_BUTTON_LEFT_SHOULDER = LI_PAD1_BUTTON9,
  LI_XBOX360_PAD1_BUTTON_RIGHT_SHOULDER= LI_PAD1_BUTTON10,
  LI_XBOX360_PAD1_BUTTON_BIG           = LI_PAD1_BUTTON11,
  LI_XBOX360_PAD1_LEFT_TRIGGER         = LI_PAD1_BUTTON12,
  LI_XBOX360_PAD1_RIGHT_TRIGGER        = LI_PAD1_BUTTON13,
  
};

//##
// Maximum number of devices we support
enum lInputDeviceNum
{
  LI_MAX_INPUT_DEVICES = 8,
};

//##
// Input device types
enum lInputDeviceType
{
  LI_DEVICE_KEYBOARD,
  LI_DEVICE_MOUSE,
  LI_DEVICE_JOYSTICK,
  LI_DEVICE_CONTROLPAD,

  // Win32 specific types

  LI_WIN32_DEVICE_X360_GAMEPAD,

  // PS2 specific types

  LI_PS2_DEVICE_DUALSHOCK,

  // Wii specific types

  LI_WII_DEVICE_WIIMOTE,          //## Wiimote
  LI_WII_DEVICE_WIIMOTE_NUNCHUK,  //## Wiimote + Nunchuk
  LI_WII_DEVICE_WIIMOTE_CLASSIC,  //## Wiimote + Classic controller
  LI_WII_DEVICE_WIIMOTE_FUTURE,   //## Wiimote + Future controller
  LI_WII_DEVICE_WIIMOTE_MASTER,   //## Wiimote + Master controller
  LI_WII_DEVICE_WIIMOTE_UNKNOWN,  //## Wiimote + Unknown - interim type while an extension controller is being recognised

  LI_XBOX360_DEVICE_CONTROL_PAD,
  
  LI_DEVICE_UNKNOWN,
};

//##
// Input status values
enum lInputStatus
{
  LI_STATUS_OK,
  LI_STATUS_ERROR,
  LI_STATUS_PROCESSING,
  LI_STATUS_DISCONNECTED,

  LI_STATUS_UNKNOWN,
};

// Each input device to have a GUID.
//

//##
// Information on an input device
class lInputDevice
{
public:
  //##
  // Query the device type (lInputDeviceType)
  virtual lInputDeviceType      Type() const = 0;

  //##
  // Query the ID of this device
  virtual unsigned int          GetID() const = 0;

  //##
  // Return the lInputTypes that this device exposes
  virtual lArray<lInputType>   &GetInputTypes() const = 0;
};

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Base input system
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LINPUT_API lInputSystemBase
{
public:

  lInputSystemBase();
  virtual ~lInputSystemBase();

  // Initialisation

  virtual bool                Init(void * platformSpecific = NULL) = 0;         //## Init the input system
  virtual void                Deinit() = 0;                                     //## Deinit the input system
  virtual void                Update() = 0;                                     //## Update the input state
  virtual void                GlobalFocus(bool) = 0;                            //## Called when when gaining/loosing global focus
  static void                 Select( lInputSystemBase * );                     //## Select the global input device

  // Device interface

  virtual unsigned int        NumDevices() = 0;                                 //## Number of attached devices
  virtual lInputDevice      * GetDevice( unsigned int ) = 0;                    //## Get a specific device number

//  virtual bool                SetDevicePriority( int32, lInputDevice * ) = 0;   //## Set priority of a device
//  virtual int32               GetDevicePriority( lInputDevice * ) = 0;          //## Get priority of a device

  virtual lInputStatus        CheckStatus( uint32 deviceID ) = 0;               //## Get status of a device id (*currently* port number on PS2 / Xbox)
  virtual lInputDeviceType    CheckType( uint32 deviceID ) = 0;                 //## Get type of a device id (*currently* port number on PS2 / Xbox)

  // Keyboard interface

  virtual const int *         GetKeyScanCodeBuffer(int *events) = 0;            //## Get keyboard scan code buffer
  virtual const char *        GetKeyAsciiBuffer(int *events) = 0;               //## Get keyboard ascii code buffer
  virtual unsigned char       ScanToAscii(unsigned char scan,bool shift) = 0;   //## Convert a scan code to an ascii code
  virtual void                ClearKeyBuffers() = 0;                            //## Clear keyboard buffers

  virtual void                Vibrate( uint8,uint8 ) = 0;

  // Base implementation

  lInputDevice              * GetDeviceType( lInputDeviceType );                //## Get first occurance of a device

  inline void                 SetKeyDelay(unsigned int delay)                   { _keyRepeatDelay = delay;     }
  inline void                 SetKeyRepeat(unsigned int repeat)                 { _keyRepeatRate = repeat;     }
  inline void                 SetExtendedAsciiState(bool state)                 { _extendedAsciiState = state; }
  inline const unsigned int   GetKeyDelay() const                               { return _keyRepeatDelay;      }
  inline const unsigned int   GetKeyRepeat() const                              { return _keyRepeatRate;       }
  inline const bool           GetExtendedAsciiState(void) const                 { return _extendedAsciiState;  }

  // Input states

//  inline const float          GetInputState( lInputType inputType ) const        { if( inputType >= 0 && inputType < LI_NUM_INPUT_TYPES ) return _inputStateTable[ inputType ]; return 0; }
//  inline void                 SetInputState( lInputType inputType, float state ) { if( inputType >= 0 && inputType < LI_NUM_INPUT_TYPES ) _inputStateTable[ inputType ] = state; }

  //##
  // Get the input state for the type/device specified
  // In:
  //   inputType - type to query
  //   deviceID  - device to query (UNUSED)
  // Out:
  //   Returns the floating point value of this input state
  inline const float          GetInputState( lInputType inputType, uint32 deviceID = 0 ) const        { lAssert( deviceID < LI_MAX_INPUT_DEVICES ); if( inputType >= 0 && inputType < LI_NUM_INPUT_TYPES ) return _inputStateTable[ (deviceID * LI_NUM_INPUT_TYPES) + (uint32)inputType ]; return 0; }

  //##
  // Set the input state for the type/device specified
  // In:
  //   inputType - type to set
  //   state     - floating point value to set the state to
  //   deviceID  - device to set (UNUSED)
  inline void                 SetInputState( lInputType inputType, float state, uint32 deviceID = 0 ) { lAssert( deviceID < LI_MAX_INPUT_DEVICES ); if( inputType >= 0 && inputType < LI_NUM_INPUT_TYPES ) _inputStateTable[ (deviceID * LI_NUM_INPUT_TYPES) + (uint32)inputType ] = state; }

  // General mouse helper routines

  //## Return the default mouse x axis value
  inline const float          GetMouseX( int deviceID = 0 ) const                                 { return _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_X_AXIS]; }
  //## Return the default mouse y axis value
  inline const float          GetMouseY( int deviceID = 0 ) const                                 { return _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_Y_AXIS]; }
  //## Return the default mouse z axis value
  inline const float          GetMouseZ( int deviceID = 0 ) const                                 { return _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_Z_AXIS]; }
  //## Return the state of the mouse button specified
  inline const float          GetMouseButton( int button, int deviceID = 0 ) const                 { if( button < 0 || button > 3 ) return 0; else return _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_BUTTON1+button]; }

  inline void                 SetMouseX( float val, int deviceID = 0 )                              { _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_X_AXIS] = val;  }
  inline void                 SetMouseY( float val, int deviceID = 0 )                              { _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_Y_AXIS] = val;  }
  inline void                 SetMouseZ( float val, int deviceID = 0 )                              { _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_Z_AXIS] = val;  }
  inline void                 SetMouseButton( int button, float state, int deviceID = 0 )           { if( button < 0 || button > 3 ) return; else _inputStateTable[(deviceID * LI_NUM_INPUT_TYPES) + LI_MOUSE_BUTTON1+button] = state; }

private:
  unsigned int _keyRepeatDelay;                         // key repeat delay rate in ms
  unsigned int _keyRepeatRate;                          // key repeat rate in ms
  bool         _extendedAsciiState;                     // true if extended ascii codes are enabled
  float        _inputStateTable[LI_MAX_INPUT_DEVICES * LI_NUM_INPUT_TYPES];   // input type states
};

END_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Switch right include file
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#if defined(L_PLATFORM_PS2)
  #include "./linputps2.h"
#elif defined(L_PLATFORM_PSP)
  #include "./linputpsp.h"
#elif defined(L_PLATFORM_NDS)
  #include "./linputnds.h"
#elif defined(L_PLATFORM_XBOX)
  #include "./linputxbox.h"
#elif defined(L_PLATFORM_WII)
  #include "./linputwii.h"
#elif defined(L_PLATFORM_XBOX360)
  #include "./linputxbox360.h"
#elif defined(_WIN32)
  #include "./linputdx8.h"
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform typedefs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

BEGIN_L_NAMESPACE

  #if defined(L_PLATFORM_PS2)
    typedef lInputSystemPS2        lInputSystem;
  #elif defined(L_PLATFORM_PSP)
    typedef lInputSystemPSP        lInputSystem;
  #elif defined(L_PLATFORM_NDS)
    typedef lInputSystemNDS        lInputSystem;
  #elif defined(L_PLATFORM_WII)
    typedef lInputSystemWII        lInputSystem;
  #elif defined(L_PLATFORM_XBOX)
    typedef lInputSystemXBOX       lInputSystem;
  #elif defined(L_PLATFORM_XBOX360)
    typedef lInputSystemXbox360    lInputSystem;
  #elif defined(L_PLATFORM_WINDOWS)
    typedef lInputSystemDX8        lInputSystem;
  #endif

END_L_NAMESPACE


#endif



