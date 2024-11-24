#ifndef _RESOURCE_HEADER
#define _RESOURCE_HEADER

class CDirectoryEntry
{
public:
	CDirectoryEntry()						{}
	~CDirectoryEntry()						{}

	CDirectoryEntry	*GetChild()				{ return pChildEntry; }
	CDirectoryEntry *GetSibling()			{ return pSiblingEntry; }
	void SetChild(CDirectoryEntry *pDir)	{ pChildEntry = pDir; };
	void SetSibling(CDirectoryEntry *pDir)	{ pSiblingEntry = pDir; }
	unsigned int GetSize()					{ return Size; }
	unsigned int GetOffset()				{ return Offset; }
	wchar_t *GetName();

private:
	CDirectoryEntry	*pChildEntry;
	CDirectoryEntry *pSiblingEntry;
	BOOL			Type;
	unsigned int	Size;
	unsigned int	Offset;
	char			Name[1];		
};

class CResource
{
public:
	enum
	{
		SEEK_MODE_FILE			= 0,
		SEEK_MODE_DIRECTORY		= 1,
		SEEK_MODE_STORE_PATH	= 2,
		MAX_NESTED_DIRECTORIES	= 16,
	};

	CResource();
	~CResource();

	BOOL Open(wchar_t *);
	void Close();
	BOOL ChangeDirectory(wchar_t *);
	void *Load(wchar_t *);
	BOOL LoadAt(wchar_t *, void *);
	unsigned int SeekToFile(wchar_t *);
	BOOL Read(void *, unsigned int, unsigned int);

private:
	HANDLE			Handle;
	unsigned int	FileSize;
	unsigned int	DirectoryOffset;
	unsigned int	DirectoryDepth;
	CDirectoryEntry	*pDirectory;
	CDirectoryEntry	*pDirectoryMarker[MAX_NESTED_DIRECTORIES];
	void			InitDirectoryEntry(CDirectoryEntry *, unsigned int);
	CDirectoryEntry *SeekFile(wchar_t *, int);
};

#endif
