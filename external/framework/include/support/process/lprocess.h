//����������������������������������������������������������������������������
// File        : lprocess.h
// Description : System process spawning
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LPROCESS_H_INCLUDED
#define LPROCESS_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/platform/lcore.h>
#include <lemon/containers/lstring.h>
#include <lemon/containers/larray.h>
#include <lemon/io/liostream.h>

BEGIN_L_NAMESPACE

namespace lProcess {

class ExecSpool;

//����������������������������������������������������������������������������
// Execute a system process
//����������������������������������������������������������������������������

int Execute( ExecSpool * spool, const char * command );

//����������������������������������������������������������������������������
// ExecSpool
//����������������������������������������������������������������������������

class ExecSpool
{
public:
  virtual ~ExecSpool() {}

  virtual void Start() = 0;
  virtual void Part(const char * buffer) = 0;
  virtual void End() = 0;
};

//����������������������������������������������������������������������������
// ExecSpoolLine
//����������������������������������������������������������������������������

class ExecSpoolLine : public ExecSpool
{
  lString m_bufferedLine;

public:
  ExecSpoolLine();
  virtual ~ExecSpoolLine();

  // ExecSpool interface

  virtual void Start();
  virtual void Part(const char * buffer);
  virtual void End();

  // ExecSpoolLine interface

  virtual void ProcessLine( const char * line ) = 0;
};

//����������������������������������������������������������������������������
// ExecSpoolCaptureLine
//����������������������������������������������������������������������������

class ExecSpoolCaptureLine : public ExecSpoolLine
{
  lArray<lString> m_lineList;

public:
  ExecSpoolCaptureLine();
  virtual ~ExecSpoolCaptureLine();

  // ExecSpoolLine interface

  virtual void ProcessLine( const char * line );

  // ExecSpoolCaptureLine

  void         Print();
  int          GetNumLines();
  const char * GetLine( int index );
};

//����������������������������������������������������������������������������
// ExecSpoolCaptureStream
//����������������������������������������������������������������������������

class ExecSpoolCaptureStream : public ExecSpool, public lIOStreamSeekable
{
  lArray<char> m_data;
  int          m_index;

public:
  ExecSpoolCaptureStream();
  virtual ~ExecSpoolCaptureStream();

  // ExecSpool interface

  virtual void Start();
  virtual void Part(const char * buffer);
  virtual void End();

  // lIOStreamSeekable interface

  virtual unsigned int Read(void *data,unsigned int size);
  virtual unsigned int Write(const void *data,unsigned int size);
  virtual unsigned int Seek(int offset,L_IO_SEEK whence);
  virtual unsigned int Tell(void);
  virtual unsigned int Length(void);
  virtual bool         Reading(void);
  virtual bool         Writing(void);

  // ExecSpoolCaptureStream interface

  lArray<char> & GetData();
};

} // lProcess

END_L_NAMESPACE

#endif

