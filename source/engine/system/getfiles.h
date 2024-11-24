//
// Header file for GetFiles.CPP
// GetFiles program - search a directory to get all the specified files
// given by a wildcard
//
// By Nick Tuckett, for R.W.Games

#ifndef __GETFILES_H__
#define __GETFILES_H__

// This is the structure used to build a file list. FileAttributes is
// identical in structure to that in WIN32_FIND_DATA (see help file)
typedef struct TFileData
{
	char	   FileName [13];
	DWORD	   FileAttributes;
	TFileData *Next;
} TFileData;

extern int FindFiles(CString Wildcard, TFileData **FileList);
extern void FreeFileList(TFileData *FileList);

#endif
