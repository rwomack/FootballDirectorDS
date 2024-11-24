#include "stdafx.h"
#include "KResource.h"

CResource::CResource()
{
	Handle		   = INVALID_HANDLE_VALUE;
	DirectoryDepth = 0;
}

CResource::~CResource()
{
}

BOOL CResource::Open(wchar_t *pFilename)
{
	unsigned long	BytesRead;

	// Attempt to open the resource
	Handle = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Return an error if unsuccessful
	if (Handle == INVALID_HANDLE_VALUE) return FALSE;

	// Obtain the size of the resource
	FileSize = GetFileSize(Handle, NULL);

	// Seek to the start of the resource
	SetFilePointer(Handle, 0, NULL, FILE_BEGIN);

	// Read the offset to the directory structure
	ReadFile(Handle, &DirectoryOffset, sizeof(unsigned int), &BytesRead, NULL);

	// Allocate memory for the directory structure
	pDirectory = reinterpret_cast<CDirectoryEntry *>(new unsigned char[FileSize - DirectoryOffset]);

	if (!pDirectory)
	{
		Close();

		return FALSE;
	}

	// Seek to the start of the directory structure
	SetFilePointer(Handle, DirectoryOffset, NULL, FILE_BEGIN);

	// Read in the directory structure
    ReadFile(Handle, pDirectory, FileSize - DirectoryOffset, &BytesRead, NULL);

	if (BytesRead != (FileSize - DirectoryOffset))
	{
		Close();

		return FALSE;
	}

	pDirectoryMarker[0] = pDirectory;

	// Initialise the directory entries
	InitDirectoryEntry(pDirectory, reinterpret_cast<unsigned int>(pDirectory));

	return TRUE;
}

void CResource::Close()
{
	if (pDirectory)
	{
		delete [] pDirectory;
		pDirectory = NULL;
	}

	if (Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(Handle);
		Handle = INVALID_HANDLE_VALUE;
	}
}

void CResource::InitDirectoryEntry(CDirectoryEntry *pDirectoryEntry, unsigned int BaseAddress)
{
	// See if we have a child entry
	if (pDirectoryEntry->GetChild() != reinterpret_cast<CDirectoryEntry *>(-1))
	{
		// Change the offset of the child entry into a pointer
		pDirectoryEntry->SetChild(reinterpret_cast<CDirectoryEntry *>(reinterpret_cast<unsigned int>(pDirectoryEntry->GetChild()) + BaseAddress));

		// Recurse into the child entry
		InitDirectoryEntry(pDirectoryEntry->GetChild(), BaseAddress);
	}

	// See if we have a sibling entry
	if (pDirectoryEntry->GetSibling() != reinterpret_cast<CDirectoryEntry *>(-1))
	{
		// Change the offset of the sibling entry into a pointer
		pDirectoryEntry->SetSibling(reinterpret_cast<CDirectoryEntry *>(reinterpret_cast<unsigned int>(pDirectoryEntry->GetSibling()) + BaseAddress));

		// Recurse into the sibling entry
		InitDirectoryEntry(pDirectoryEntry->GetSibling(), BaseAddress);
	}
}

BOOL CResource::ChangeDirectory(wchar_t *pDirectoryName)
{
	// Return an error if directory name is null or the resource isn't open
	if (!pDirectoryName || (Handle == INVALID_HANDLE_VALUE)) return FALSE;

	// Attempt to seek to the specified directory, returning if unsuccessful
	if (!SeekFile(pDirectoryName, SEEK_MODE_DIRECTORY | SEEK_MODE_STORE_PATH)) return FALSE;

	// Return successful
	return TRUE;
}

CDirectoryEntry *CResource::SeekFile(wchar_t *pName, int Mode)
{
	unsigned int	i;
	unsigned int	Depth = DirectoryDepth;
	CDirectoryEntry	*pFile;
	CDirectoryEntry	*pMarker[MAX_NESTED_DIRECTORIES];
	wchar_t			Root[] = L"\\";

	// Return an error if name is null or the resource isn't open
	if (!pName || (Handle == INVALID_HANDLE_VALUE)) return FALSE;

	// Record the current directory markers, so that if the change directory fails
	// the current directory markers will not be corrupted
	for (i = 0 ; i < Depth + 1 ; i++)
	{
		pMarker[i] = pDirectoryMarker[i];
	}

	// See if the first character of the directory name is a back slash
	if (*pName == L'\\')
	{
		// Yes, so move to root directory(depth 0, offset 0)
		Depth = 0;
		pMarker[0] = pDirectory;

		// Skip the processed slash
		pName++;
	}

	// Continue until the directory string ends
	while(*pName != L'\0')
	{
		int				j = 0;
		wchar_t			DirectoryName[MAX_PATH];
		CDirectoryEntry	*pDirectoryEntry;

		// Check for moving up a directory
		if ((*pName == L'.') && (*(pName + 1) == L'.'))
		{
			// Move up a directory if we can
			if (Depth > 0) Depth--;

			// Skip the '..' and the expected slash after it
			pName += 3;

			// Return to the start of the while loop
			continue;
		}

		// Continue until the character is a back slash or terminator
		while((*pName != L'\\') && (*pName != L'\0'))
		{
			// Build up the directory name
			DirectoryName[j++] = *(pName++);
		}

		// Terminate the directory name
		DirectoryName[j] = '\0';

		// Could have exited the while loop with a slash or terminator
		// If it was a slash, then skip it
		if (*pName == L'\\') pName++;

		// Get a pointer to the current directory position
		pDirectoryEntry = pMarker[Depth];

		while(1)
		{
			// If we have a match, break out of the loop
			if (!wcsicmp(pDirectoryEntry->GetName(), DirectoryName)) break;

			// See if we have hit the end of the current directory
			if (pDirectoryEntry->GetSibling() == reinterpret_cast<CDirectoryEntry *>(-1))
			{
				// No more entries, return an error
				return NULL;
			}
			else
			{
				// Step to the next entry
				pDirectoryEntry = pDirectoryEntry->GetSibling();
			}
		}

		// Store the new offset for the current directory
		pMarker[++Depth] = pDirectoryEntry->GetChild();

		// Always store the current entry as the file entry(as this may be the last level)
		pFile = pDirectoryEntry;
	}

	if (Mode & SEEK_MODE_STORE_PATH)
	{
		if (Mode & SEEK_MODE_DIRECTORY)
		{
			// We can store all the entries we found(all are directories)
			DirectoryDepth = Depth;

			// Scan through all the temporary entries
			for (i = 0 ; i < Depth + 1; i++)
			{
				// Update the real directory markers
				pDirectoryMarker[i] = pMarker[i];
			}
		}
		else
		{
			// Store all but the last entry(last = file, rest = directories)
			DirectoryDepth = Depth - 1;

			// Scan through all the temporary entries
			for (i = 0 ; i < Depth ; i++)
			{
				// Update the real directory markers
				pDirectoryMarker[i] = pMarker[i];
			}
		}
	}

	// Return a pointer to the directory or a file
	if (Mode & SEEK_MODE_DIRECTORY)
	{
		// Directory
		return pMarker[Depth];
	}
	else
	{
		// File
		return pFile;
	}
}

