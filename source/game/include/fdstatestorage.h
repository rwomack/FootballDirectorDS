//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstatestorage.h
// Description : FDStateStorage interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSTATESTORAGE_H_INCLUDED
#define FDSTATESTORAGE_H_INCLUDED

#include <lemon/lapi.h>

BEGIN_L_NAMESPACE

#define MAX_STATE_DATASIZE 64


class FDStateStorage
{
 public:
  FDStateStorage();
  ~FDStateStorage();

  void      Serialize( void *data, unsigned int size );
  void      Reset();
  int       Pos();
  int       Length();

  bool      IsLoading(void);
  bool      IsSaving(void);

  // initialise a load of this state data
  void      InitLoad();

  // initialise a save over this state data
  void      InitSave();

 private:

  unsigned int Read(void *data,unsigned int size);
  unsigned int Write(const void *data,unsigned int size);

 // data storage
  char  m_data[MAX_STATE_DATASIZE];
  int   m_dataPos;
  int   m_dataSize;

  bool  m_bLoading;
};

inline FDStateStorage& operator<< ( FDStateStorage& stream, bool & c )           { uint8 x = c ? 1 : 0; stream.Serialize( &x, 1 ); c = (x == 0) ? false : true; return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, uint8 & c )          { stream.Serialize( &c, 1 ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, int8 & c )           { stream.Serialize( &c, 1 ); return stream; }
//inline FDStateStorage& operator<< ( FDStateStorage& stream, char & c )           { stream.Serialize( &c, 1 ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, uint16 &c )          { stream.Serialize( &c, 2 ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, int16 & c )          { stream.Serialize( &c, 2 ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, uint32 &c )          { stream.Serialize( &c, sizeof(uint32) ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, int32 & c )          { stream.Serialize( &c, sizeof(int32) );  return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, uint64 & c )         { stream.Serialize( &c, sizeof(uint64) ); return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, int64 & c )          { stream.Serialize( &c, sizeof(int64) );  return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, float & c )          { stream.Serialize( &c, sizeof(float) );  return stream; }
inline FDStateStorage& operator<< ( FDStateStorage& stream, double & c )         { stream.Serialize( &c, sizeof(double) ); return stream; }


END_L_NAMESPACE
#endif

