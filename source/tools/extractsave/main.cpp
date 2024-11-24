//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : main.cpp
// Description : ExtractSave
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

/*
 Modification History
 --------------------

 05/06/2008 - First version                                       - Simon

 Notes
 -----

*/

#include <lemon/platform/lcore.h>
#include <stdio.h>

using namespace L_NAMESPACE_NAME;

void main( int argc, char *argv[] )
{
  if( argc < 3 )
  {
    printf("%1 <R4 save filename> <game save filename>\n", argv[0]);
    printf("Extract game.save from R4 savefile\n");
    return;
  }

  FILE *fpIn  = fopen( argv[1], "rb" );
  FILE *fpOut = fopen( argv[2], "wb" );

  if( fpIn == NULL || fpOut == NULL )
  {
    printf("Couldn't open files for extract\n");
    return;
  }

  // skip to data
  fseek( fpIn, 9471+40+4+4, SEEK_SET );

  int size = 0;
  while( !feof( fpIn ) )
  {
    char data[1024];
    int read = fread( data, 1, 1024, fpIn );
    fwrite( data, read, 1, fpOut );
    size += read;
  }

  printf("%d bytes extracted\n", size);

  fclose( fpIn );
  fclose( fpOut );
}


