//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmemory.cpp
// Description : Memory-related functionality for the game -- mostly logging-related.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>

#ifdef USE_DLMALLOC
#  include <dlmalloc.h>
#endif

#include <lemon/lapi.h>
#include <lemon/platform/lcore.h>
#include <lemon/platform/lassert.h>
#include <lemon/io/lfileiostream.h>
#include <stdio.h>

#ifdef L_PLATFORM_NDS
//NOTE: Use ARM for this file
#include <nitro/code32.h>
#endif

#ifdef _DEBUG
#define FD_MEMORY_OVERWRITE_MEMORY_ON_ALLOC_AND_FREE
#endif

#define PLAYERSIZE 104
#define MAXNUMPLAYERS 4300

//#define LOG_LOADSAVEMUSIC

  int            s_totalallocs = 0;
  int            s_totalfrees = 0;
  int            s_numallocs = 0;


//////////////////////////////////////////////////////////////////////////
// Header structure for allocations.
struct FDAllocHeader
{
	const char *file;
	int line;
	size_t requested_size;
	unsigned int sequence_number;
	FDAllocHeader *prev;
	FDAllocHeader *next;
};

static unsigned int s_uiSequenceNumber = 0;

static bool s_forceLoadSaveHeap = false;

void ForceLoadSaveHeap( bool b )
{
  s_forceLoadSaveHeap = b;
  if( b ) lLogFmt("Forcing load/save heap\n");
  else    lLogFmt("Unforcing load/save heap\n");
}

#ifdef USE_DLMALLOC

//////////////////////////////////////////////////////////////////////////
// Tracking information for a heap.
class FDHeap
{
public:
	mspace heap_mspace;
	size_t heap_size;
	const char *heap_name;
	FDAllocHeader *first_alloc;

	void create(const size_t _heap_size, const char *_heap_name);
	void destroy();

	void *malloc(const size_t blockSize);
	void *malloc(const size_t blockSize, const char *file, const int line);

	void free(void *block);
	void free(void *block, const char *file, const int line);

	void *realloc(const size_t blockSize, void *old_block);
	void *realloc(const size_t blockSize, void *old_block, const char *file, const int line);

	void dump_to_file(Lemon::lFileIOStream &cStream);
};

static FDHeap g_FDMspaces[FDMPT_MAX];

void FDHeap::create(const size_t _heap_size, const char *_heap_name)
{
	heap_mspace = create_mspace(_heap_size, 0);
	heap_size = _heap_size;
	heap_name = _heap_name;
	first_alloc = NULL;
}

void FDHeap::destroy()
{
	destroy_mspace(heap_mspace);
	heap_size = 0;
	heap_name = NULL;
	first_alloc = NULL;
}

void *FDHeap::malloc(const size_t blockSize)
{
	return malloc(blockSize, __FILE__, __LINE__);
}

void *FDHeap::malloc(const size_t blockSize, const char *file, const int line)
{
#ifdef FD_MEMORY_DEBUG_INFO
	const size_t size_to_allocate = blockSize + sizeof(FDAllocHeader);
	FDAllocHeader *new_header = (FDAllocHeader *)mspace_malloc(heap_mspace, size_to_allocate);

	new_header->file = file;
	new_header->line = line;
	new_header->prev = NULL;
	new_header->requested_size = blockSize;
	new_header->next = first_alloc;
	new_header->sequence_number = s_uiSequenceNumber++;

	if (new_header->next != NULL)
		new_header->next->prev = new_header;

	first_alloc = new_header;

	void *return_pointer = (void *)(new_header + 1);
#else
	void *return_pointer = mspace_malloc(heap_mspace, blockSize);
#endif

#ifdef FD_MEMORY_OVERWRITE_MEMORY_ON_ALLOC_AND_FREE
	::memset(return_pointer, 0xcdcdcdcd, blockSize);
#endif

  s_totalallocs++;
  s_numallocs++;

#ifdef LOG_LOADSAVEMUSIC
  if( this == &g_FDMspaces[FDMPT_LOADSAVEMUSIC] )
  {
    lLogBreak();
    lLogFmt("Alloc %d (%s : %d) = %x\n", blockSize, file, line, return_pointer);
    DumpDlmallocStats();
    lLogBreak();
  }
#endif

	return return_pointer;
}

void FDHeap::free(void *block)
{
	free(block, __FILE__, __LINE__);
}

