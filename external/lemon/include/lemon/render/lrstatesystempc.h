//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrstatesystempc.h
// Description : Win32 PC DirectX state system
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#if !defined(L_PLATFORM_WINDOWS)
#error lstatesystempc.h should only be included on Win32 PC platforms
#endif

#include <D3D9.H>

#ifndef __CTSTATESYSTEMPC_H__
#define __CTSTATESYSTEMPC_H__

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// StateSystem - Template state system
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename TSTATE>
class StateSystemPC
{
public:
  struct  StateBase;
  typedef lList<StateBase> State;

  struct StateBase
  {
    TSTATE       state;
    unsigned int newValue;
  };

  State *        m_stateUsedHead;
  State *        m_stateUnusedHead;
  State          m_stateTable[TSTATE::MAX_STATES];

  StateSystemPC()
  {
    m_stateUsedHead   = NULL;
    m_stateUnusedHead = NULL;

    for( int i=0;i<TSTATE::MAX_STATES;i++ )
    {
      State * state = &m_stateTable[i];
      state->state.type = (TSTATE::STATE_TYPE)i;
      state->Link( m_stateUnusedHead );
    }
  }

  void Reset()
  {
    // set all states to their default value

    for( int i=0;i<TSTATE::MAX_STATES;i++ )
    {
      State * state = &m_stateTable[i];
      state->state.value = state->state.GetDefault();
    }
  }

  // Grab a state from the main linked list into another linked list
  // Setting the 'newValue' field

  void GrabState( typename TSTATE::STATE_TYPE index, unsigned int value, State *& listHead )
  {
    lAssert( index >= 0 && index < TSTATE::MAX_STATES );
    State & state = m_stateTable[ index ];
    state.Unlink();
    state.Link( listHead );
    state.newValue = value;
  }

  // Reclaim states from another linked list

  void ReclaimStates( State *& listHead )
  {
    State * state = listHead;
    while( state )
    {
      State * nextState = state->ListNext;

      // set to default value

      state->Unlink();

      // unlink/link

      if( state->newValue != state->state.GetDefault() )
      {
        state->Link( m_stateUsedHead );
      }
      else
      {
        state->Link( m_stateUnusedHead );
      }

      state = nextState;
    }
  }

  // Apply states from another linked list, setting all other states
  // to their default value if bDefault is set.

  void Apply( State *& listHead, LPDIRECT3DDEVICE9 pD3DDevice, unsigned int stateStage, bool bDefault )
  {
    // remove remaining states, setting their default values

    if( bDefault )
    {
      State * state = m_stateUsedHead;
      while( state )
      {
        State * nextState = state->ListNext;

        // set value to default

        if( state->state.value != state->state.GetDefault() )
        {
          state->state.value = state->state.GetDefault();
          state->state.Set(pD3DDevice,stateStage);
        }

        // unlink/link

        state->Unlink();
        state->Link( m_stateUnusedHead );
        state = nextState;
      }
    }

    // add used states to the state list

    State * state = listHead;
    while( state )
    {
      State * nextState = state->ListNext;

      // set to new value

      // unlink/link

      state->Unlink();
      state->Link( m_stateUsedHead );

      // set to new value

      if( state->state.value != state->newValue )
      {
        state->state.value = state->newValue;
        state->state.Set(pD3DDevice,stateStage);
      }

      state = nextState;
    }
  }

  // Apply states from an array of states, setting all other states
  // to their default value if bDefault is set

  void Apply( lArray<TSTATE> &stateBlock, LPDIRECT3DDEVICE9 pD3DDevice, unsigned int stateStage, bool bDefault )
  {
    // remove used states from state list

    for( int i=0;i<stateBlock.Size();i++ )
    {
      State & state = m_stateTable[ stateBlock(i).type ];
      state.Unlink();
      state.Link( m_stateUnusedHead );
    }

    // remove remaining states, setting their default values

    if( bDefault )
    {
      State * state = m_stateUsedHead;
      while( state )
      {
        State * nextState = state->ListNext;

        // set value to default

        if( state->state.value != state->state.GetDefault() )
        {
          state->state.value = state->state.GetDefault();
          state->state.Set(pD3DDevice,stateStage);
        }

        // unlink/link

        state->Unlink();
        state->Link( m_stateUnusedHead );
        state = nextState;
      }
    }

    // add used states to the state list

    for( int i=0;i<stateBlock.Size();i++ )
    {
      const TSTATE & newState = stateBlock(i);
      State        & curState = m_stateTable[ stateBlock(i).type ];

      // set to new value

      // unlink/link

      curState.Unlink();
      curState.Link( m_stateUsedHead );

      // set to new value

      if( curState.state.value != newState.value )
      {
        curState.state = newState;
        curState.state.Set(pD3DDevice,stateStage);
      }
    }
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// RenderState - A render state
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LRENDER_API RenderState
{
  RenderState(D3DRENDERSTATETYPE type = (D3DRENDERSTATETYPE)0, unsigned int value = 0) : type(type), value(value)
  {
    lAssert( type >= 0 && type < MAX_STATES );
  }

  unsigned int GetDefault()                                     { return s_default[type]; }
  void Set( LPDIRECT3DDEVICE9 pD3DDevice, int /*nothing*/ )
  {
    //NOTE: Disable state system from overriding scissortestenable as we set it inside lRenderPC::SetScissor
    if( type != D3DRS_SCISSORTESTENABLE )
      pD3DDevice->SetRenderState( type, value );
  }

  enum { MAX_STATES = 210 };
  typedef D3DRENDERSTATETYPE STATE_TYPE;

  D3DRENDERSTATETYPE  type;
  unsigned int        value;
  static unsigned int s_default[MAX_STATES];
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SamplerState - A sampler state
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LRENDER_API SamplerState
{
  SamplerState(D3DSAMPLERSTATETYPE type = (D3DSAMPLERSTATETYPE)0, unsigned int value = 0) : type(type), value(value)
  {
    lAssert( type >= 0 && type < MAX_STATES );
  }

  unsigned int GetDefault()                                     { return s_default[type]; }
  void         Set( LPDIRECT3DDEVICE9 pD3DDevice, int sampler ) { pD3DDevice->SetSamplerState( sampler, type, value );  }

  enum { MAX_STATES = 14 };
  typedef D3DSAMPLERSTATETYPE STATE_TYPE;

  D3DSAMPLERSTATETYPE type;
  unsigned int        value;
  static unsigned int s_default[MAX_STATES];
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// TextureState - A texture state
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LRENDER_API TextureState
{
  TextureState(D3DTEXTURESTAGESTATETYPE type = (D3DTEXTURESTAGESTATETYPE)0, unsigned int value = 0) : type(type), value(value)
  {
    lAssert( type >= 0 && type < MAX_STATES );
  }

  unsigned int GetDefault()                                   { return s_default[type]; }
  void         Set( LPDIRECT3DDEVICE9 pD3DDevice, int stage ) { pD3DDevice->SetTextureStageState( stage, type, value ); }


  enum { MAX_STATES = 33 };
  typedef D3DTEXTURESTAGESTATETYPE STATE_TYPE;

  D3DTEXTURESTAGESTATETYPE type;
  unsigned int             value;
  static unsigned int      s_default[MAX_STATES];
};

END_L_NAMESPACE

#endif
