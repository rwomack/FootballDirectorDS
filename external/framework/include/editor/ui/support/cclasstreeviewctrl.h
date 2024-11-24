//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : cclasstreeviewctrl.h
// Description : lClass tree view
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CCLASSTREEVIEWCTRL_H_INCLUDED
#define EDITOR_UI_SUPPORT_CCLASSTREEVIEWCTRL_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>
#include <lemon/object/lobject.h>
#include <lemon/containers/larray.h>

namespace WTL
{

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CClassTreeViewCtrl
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class TBase>
class CClassTreeViewCtrlT : public CTreeViewCtrlExT<TBase>
{
  // CClassTreeViewCtrl implementation

private:
  struct Item
  {
    CTreeItem       m_treeItem;
    Lemon::lClass * m_class;
    bool            m_bIsFiltered;
  };

private:
  Item * FindItem( Lemon::lClass * objectClass )
  {
    for( int i=0;i<m_itemList.Size();i++ )
    {
      if( m_itemList(i).m_class == objectClass ) return &m_itemList(i);
    }
    return NULL;
  }

  Lemon::lClass * FindLeaf( CTreeItem item )
  {
    for( int i=0;i<m_itemList.Size();i++ )
    {
      if( m_itemList(i).m_treeItem == item )
      {
        if( m_itemList(i).m_bIsFiltered ) return NULL;
        return m_itemList(i).m_class;
      }
    }
    return NULL;
  }

  bool IsClassPrivate( Lemon::lClass * pClass )
  {
    // TODO.
    return false;
  }

  void RecursePopulate(Lemon::lClass * currentClass)
  {
    bool isValid = m_filterList.Find( currentClass, NULL );
    if( m_filterList.Size() == 0 ) isValid = true;
    if( !isValid ) return;

    int index = m_itemList.Add();
    m_itemList(index).m_class = currentClass;
    m_itemList(index).m_bIsFiltered = !isValid;

    // if this class is private to the game, don't add it

    if( ! IsClassPrivate(currentClass) )
    {
      // find the first non-private super-class, which we'll
      // use as the 'assumed' parent.

      Lemon::lClass * superClass = currentClass->SuperClass();

      while( superClass && IsClassPrivate( superClass ) )
      {
        superClass = superClass->SuperClass();
      }

      if( superClass == NULL )
      {
        m_itemList(index).m_treeItem = InsertItem( currentClass->ClassName(), 0,0  );
      }
      else
      {
        Item * item = FindItem( superClass );

        if( item == NULL )
        {
          m_itemList(index).m_treeItem = InsertItem( currentClass->ClassName(), 0,0  );
        }
        else
        {
          m_itemList(index).m_treeItem = InsertItem( currentClass->ClassName(), item->m_treeItem, 0  );
        }
      }
    }

    Lemon::lClass * nextClass = Lemon::lClass::FirstClass();
    while( nextClass )
    {
      if( nextClass->SuperClass() == currentClass )
      {
        RecursePopulate( nextClass );
      }

      nextClass = nextClass->NextClass();
    }
  }

public:
  void PopulateTree(Lemon::lClass * rootClass)
  {
    ClearTree();

    RecursePopulate( rootClass );

    if( m_itemList.Size() > 0 )
    {
      m_itemList(0).m_treeItem.Expand(TVE_EXPAND);
    }
  }
  void ClearTree()
  {
    DeleteAllItems();
    m_itemList.Empty();
  }

  void AddFilter( Lemon::lClass * objClass )
  {
    int index;
    if( m_filterList.Find( objClass, &index ) )
    {
      return;
    }
    index = m_filterList.Add();
    m_filterList(index) = objClass;
  }

  void RemoveFilter( Lemon::lClass * objClass )
  {
    int index;
    if( m_filterList.Find( objClass, &index ) )
    {
      m_filterList.Remove( index );
    }
  }

  void ClearFilters()
  {
    m_filterList.Empty();
  }

  Lemon::lClass * GetSelectedClass()
  {
    return FindLeaf( GetSelectedItem() );
  }

private:
  Lemon::lArray<Item>            m_itemList;
  Lemon::lArray<Lemon::lClass *> m_filterList;
};

typedef CClassTreeViewCtrlT<CWindow> CClassTreeViewCtrl;

}
#endif



