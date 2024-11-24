//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmenu.h
// Description : Menu hierarchy navigation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMENU_H_INCLUDED
#define LMENU_H_INCLUDED

#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMenuNode
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lMenuNode
{
public:
  lMenuNode( int scriptId, int helpId, int numStates, int menuIdx, lMenuNode * child = NULL )
  {
    m_scriptId = scriptId;
    m_helpId = helpId;
    m_menuIdx = menuIdx;
    m_numStates = numStates;
    m_currentState = 0;
    m_currentChild = 0;
    m_parent = NULL;
    m_bHide = false;
    m_bVolumeBars = false;

    if( child != NULL )
    {
      AddChild( child );
    }
  }

  int                     m_scriptId;
  int                     m_helpId;
  int                     m_numStates;
  int                     m_currentState;
  int                     m_currentChild;
  int                     m_menuIdx;
  lArray<lMenuNode*>      m_childList;
  lMenuNode*              m_parent;
  bool                    m_bHide;
  bool                    m_bVolumeBars;//display multiple state items as a set of bars

  void AddChild( lMenuNode *child )
  {
    m_childList.Push( child );
    child->m_parent = this;
  }

  bool IsLeafNode()
  {
    if( m_childList.Size() == 0 ) return true;
    return false;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMenu
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
enum lMenuAction
{
  LMENU_ACTION_SELECT,
  LMENU_ACTION_BACK,
  LMENU_ACTION_UP,
  LMENU_ACTION_DOWN,
  LMENU_ACTION_LEFT,
  LMENU_ACTION_RIGHT,
};

class lMenu
{
public:
  lMenu();
  ~lMenu();

  void SetNode(lMenuNode* node);
  lMenuNode* GetCurrentNode();
  lMenuNode* GetLastNode();

  // search from m_currentNode and below for the nodeID specified
  lMenuNode* GetNodeById( int nodeId );

  void DoAction( lMenuAction );

private:
  lMenuNode* FindNodeById( lMenuNode *curNode, int nodeId );

  lMenuNode * m_currentNode;
  lMenuNode * m_lastNode;
  int         m_branchIdx;
  bool        m_bLoopChildren;
};

END_L_NAMESPACE

#endif


