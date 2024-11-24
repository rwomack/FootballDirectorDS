//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : main.cpp
// Description : ImplantSave
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

/*
 Modification History
 --------------------

 20/07/2008 - First version                                       - Barog

 Notes
 -----

*/

#include <lemon/platform/lcore.h>
#include <lemon/hash/lcrc32.h>
#include <stdio.h>

using namespace L_NAMESPACE_NAME;

void main( int argc, char *argv[] )
{
  if( argc < 3 )
  {
    printf("%1 <DS save filename> <game save filename>\n", argv[0]);
    printf("Implant game save into DS savefile\n");
    return;
  }

  FILE *fpIn  = fopen( argv[1], "rb" );
  FILE *fpIn2 = fopen( argv[2], "rb" );

  if( fpIn == NULL || fpIn2 == NULL )
  {
    printf("Couldn't open files for implant\n");
    return;
  }

  // read in existing data
  fseek( fpIn2, 0, SEEK_END );
  int outsize = ftell( fpIn2 );
  fseek( fpIn2, 0, SEEK_SET );

  char *curData = (char *)malloc( outsize );
  fread( curData, outsize, 1, fpIn2 );

  fclose( fpIn2 );

  // seek to start of data
  int outOffset = 9471+40+4+4;
  //fseek( fpOut, 9471+40+4+4, SEEK_SET );

  // read + write data
  int size = 0;
  unsigned int crc = 0;
  while( !feof( fpIn ) )
  {
    char data[1024];
    int read = fread( data, 1, 1024, fpIn );
    crc   = lCRC32( crc, data, read );
    memcpy( &curData[ outOffset ], data, read );
    outOffset += read;
    //fwrite( data, read, 1, fpOut );
    size += read;
  }

  // seek to start of crc(uint32), size(uint32), data
  outOffset = 9471+40;
  //fseek( fpOut, 9471+40, SEEK_SET );

  // write crc
  memcpy( &curData[ outOffset ], &crc, sizeof(crc) );
  outOffset += 4;
  //fwrite( &crc, sizeof(crc), 1, fpOut );
  memcpy( &curData[ outOffset ], &size, sizeof(size) );
  outOffset += 4;
  //fwrite( &size, sizeof(size), 1, fpOut );

  printf("%d bytes implanted - CRC %08x\n", size, crc);

  fclose( fpIn );

  // write new file
  FILE *fpOut = fopen( argv[2], "wb" );
  fwrite( curData, outsize, 1, fpOut );
  fclose( fpOut );
}


