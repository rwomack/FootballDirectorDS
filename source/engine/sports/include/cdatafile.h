#ifndef CDATAFILE_H_INCLUDED
#define CDATAFILE_H_INCLUDED

class CDataFile
{
public:
  CDataFile();
  virtual ~CDataFile();

  virtual bool         IsValid();

  virtual bool         Reading(void);
  virtual bool         Writing(void);
  virtual unsigned int Read(void *,unsigned int) = 0;
  virtual unsigned int Write(const void *,unsigned int)  = 0;

  void EnterBitMode(const bool bWriting);
  void ExitBitMode();
  unsigned int ReadBits(const unsigned int uiNumBits);
  unsigned int WriteBits(const unsigned int uiBitsToWrite, const unsigned int uiNumBits);
//  virtual unsigned int ReadBitValue( int numBits ) = 0;
//  virtual unsigned int WriteBitValue( int val, int numBits ) = 0;

  virtual unsigned int Access(void *,unsigned int);

  virtual void         Debug( const char *str = "" ) {}
private:
  bool m_bInBitMode : 1;
  unsigned int m_uiBitBuffer;
  unsigned int m_uiBitCounter;
};

#endif // #ifndef CDATAFILE_H_INCLUDED
