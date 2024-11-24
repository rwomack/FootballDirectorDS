//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : llist.h
// Description : List template class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//class lArchive;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lList - Doubly linked list template
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Doubley Linked List template
//
template<class T> class lList : public T
{
public:
  lList() : ListNext(NULL), ListPrevLink(NULL) {}

  lList *  ListNext;
  lList ** ListPrevLink;

  inline void Unlink()
  {
    if( ListNext ) ListNext->ListPrevLink = ListPrevLink;
    *ListPrevLink = ListNext;
  }

  inline void Link( lList*& Prev )
  {
    if( Prev )     Prev->ListPrevLink = &ListNext;
    ListNext     = Prev;
    ListPrevLink = &Prev;
    Prev         = this;
  }

  //friend lArchive& operator<< ( lArchive &ar, lList<T> &c )
  //{
  //  return T::operator<<(ar,*(T *)&c); }
  //}
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lListLink - Non template linked list
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lLinkedList;

class lListLink
{
public:
  lListLink() : m_next(0), m_prev(0) {}
  lListLink(lListLink *next, lListLink *prev) : m_next(next), m_prev(prev) {}

  bool LinkIsHead() const { return (m_prev == 0); }
  bool LinkIsTail() const { return (m_next == 0); }

  lListLink *LinkNext() const { return m_next; }
  lListLink *LinkPrev() const { return m_prev; }

  void LinkInsertBefore(lListLink *link)
  {
    LinkAttachAfter(link->m_prev);
    LinkAttachBefore(link);
  }

  void LinkInsertAfter(lListLink *link)
  {
    LinkAttachBefore(link->m_next);
    LinkAttachAfter(link);
  }

  void LinkRemove()
  {
    m_next->m_prev = m_prev;
    m_prev->m_next = m_next;
		m_next = m_prev = 0;
  }

private:
  lListLink  *m_next;
  lListLink  *m_prev;

  inline void LinkAttachBefore(lListLink *link)
  {
		m_next       = link;
		link->m_prev = this;
	}

  inline void LinkAttachAfter(lListLink *link)
  {
		m_prev       = link;
		link->m_next = this;
  }

  friend class lLinkedList;
};

class lLinkedList
{
public:
  lLinkedList() : m_head(&m_tail, 0), m_tail(0, &m_head) {}

  lListLink *GetHead() const { return m_head.LinkNext(); }
  lListLink *GetTail() const { return m_tail.LinkPrev(); }

  void Empty()                   { m_head.LinkAttachBefore(&m_tail); }
  void AddHead(lListLink *link) { link->LinkInsertAfter(&m_head); }
  void AddTail(lListLink *link) { link->LinkInsertBefore(&m_tail); }

  void AppendHead(lLinkedList& list)
  {
    list.GetTail()->LinkAttachBefore(GetHead());
    list.GetHead()->LinkAttachAfter(&m_head);
    list.Empty();
  }

  void AppendTail(lLinkedList& list)
  {
    list.GetHead()->LinkAttachAfter(GetTail());
    list.GetTail()->LinkAttachBefore(&m_tail);
    list.Empty();
  }

  bool IsEmpty() const { return GetHead()->LinkIsTail(); }

private:
  lListLink m_head;
  lListLink m_tail;
};



END_L_NAMESPACE
#endif