void FDHeap::free(void *block, const char *file, const int line)
{
#ifdef FD_MEMORY_DEBUG_INFO
	if (block == NULL)
		return;

	FDAllocHeader *header = ((FDAllocHeader *)block) - 1;

	if (header->prev != NULL)
	{
		header->prev->next = header->next;
	}
	else
	{
		// This is the first one in the list.
		first_alloc = header->next;
	}

	if (header->next != NULL)
	{
		header->next->prev = header->prev;
	}

	void *tofree = header;

#ifdef FD_MEMORY_OVERWRITE_MEMORY_ON_ALLOC_AND_FREE
	::memset(block, 0xdddddddd, header->requested_size);
#endif
#else
	void *tofree = block;
#endif

#ifdef LOG_LOADSAVEMUSIC
  if( this == &g_FDMspaces[FDMPT_LOADSAVEMUSIC] )
  {
    lLogBreak();
    lLogFmt("Free %x (%s : %d)\n", tofree, file, line);
    DumpDlmallocStats();
    lLogBreak();
  }
#endif


  s_totalfrees++;
  s_numallocs--;

	dlfree(tofree);
}

void *FDHeap::realloc(const size_t blockSize, void *old_block)
{
	return realloc(blockSize, old_block, __FILE__, __LINE__);
}

void *FDHeap::realloc(const size_t blockSize, void *old_block, const char *file, const int line)
{
#ifdef LOG_LOADSAVEMUSIC
  if( this == &g_FDMspaces[FDMPT_LOADSAVEMUSIC] )
  {
    lLogBreak();
    lLogFmt("Realloc %x : %d (%s : %d)\n", old_block, blockSize, file, line);
    DumpDlmallocStats();
    lLogBreak();
  }
#endif

#ifdef FD_MEMORY_DEBUG_INFO
	if (blockSize == 0)
	{
		free(old_block, file, line);
		return NULL;
	}

	if (old_block == NULL)
		return malloc(blockSize, file, line);

	FDAllocHeader *header = ((FDAllocHeader *)old_block) - 1;
	const size_t size_to_allocate = blockSize + sizeof(FDAllocHeader);

	FDAllocHeader *prev = header->prev;
	FDAllocHeader *next = header->next;

	FDAllocHeader *new_header = (FDAllocHeader *)mspace_realloc(heap_mspace, header, size_to_allocate);

	if (prev != NULL)
		prev->next = new_header;
	if (next != NULL)
		next->prev = new_header;
	if (first_alloc == header)
		first_alloc = new_header;

	new_header->requested_size = blockSize;

	void *return_pointer = (void *)(new_header + 1);
	return return_pointer;
#else
	return mspace_realloc(heap_mspace, old_block, blockSize);
#endif
}

void FDHeap::dump_to_file(Lemon::lFileIOStream &cStream)
{
	char temp_buffer[1024];
	::sprintf(temp_buffer, "<heap>\n");
	cStream.Write(temp_buffer, ::strlen(temp_buffer));

  mallinfo cMemoryInfo = mspace_mallinfo(heap_mspace);

  ::sprintf(temp_buffer, "<name>%s</name>\n<size>%d</size><used>%d</used>\n", heap_name, heap_size, cMemoryInfo.uordblks);
	cStream.Write(temp_buffer, ::strlen(temp_buffer));

	::sprintf(temp_buffer, "<allocations>\n");
	cStream.Write(temp_buffer, ::strlen(temp_buffer));

	FDAllocHeader *iter = first_alloc;
	while (iter != NULL)
	{
		::sprintf(temp_buffer, "<allocation>\n");
		cStream.Write(temp_buffer, ::strlen(temp_buffer));

		::sprintf(temp_buffer, "<address>%u</address>\n<size>%d</size>\n<filename>%s</filename>\n<linenumber>%d</linenumber><sequence>%u</sequence>\n",
			(unsigned int)((void *)(iter + 1)), iter->requested_size, iter->file, iter->line, iter->sequence_number);
		cStream.Write(temp_buffer, ::strlen(temp_buffer));

		::sprintf(temp_buffer, "</allocation>\n");
		cStream.Write(temp_buffer, ::strlen(temp_buffer));

		iter = iter->next;
	}

	::sprintf(temp_buffer, "</allocations>\n");
	cStream.Write(temp_buffer, ::strlen(temp_buffer));

	::sprintf(temp_buffer, "</heap>\n");
	cStream.Write(temp_buffer, ::strlen(temp_buffer));
}