void *CResource::Load(wchar_t *pFilename)
{
	CDirectoryEntry		*pDirectoryEntry;
	void				*pFile;
	unsigned int		FileSize;
	unsigned long		BytesRead;

	// Return an error if name is null or the resource isn't open
	if (!pFilename || (Handle == INVALID_HANDLE_VALUE)) return FALSE;

	// Get info on the requested file
	pDirectoryEntry = SeekFile(pFilename, SEEK_MODE_FILE);

	// Return an error if we can't get the info
	if (!pDirectoryEntry) return NULL;

	// Store the file size
	FileSize = pDirectoryEntry->GetSize();

	// Allocate memory for the file
	pFile = new unsigned char[FileSize];

	// Return an error if the allocation failed
	if (!pFile) return NULL;

	// Move to the files position within the resource
	SetFilePointer(Handle, pDirectoryEntry->GetOffset(), 0, FILE_BEGIN);

	// Read in the file
	ReadFile(Handle, pFile, FileSize, &BytesRead, NULL);

	// If there was an error loading the file, return an error
	if (BytesRead != FileSize)
	{
		delete [] pFile;

		return NULL;
	}

	// Return a pointer to the loaded file
	return pFile;
}

BOOL CResource::LoadAt(wchar_t *pFilename, void *pBuffer)
{
	CDirectoryEntry		*pDirectoryEntry;
	unsigned int		FileSize;
	unsigned long		BytesRead;

	// Return an error if name or buffer is null or the resource isn't open
	if (!pFilename || !pBuffer || (Handle == INVALID_HANDLE_VALUE)) return FALSE;

	// Get info on the requested file
	pDirectoryEntry = SeekFile(pFilename, SEEK_MODE_FILE);

	// Return an error if we can't get the info
	if (!pDirectoryEntry) return FALSE;

	// Store the file size
	FileSize = pDirectoryEntry->GetSize();

	// Move to the files position within the resource
	SetFilePointer(Handle, pDirectoryEntry->GetOffset(), 0, FILE_BEGIN);

	// Read in the file
	ReadFile(Handle, pBuffer, FileSize, &BytesRead, NULL);

	// If there was an error loading the file, return an error
	if (BytesRead != FileSize) return FALSE;

	// Return successful
	return TRUE;
}

// Seeks to the specified file, and returns the size
unsigned int CResource::SeekToFile(wchar_t *pFilename)
{
	CDirectoryEntry		*pDirectoryEntry;

	// Return an error if name is null or the resource isn't open
	if (!pFilename || (Handle == INVALID_HANDLE_VALUE)) return 0;

	// Get info on the requested file
	pDirectoryEntry = SeekFile(pFilename, SEEK_MODE_FILE);

	// Return an error if we can't get the info
	if (!pDirectoryEntry) return 0;

	SetFilePointer(Handle, pDirectoryEntry->GetOffset(), 0, FILE_BEGIN);
	// Return the file size
	return pDirectoryEntry->GetSize();
}

// Reads in the specified number of bytes from the current file
BOOL CResource::Read(void *pBuffer, unsigned int Elements, unsigned int ElementSize)
{
	unsigned long	BytesRead;

	// Return an error if buffer is null or the resource isn't open
	if (!pBuffer || (Handle == INVALID_HANDLE_VALUE)) return FALSE;

	// Read in the specified number of bytes
	ReadFile(Handle, pBuffer, Elements * ElementSize, &BytesRead, NULL);

	// If there was an error loading the file, return an error
	if (BytesRead != Elements * ElementSize) return FALSE;

	// Return successful
	return TRUE;
}

// Obtains the file / directory name
wchar_t *CDirectoryEntry::GetName()
{
	static wchar_t	WideCharName[MAX_PATH];

	// Convert from multi byte to wide chars
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, WideCharName, MAX_PATH);

	return WideCharName;
}
