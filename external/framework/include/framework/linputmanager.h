//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : linputmanager.h
// Description : lInputManager
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.
//
//  TODO:
//    Add combo system
//    Textual representation of inputs
//    Finish deadzone
//
//    Only supports Win32, NDS, PS2, PSP, PC, WII as far as default controls are concerned

#ifndef _inputmanager_
#define _inputmanager_

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>
#include <lemon/input/linput.h>

BEGIN_L_NAMESPACE

// Need more concept names
enum InputConcept
{
  IC_NONE,

  IC_PrimaryXAxis,
  IC_PrimaryYAxis,

  IC_SecondaryXAxis,
  IC_SecondaryYAxis,

  IC_PrimaryButton1,
  IC_PrimaryButton2,
  IC_PrimaryButton3,
  IC_PrimaryButton4,
  IC_PrimaryButton5,
  IC_PrimaryButton6,
  IC_PrimaryButton7,
  IC_PrimaryButton8,

  IC_PrimaryStart,
  IC_SecondaryStart,

  IC_PrimaryBack,
  IC_SecondaryBack,

  IC_PrimaryMouseClick,
  IC_SecondaryMouseClick,

  // Could be used with buttons/dpad/analog instead of the mouse
  IC_PrimaryMouseXAxis,
  IC_PrimaryMouseYAxis,

  IC_SecondaryMouseXAxis,
  IC_SecondaryMouseYAxis,

  IC_PrimaryUp,
  IC_PrimaryDown,
  IC_PrimaryLeft,
  IC_PrimaryRight,
};

// Input states
enum {
  INPUT_NONE,
  INPUT_PRESSED,
  INPUT_HELD,
  INPUT_RELEASED,
};

const char *  KeyToString(lInputType type);
//int           GetNumberDevices();
//int           GetNumberDevicesConnected();

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// InputObject
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class InputObject
{
public:

  // Set default values
  InputObject() :
      m_state(INPUT_NONE),
      m_value(0),
      m_defaultClickTime(0.2f),
      m_clickTimer(0.2f),
      m_clickCounter(0),
      m_inverted(false)
  {
  }

  void  SetState(int state ) { m_state = state; };
  int   GetState() { return m_state; };

  bool  Pressed() {  return ( m_state == INPUT_PRESSED ); }
  bool  Held() { return ( m_state == INPUT_HELD ); }
  bool  Released() { return ( m_state == INPUT_RELEASED ); }

  void  SetValue( float value ) { m_value = value; };
  float GetValue();

  bool  Positive();
  bool  Negative();

  bool  FullyPositive();
  bool  FullyNegative();

  bool  FullyPressed() {  return ( FullyPositive() || FullyNegative() ); }

  int   GetClickCount() { return m_clickCounter; }
  void  SetDefaultClickTime( float time ) { m_defaultClickTime = time; }

  void  Invert() { m_inverted ^= true; };

//private:
  int           m_state;

  float         m_value;

  // repeat times
  float         m_defaultClickTime;
  float         m_clickTimer;
  int           m_clickCounter;

  bool          m_inverted;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Device
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// Deals with device specific actions such as retrieving value and processing
// state.
// TODO: Apply deadzone to GetValue so that it is still a value between -1 n 1

class Device
{
public:
  Device() :
    m_primaryKey(LI_NONE),
    m_secondaryKey(LI_NONE),
    m_deviceNumber(0),
    m_deadzone(0)
  {};

  Device( lInputType primarykey, int deviceNumber) :
    m_primaryKey(primarykey),
    m_secondaryKey(LI_NONE),
    m_deviceNumber(deviceNumber),
    m_deadzone(0)
  {};

  Device( lInputType primarykey, lInputType secondkey, int deviceNumber) :
    m_primaryKey(primarykey),
    m_secondaryKey(secondkey),
    m_deviceNumber(deviceNumber),
    m_deadzone(0)
  {};

  ~Device() {};

  void          Tick( float delta );

  InputObject & GetInput() { return m_inputObject; };

  void          SetInput( lInputType primarykey, lInputType secondkey = LI_NONE, int deviceNumber = 0 )
  {
    m_primaryKey = primarykey;
    m_secondaryKey = secondkey;
    m_deviceNumber = deviceNumber;
  }

private:

  lInputType    m_primaryKey;
  lInputType    m_secondaryKey;
  int           m_deviceNumber;

  InputObject   m_inputObject; // holds state information

  //deadzone
  float         m_deadzone;
};

// Map id to concept
struct InputMap
{
  InputConcept  concept;
  Device        input;
  int           id;
  int           player;
  bool          enabled;
  char          name[256];
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// MouseInputObject
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// --- Not sure this is really needed anymore, a new concept id could be used
// Currently only tested on win32, other platforms are not fully implemented
// - although nds should work
//
class MouseInputObject
{
public:

  MouseInputObject(int player, int controller);
  MouseInputObject(int player, int controller, lInputType xaxis, lInputType yaxis);

  ~MouseInputObject();

  void  Tick(float delta);

  float GetPositionX();
  float GetPositionY();

  float GetRelPositionX();
  float GetRelPositionY();

  int   GetPlayer() { return m_player; };
private:

  float m_positionX;
  float m_positionY;

  int   m_player;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// InputManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lInputManager
{
public:

  static void Init();

  static void DeInit();

  static void LoadDefaultControls();

  static void RegisterInput( int id, InputConcept input, const char* name, int player = 0 );
  static void AssignInput();

  static void ChangeConcept( InputConcept concept, lInputType primary, lInputType secondary = LI_NONE );


  static void                   AddMouse( MouseInputObject * object );
  static MouseInputObject *     GetMouse( int player );

  // Ask for input object
  static InputObject &          GetInput( int id, int player = 0 );

  // Swaps both ways
  static void                   SwapControls( int player_one, int player_two );

  static void                   SetEnableId( int id, bool bEnable);
  static void                   SetEnablePlayer( int player, bool bEnable);
  static void                   SetEnableAllInput( bool bEnable );

  static void                   Tick( float delta );

  static bool                   IsInit();
};

END_L_NAMESPACE

#endif