static void UnknownHeapFree(void *ptr)
{
  s_totalfrees++;
  s_numallocs--;

#ifdef FD_MEMORY_DEBUG_INFO
	FDAllocHeader *header = ((FDAllocHeader *)ptr) - 1;

	if (header->prev == NULL)
	{
		// Check if this is one of the first allocations
		// in a heap. If so, then update the heap's first_alloc member.
		bool found_first = false;
		for (int i = 0; i < FDMPT_MAX; i++)
		{
			if (g_FDMspaces[i].first_alloc == header)
			{
				g_FDMspaces[i].first_alloc = header->next;
				found_first = true;
				break;
			}
		}

		lAssert(found_first);
	}
	else
	{
		header->prev->next = header->next;
	}

	if (header->next != NULL)
		header->next->prev = header->prev;

	dlfree(header);
#else
	dlfree(ptr);
#endif
}

void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock)
{
	return g_FDMspaces[ePool].realloc(newsize, oldblock);
}

void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock, const char *file, const int line)
{
	return g_FDMspaces[ePool].realloc(newsize, oldblock, file, line);
}

//////////////////////////////////////////////////////////////////////////
static bool s_MspacesInitialized = false;
void Initialize_Mspaces()
{
	lAssert(s_MspacesInitialized == false);

	// ELN - 6-Apr-2008: Set up mspaces for the application. These numbers were based off of empirical
	// testing, and a sanity check against the memory budget. If the mspaces run out
	// of memory, they will try and allocate additional space...not ideal, so the budgets
	// should be adjusted if necessary, but this should not be fatal either.

  lLogFmt("Allocating memory\n");
  lLogFmt("  LargePlayerList: %d*%d = %d\n", PLAYERSIZE, MAXNUMPLAYERS, PLAYERSIZE*MAXNUMPLAYERS);

  // 280899 used during load (of maximum size)
  // 270592 used during save (of any size)
  // + 40kb for sound / misc

	// Use less RAM on other platforms by default.
  g_FDMspaces[FDMPT_GAME_LIB].create(300000, "Game Heap");       // 256K - 4K
  g_FDMspaces[FDMPT_FD_ENGINE_LIB].create(-4096 + (1<<19) + (1<<17), "FD Engine Heap");
  g_FDMspaces[FDMPT_FD_ENGINE_LIB_LARGE].create(PLAYERSIZE*MAXNUMPLAYERS, "FD Engine LARGE Heap");
  g_FDMspaces[FDMPT_LEMON].create(-4096 + (1<<19) + (1<<16), "Lemon Engine Heap");    // 128k - 4K
  g_FDMspaces[FDMPT_STRING].create(-4096 + (1<<17), "Strings Heap");      // 64K - 4K
  g_FDMspaces[FDMPT_LOADSAVEMUSIC].create( -4096 + 262144 + 32768, "Load/Save/Music Heap");

	// Change the trim threshold to a much smaller value.
  //::dlmallopt(M_TRIM_THRESHOLD, 16 * 1024);

	s_MspacesInitialized = true;
}

//////////////////////////////////////////////////////////////////////////
void DumpDlmallocStats()
{
	// ELN - 6-Apr-2008: Dump out basic stats for mspace usage.
	for (int i = 0; i < (int)FDMPT_MAX; i++)
	{
    mallinfo cMemoryInfo = mspace_mallinfo(g_FDMspaces[i].heap_mspace);

    #ifdef L_PLATFORM_NDS
    OS_Printf("mspace %d (%s): %d bytes used, %d bytes free, %d allocs (total %d allocs %d frees)\n", i, g_FDMspaces[i].heap_name, cMemoryInfo.uordblks, cMemoryInfo.fordblks, s_numallocs, s_totalallocs, s_totalfrees );
    #else
    lLogFmt("mspace %d (%s): %d bytes used, %d bytes free, %d allocs (total %d allocs %d frees)\n", i, g_FDMspaces[i].heap_name, cMemoryInfo.uordblks, cMemoryInfo.fordblks, s_numallocs, s_totalallocs, s_totalfrees );
    #endif
	}
}

