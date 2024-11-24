//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// File        : ltreenode.h
// Description : Tree node template class
// Notes       :
//
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LTREENODE_H_INCLUDED
#define LTREENODE_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// lTreeNode
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
//
// Generic tree node using the curiously reoccurring template pattern.
//
// class MyNode : public lTreeNode<MyNode>
// {
// };
//

template< class T >
class lTreeNode
{
public:
  lTreeNode();
  ~lTreeNode();

  void    SetParent( T * );
  T     * GetParent() const;

  void    DetachFromTree();

  void    InsertAfter( T * );
  void    InsertBefore( T * );

  T     * FirstChild() const;
  T     * LastChild() const;
  T     * NextSibling() const;
  T     * PrevSibling() const;

  bool    IsDescendantOf( const T * ) const;

  T     * NextNode() const; // walk tree
  T     * NextLeaf() const; // walk tree leafs

private:
  T * m_parent;
  T * m_nextSibling;
  T * m_firstChild;

  void RemoveFromParent();
};

//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// lTreeNode implementation
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// lTreeNode constructor
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
lTreeNode<T>::lTreeNode()
  : m_parent( static_cast<T*>(0) ),
    m_nextSibling( static_cast<T*>(0) ),
    m_firstChild( static_cast<T*>(0) )
{
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// lTreeNode destructor
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
lTreeNode<T>::~lTreeNode()
{
  // detach from our parent

  RemoveFromParent();

  // delete our children

  T * child = m_firstChild;
  m_firstChild = static_cast<T*>(0);

  while( child )
  {
    T * nextSibling = child->m_nextSibling;

    // detach the child here,
    // we don't need to maintain the sibling list

    child->m_parent      = static_cast<T*>(0);
    child->m_nextSibling = static_cast<T*>(0);

    // delete the child

    delete child;

    // next child

    child = nextSibling;
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Set the parent of the node
// SetParent
//
// In:
//   newParent - new parent for the node, or NULL.
//
// Out:
//   NONE
//
// Notes:
//   The node will be removed from its current parent (if any) and
//   inserted at the head of the child list for the new parent.
//   If the new parent is NULL, the node will just be remove from its
//   current parent.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
void lTreeNode<T>::SetParent( T * newParent )
{
	RemoveFromParent();

  m_parent = newParent;

  if( m_parent )
  {
    m_nextSibling          = m_parent->m_firstChild;
    m_parent->m_firstChild = static_cast<T*>(this);
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Insert the node into a tree after the specified sibling
// InsertAfter
//
// In:
//   prevSibling - sibling to insert the node after
//
// Out:
//   NONE
//
// Notes:
//   The node will be removed from its current parent and then inserted
//   into the new parents child list after the specified sibling.
//   If the specified sibling *must* have a parent.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
void lTreeNode<T>::InsertAfter( T * prevSibling )
{
  lAssert( prevSibling != static_cast<T*>(0) );
  lAssert( prevSibling->m_parent != static_cast<T*>(0) );

  if( prevSibling == static_cast<T*>(this) )
  {
    // no movement, exit now.
    // otherwise we'll remove ourselves then try and re-attach to a NULL parent.
    return;
  }

	RemoveFromParent();

  m_parent = prevSibling->m_parent;

  lAssert( m_parent != static_cast<T*>(0) );

  if( m_parent )
  {
    m_nextSibling              = prevSibling->m_nextSibling;
    prevSibling->m_nextSibling = static_cast<T*>(this);
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Insert the node into a tree before the specified sibling
// InsertBefore
//
// In:
//   nextSibling - sibling to insert the node before
//
// Out:
//   The node will be removed from its current parent and then inserted
//   into the new parents child list before the specified sibling.
//   If the specified sibling *must* have a parent.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
void lTreeNode<T>::InsertBefore( T * nextSibling )
{
  lAssert( nextSibling != static_cast<T*>(0) );
  lAssert( nextSibling->m_parent != static_cast<T*>(0) );

  if( nextSibling == static_cast<T*>(this) )
  {
    // no movement, exit now.
    // otherwise we'll remove ourselves then try and re-attach to a NULL parent.
    return;
  }

	RemoveFromParent();

  m_parent = nextSibling->m_parent;

  lAssert( m_parent != static_cast<T*>(0) );

  if( m_parent )
  {
    if( nextSibling == m_parent->m_firstChild )
    {
      // nextSibling is the first child of the parent node,
      // so insert ourselves at the head of child list

      m_nextSibling          = m_parent->m_firstChild;
      m_parent->m_firstChild = static_cast<T*>(this);
    }
    else
    {
      // get the node before the next sibling and
      // insert ourselves between them

      T * prevSibling = nextSibling->PrevSibling();

      lAssert( prevSibling != static_cast<T*>(0) );
      lAssert( prevSibling->m_nextSibling == nextSibling );

      m_nextSibling              = nextSibling;
      prevSibling->m_nextSibling = static_cast<T*>(this);
    }
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Detach the node from the tree, re-rooting any children to its parent
// DetachFromTree
//
// In:
//   NONE
//
// Out:
//   NONE
//
// Notes:
//   The node will be isolated with no parent or children.
//   Any children of the node will be attached to the current parent.
//   If the node has no parent, the children will be left without a parent.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
void lTreeNode<T>::DetachFromTree()
{
  T * oldParent = m_parent;

	RemoveFromParent();

  while( m_firstChild )
  {
    m_firstChild->SetParent( oldParent );
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the parent of this node
// GetParent
//
// In:
//   NONE
//
// Out:
//   Returns the parent of the node, or NULL if it is a root node.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::GetParent() const
{
  return m_parent;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the first child of this node
// FirstChild
//
// In:
//   NONE
//
// Out:
//   Returns the first child of this node, or NULL if there are no children.
//
// Notes:
//   Use child->NextSibling() to iterate over the childen.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::FirstChild() const
{
  return m_firstChild;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the last child of this node
// LastChild
//
// In:
//   NONE
//
// Out:
//   Returns the last child of this node, or NULL if there are no children.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::LastChild() const
{
  if( m_firstChild == static_cast<T*>(0) )
  {
    return static_cast<T*>(0);
  }

  T * node = m_firstChild;

  while( node->m_nextSibling )
  {
    node = node->m_nextSibling;
  }

  return node;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the next sibling of this node
// NextSibling
//
// In:
//   NONE
//
// Out:
//   Returns the next sibling of this node, or NULL if it is the last
//   sibling, or has no siblings.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::NextSibling() const
{
  return m_nextSibling;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the previous sibling of this node
// PrevSibling
//
// In:
//   NONE
//
// Out:
//   Returns the previous sibling of this node, or NULL if it is the first
//   sibling, or has no siblings.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::PrevSibling() const
{
  if( m_parent == static_cast<T*>(0) || m_parent->m_firstChild == static_cast<const T*>(this) )
  {
    // no parent, or we are the first child.
    return static_cast<T*>(0);
	}

  T * node = m_parent->m_firstChild;
  T * prev = static_cast<T*>(0);

  while( ( node != static_cast<const T*>(this) ) && ( node != static_cast<T*>(0) ) )
  {
		prev = node;
    node = node->m_nextSibling;
	}

  lAssert( node == static_cast<const T*>(this) ); // otherwise, not in the parents child list

	return prev;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Check if the node is a direct or indirect descendant of the given parent
// IsDescendantOf
//
// In:
//   parent - parent to check node against
//
// Out:
//   Returns true if the specified parent is a parent of this node.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
bool lTreeNode<T>::IsDescendantOf( const T * parent ) const
{
  const T * node = m_parent;

  while( node )
  {
    if( node == parent )
    {
      return true;
    }

    node = node->m_parent;
  }

  return false;
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the next node in the tree, searching depth first
// NextNode
//
// In:
//   NONE
//
// Out:
//   Returns the next node in the tree.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T * lTreeNode<T>::NextNode() const
{
  if( m_firstChild )
  {
    return m_firstChild;
  }

  const T * node = static_cast<const T*>(this);

  while( node && node->m_nextSibling == NULL )
  {
    node = node->m_parent;
  }

  if( node )
  {
    return node->m_nextSibling;
  }

  return static_cast<T*>(0);
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Get the next leaf in the tree, searching depth first
// NextLeaf
//
// In:
//   NONE
//
// Out:
//   Returns the next leaf in the tree.
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
T *lTreeNode<T>::NextLeaf() const
{
  if( m_firstChild )
  {
    T *node = m_firstChild;

    while( node->m_firstChild )
    {
      node = node->m_firstChild;
		}

    return node;
  }

  const T * node = static_cast<const T*>(this);

  while( node && node->m_nextSibling == NULL )
  {
    node = node->m_parent;
  }

  if( node )
  {
    T * nextNode = node->m_nextSibling;

    while( nextNode->m_firstChild )
    {
      nextNode = nextNode->m_firstChild;
    }

    return nextNode;
  }

  return static_cast<T*>(0);
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Removes the node from the parent
// RemoveFromParent
//
// In:
//   NONE
//
// Out:
//   NONE
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
template< class T >
void lTreeNode<T>::RemoveFromParent()
{
  if( m_parent )
  {
    T * prev = PrevSibling();

    if( prev )
    {
      prev->m_nextSibling = m_nextSibling;
    }
    else
    {
      m_parent->m_firstChild = m_nextSibling;
		}
	}

  m_parent      = static_cast<T*>(0);
  m_nextSibling = static_cast<T*>(0);
}

END_L_NAMESPACE
#endif