void DumpDlmallocStatsToFile(const char *file_name)
{
	Lemon::lFileIOStream cOutStream;

	const bool bSuccess = cOutStream.Open(file_name, L_IO_ACCESS_WRITE, L_IO_OPEN_CREATE_ALWAYS);
	if (bSuccess == false)
		return;

	char temp_buffer[256];
	::sprintf(temp_buffer, "<MemoryDump>\n");
	cOutStream.Write(temp_buffer, ::strlen(temp_buffer));

	for (int i = 0; i < (int)FDMPT_MAX; i++)
  {
    if( i == FDMPT_LOADSAVEMUSIC ) continue;
		g_FDMspaces[i].dump_to_file(cOutStream);
  }

	::sprintf(temp_buffer, "</MemoryDump>\n");
	cOutStream.Write(temp_buffer, ::strlen(temp_buffer));

	cOutStream.Close();
}

void TrimHeaps()
{
	for (int i = 0; i < (int)FDMPT_MAX; i++)
		TrimHeap((FDMemoryPoolType)i);
}

void TrimHeap(const FDMemoryPoolType eHeap)
{
	// Give 512 bytes of padding.
	mspace_trim(g_FDMspaces[eHeap].heap_mspace, 512);
}

//////////////////////////////////////////////////////////////////////////
void* operator new ( size_t blocksize )
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  //return lAlloc( blocksize );
  return g_FDMspaces[FDMPT_LEMON].malloc(blocksize);
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void *operator new (size_t blocksize, const FDMemoryPoolType ePool, const char *file, const int line)
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  return g_FDMspaces[ePool].malloc(blocksize, file, line);
}
#else
void *operator new (size_t blocksize, const FDMemoryPoolType ePool)
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  return g_FDMspaces[ePool].malloc(blocksize);
}
#endif

//----------------------------------------------------------------
//
void* operator new[] ( size_t blocksize )
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  return g_FDMspaces[FDMPT_LEMON].malloc(blocksize);
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void *operator new[] (size_t blocksize, const FDMemoryPoolType ePool, const char *file, const int line)
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  return g_FDMspaces[ePool].malloc(blocksize, file, line);
}
#else
void *operator new[] (size_t blocksize, const FDMemoryPoolType ePool)
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  return g_FDMspaces[ePool].malloc(blocksize);
}
#endif

//----------------------------------------------------------------
//
void operator delete ( void* block ) throw()
{
	if( block != NULL )
	{
		UnknownHeapFree(block);
	}
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void operator delete (void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw()
{
	if (block)
	{
    g_FDMspaces[ePool].free(block, file, line);
	}
}
#else
#ifdef L_PLATFORM_NDS
void operator delete (void *block, const FDMemoryPoolType ePool)
#else
void operator delete (void *block, const FDMemoryPoolType ePool) throw()
#endif
{
	if (block)
	{
    g_FDMspaces[ePool].free(block);
	}
}
#endif

//----------------------------------------------------------------
//
void operator delete[] ( void* block ) throw()
{
	if( block != NULL )
	{
		UnknownHeapFree(block);
	}
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void operator delete[] (void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw()
{
	if (block)
	{
    g_FDMspaces[ePool].free(block, file, line);
	}
}
#else
#ifdef L_PLATFORM_NDS
void operator delete[] (void *block, const FDMemoryPoolType ePool)
#else
void operator delete[] (void *block, const FDMemoryPoolType ePool) throw()
#endif
{
	if (block)
	{
    g_FDMspaces[ePool].free(block);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
// Lemon engine memory function overrides

LPLATFORM_API void * lPlatform_DebugAlloc(const char * file, const char * function, unsigned int line, enum lArenaID arenaID, size_t size )
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

//  lLogFmt("lPlatform_DebugAlloc: %s / %s / %d - %d\n", file, function, line, size);

  if( arenaID == L_ARENA_IO && s_forceLoadSaveHeap == true )
  {
    return g_FDMspaces[FDMPT_LOADSAVEMUSIC].malloc(size, file, line);
  } else
  {
    //return g_FDMspaces[FDMPT_LEMONMEM].malloc(size, file, line);
    return g_FDMspaces[FDMPT_LEMON].malloc(size, file, line);
  }
}

LPLATFORM_API void   lPlatform_DebugFree(const char * file, const char * function, unsigned int line, void *ptr )
{
	UnknownHeapFree(ptr);
}

LPLATFORM_API void * lPlatform_DebugRealloc(const char * file, const char * function, unsigned int line, enum lArenaID arenaID, void *ptr, size_t size)
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

//  lLogFmt("lPlatform_DebugRealloc: %s / %s / %d - %d / %x\n", file, function, line, size);

  if( arenaID == L_ARENA_IO && s_forceLoadSaveHeap == true )
  {
    return g_FDMspaces[FDMPT_LOADSAVEMUSIC].realloc(size, ptr, file, line);
  } else
  {
    //return g_FDMspaces[FDMPT_LEMONMEM].realloc(size, ptr, file, line);
    return g_FDMspaces[FDMPT_LEMON].realloc(size, ptr, file, line);
  }
}

LPLATFORM_API void * lPlatform_ReleaseAlloc( enum lArenaID arenaID, size_t size )
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  if( arenaID == L_ARENA_IO && s_forceLoadSaveHeap == true )
  {
    return g_FDMspaces[FDMPT_LOADSAVEMUSIC].malloc(size);
  } else
  {
    //return g_FDMspaces[FDMPT_LEMONMEM].malloc(size);
    return g_FDMspaces[FDMPT_LEMON].malloc(size);
  }
}

LPLATFORM_API void   lPlatform_ReleaseFree( void * ptr )
{
	UnknownHeapFree(ptr);
}

LPLATFORM_API void * lPlatform_ReleaseRealloc( enum lArenaID arenaID, void *ptr, size_t size )
{
	if (s_MspacesInitialized == false)
		Initialize_Mspaces();

  if( arenaID == L_ARENA_IO && s_forceLoadSaveHeap == true )
  {
    return g_FDMspaces[FDMPT_LOADSAVEMUSIC].realloc(size, ptr);
  } else
  {
    //return g_FDMspaces[FDMPT_LEMONMEM].realloc(size, ptr);
    return g_FDMspaces[FDMPT_LEMON].realloc(size, ptr);
  }
}
//////////////////////////////////////////////////////////////////////////

#else                                   // USE_DLMALLOC

void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock)
{
  return lRealloc(oldblock, newsize);
}

void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock, const char *file, const int line)
{
  //TODO: Pass info
  return lRealloc(oldblock, newsize);
}


//////////////////////////////////////////////////////////////////////////
void* operator new ( size_t blocksize )
{
  return lAlloc( blocksize );
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void *operator new (size_t blocksize, const FDMemoryPoolType ePool, const char *file, const int line)
{
  // TODO pass info
  return lAlloc( blocksize );
}
#else
void *operator new (size_t blocksize, const FDMemoryPoolType ePool)
{
  return lAlloc( blocksize );
}
#endif

//----------------------------------------------------------------
//
void* operator new[] ( size_t blocksize )
{
  return lAlloc( blocksize );
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void *operator new[] (size_t blocksize, const FDMemoryPoolType ePool, const char *file, const int line)
{
  // TODO pass info
  return lAlloc( blocksize );
}
#else
void *operator new[] (size_t blocksize, const FDMemoryPoolType ePool)
{
  return lAlloc( blocksize );
}
#endif

//----------------------------------------------------------------
//
void operator delete ( void* block ) throw()
{
	if( block != NULL )
	{
    lFree( block );
	}
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void operator delete (void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw()
{
	if (block)
	{
    // TODO pass info
    return lFree( block );
	}
}
#else
#ifdef L_PLATFORM_NDS
void operator delete (void *block, const FDMemoryPoolType ePool)
#else
void operator delete (void *block, const FDMemoryPoolType ePool) throw()
#endif
{
	if (block)
	{
    return lFree( block );
	}
}
#endif

//----------------------------------------------------------------
//
void operator delete[] ( void* block ) throw()
{
	if( block != NULL )
	{
    return lFree( block );
	}
}

//////////////////////////////////////////////////////////////////////////
#ifdef FD_MEMORY_DEBUG_INFO
void operator delete[] (void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw()
{
	if (block)
	{
    // TODO pass info
    return lFree( block );
	}
}
#else
#ifdef L_PLATFORM_NDS
void operator delete[] (void *block, const FDMemoryPoolType ePool)
#else
void operator delete[] (void *block, const FDMemoryPoolType ePool) throw()
#endif
{
	if (block)
	{
    return lFree( block );
	}
}
#endif


#endif


