#ifndef FDMEMORY_H
#define FDMEMORY_H

#include <stddef.h>

// By default, include file/line information.
#define FD_MEMORY_DEBUG_INFO

// Don't use file/line info on the DS...
#ifdef L_PLATFORM_NDS
#ifdef FD_MEMORY_DEBUG_INFO
#undef FD_MEMORY_DEBUG_INFO
#endif
#endif

#ifndef _DEBUG
#ifdef FD_MEMORY_DEBUG_INFO
#undef FD_MEMORY_DEBUG_INFO
#endif
#endif

// Uncomment this to disable binning and just use regular allocations for those objects.
//#define FD_MEMORY_NO_BINNING 1

#ifdef L_PLATFORM_NDS
#pragma warning off (10107)             // identifier expected
#endif

enum FDMemoryPoolType
{
	FDMPT_GAME_LIB,
	FDMPT_FD_ENGINE_LIB,
  FDMPT_FD_ENGINE_LIB_LARGE,
	FDMPT_LEMON,
  //FDMPT_LEMONMEM,
	FDMPT_STRING,
  FDMPT_LOADSAVEMUSIC,
	FDMPT_MAX,
};

#ifdef L_PLATFORM_NDS
#pragma warning on (10107)             // identifier expected
#endif

#ifdef FD_MEMORY_DEBUG_INFO
// Supply file/line information.
#define FD_GAME_NEW ::new(FDMPT_GAME_LIB, __FILE__, __LINE__)
#define FD_ENGINE_NEW ::new(FDMPT_FD_ENGINE_LIB, __FILE__, __LINE__)
#define FD_ENGINE_LARGE_NEW ::new(FDMPT_FD_ENGINE_LIB_LARGE, __FILE__, __LINE__)
#define FD_STRING_NEW ::new(FDMPT_STRING, __FILE__, __LINE__)
#define FD_LOADSAVEMUSIC_NEW ::new(FDMPT_LOADSAVEMUSIC,  __FILE__, __LINE__)
#if !FD_MEMORY_NO_BINNING
#define FD_BIN_NEW new(FDMPT_FD_ENGINE_LIB, __FILE__, __LINE__)
#else
#define FD_BIN_NEW ::new(FDMPT_FD_ENGINE_LIB, __FILE__, __LINE__)
#endif

#define FD_GAME_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_GAME_LIB, (_size), (_oldblock), __FILE__, __LINE__)
#define FD_ENGINE_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_FD_ENGINE_LIB, (_size), (_oldblock), __FILE__, __LINE__)
#define FD_ENGINE_LARGE_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_FD_ENGINE_LIB_LARGE, (_size), (_oldblock), __FILE__, __LINE__)
#define FD_STRING_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_STRING, (_size), (_oldblock), __FILE__, __LINE__)

void *operator new(size_t size, const FDMemoryPoolType ePool, const char *file, const int line);
void *operator new[](size_t size, const FDMemoryPoolType ePool, const char *file, const int line);
void operator delete(void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw();
void operator delete[](void *block, const FDMemoryPoolType ePool, const char *file, const int line) throw();

#else
// Don't use file/line information.
#define FD_GAME_NEW ::new(FDMPT_GAME_LIB)
#define FD_ENGINE_NEW ::new(FDMPT_FD_ENGINE_LIB)
#define FD_ENGINE_LARGE_NEW ::new(FDMPT_FD_ENGINE_LIB_LARGE)
#define FD_STRING_NEW ::new(FDMPT_STRING)
#define FD_LOADSAVEMUSIC_NEW ::new(FDMPT_LOADSAVEMUSIC)
#if !FD_MEMORY_NO_BINNING
#define FD_BIN_NEW new(FDMPT_FD_ENGINE_LIB)
#else
#define FD_BIN_NEW ::new(FDMPT_FD_ENGINE_LIB)
#endif

#define FD_GAME_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_GAME_LIB, (_size), (_oldblock))
#define FD_ENGINE_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_FD_ENGINE_LIB, (_size), (_oldblock))
#define FD_ENGINE_LARGE_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_FD_ENGINE_LIB_LARGE, (_size), (_oldblock))
#define FD_STRING_REALLOC(_size, _oldblock) FD_Heap_Realloc(FDMPT_STRING, (_size), (_oldblock))

void *operator new(size_t size, const FDMemoryPoolType pool);
void *operator new[](size_t size, const FDMemoryPoolType pool);
void operator delete(void *block, const FDMemoryPoolType pool) throw();
void operator delete[](void *block, const FDMemoryPoolType pool) throw();
#endif

//////////////////////////////////////////////////////////////////////////
// Macros and definitions relating to allocating objects out of sized bins.
//////////////////////////////////////////////////////////////////////////

#define FDMEMORY_MAX_BINS	500
class FDMemoryBinManager
{
public:
	FDMemoryBinManager();
	~FDMemoryBinManager();

	unsigned char *GetBlockOfSize(const size_t size);
	void FreeBlockOfSize(void *pBlock, const size_t size);
private:
	int FindBinIndex(const size_t size) const;
	int CreateBinOfSize(const size_t size);

	unsigned char *m_bins[FDMEMORY_MAX_BINS];
	unsigned char *m_bin_free_ptr[FDMEMORY_MAX_BINS];
	size_t m_bin_sizes[FDMEMORY_MAX_BINS];
	size_t m_bin_count;
};

extern FDMemoryBinManager g_MemoryBinManager;

#if !FD_MEMORY_NO_BINNING
#define DECLARE_FDMEMORY_BINALLOC(_CLASSNAME)																\
	public:																									\
	void *operator new(size_t size);																		\
	void *operator new(size_t size, const FDMemoryPoolType ePool);											\
	void *operator new(size_t size, const FDMemoryPoolType ePool, const char *file, const int line);		\
	void *operator new[](size_t size);																		\
	void *operator new[](size_t size, const FDMemoryPoolType ePool);										\
	void *operator new[](size_t size, const FDMemoryPoolType ePool, const char *file, const int line);		\
	void operator delete(void *ptr);																		\
	void operator delete[](void *ptr);																		\
	void operator delete(void *ptr, const FDMemoryPoolType ePool);											\
	void operator delete(void *ptr, const FDMemoryPoolType ePool, const char *file, const int line);		\
	void operator delete[](void *ptr, const FDMemoryPoolType ePool);										\
	void operator delete[](void *ptr, const FDMemoryPoolType ePool, const char *file, const int line);


#define IMPLEMENT_FDMEMORY_BINALLOC(_CLASSNAME)																			\
	void *_CLASSNAME::operator new(size_t size)																			\
	{																													\
		return g_MemoryBinManager.GetBlockOfSize(size);																	\
	}																													\
	void *_CLASSNAME::operator new(size_t size, const FDMemoryPoolType ePool)											\
	{																													\
		return g_MemoryBinManager.GetBlockOfSize(size);																	\
	}																													\
	void *_CLASSNAME::operator new(size_t size, const FDMemoryPoolType ePool, const char *file, const int line)			\
	{																													\
		return g_MemoryBinManager.GetBlockOfSize(size);																	\
	}																													\
	void *_CLASSNAME::operator new[](size_t size)																		\
	{																													\
		void *pMemory = g_MemoryBinManager.GetBlockOfSize(size + sizeof(size_t));										\
		size_t *pSize = (size_t *)pMemory;																				\
		*pSize = size;																									\
		return (void *)(pSize + 1);																						\
	}																													\
	void *_CLASSNAME::operator new[](size_t size, const FDMemoryPoolType ePool)											\
	{																													\
		void *pMemory = g_MemoryBinManager.GetBlockOfSize(size + sizeof(size_t));										\
		size_t *pSize = (size_t *)pMemory;																				\
		*pSize = size;																									\
		return (void *)(pSize + 1);																						\
	}																													\
	void *_CLASSNAME::operator new[](size_t size, const FDMemoryPoolType ePool, const char *file, const int line)		\
	{																													\
		void *pMemory = g_MemoryBinManager.GetBlockOfSize(size + sizeof(size_t));										\
		size_t *pSize = (size_t *)pMemory;																				\
		*pSize = size;																									\
		return (void *)(pSize + 1);																						\
	}																													\
	void _CLASSNAME::operator delete(void *ptr)																			\
	{																													\
		g_MemoryBinManager.FreeBlockOfSize(ptr, sizeof(_CLASSNAME));													\
	}																													\
	void _CLASSNAME::operator delete[](void *ptr)																		\
	{																													\
		size_t *pSize = ((size_t *)ptr) - 1;																			\
		g_MemoryBinManager.FreeBlockOfSize(ptr, *pSize);																\
	}																													\
	void _CLASSNAME::operator delete(void *ptr, const FDMemoryPoolType ePool)											\
	{																													\
		g_MemoryBinManager.FreeBlockOfSize(ptr, sizeof(_CLASSNAME));													\
	}																													\
	void _CLASSNAME::operator delete(void *ptr, const FDMemoryPoolType ePool, const char *file, const int line)			\
	{																													\
		g_MemoryBinManager.FreeBlockOfSize(ptr, sizeof(_CLASSNAME));													\
	}																													\
	void _CLASSNAME::operator delete[](void *ptr, const FDMemoryPoolType ePool)											\
	{																													\
		size_t *pSize = ((size_t *)ptr) - 1;																			\
		g_MemoryBinManager.FreeBlockOfSize(ptr, *pSize);																\
	}																													\
	void _CLASSNAME::operator delete[](void *ptr, const FDMemoryPoolType ePool, const char *file, const int line)		\
	{																													\
		size_t *pSize = ((size_t *)ptr) - 1;																			\
		g_MemoryBinManager.FreeBlockOfSize(ptr, *pSize);																\
	}

#else
#define DECLARE_FDMEMORY_BINALLOC(_ClassName)
#define IMPLEMENT_FDMEMORY_BINALLOC(_ClassName)
#endif

//////////////////////////////////////////////////////////////////////////

void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock);
void *FD_Heap_Realloc(const FDMemoryPoolType ePool, const size_t newsize, void *oldblock, const char *file, const int line);

void DumpDlmallocStats();
void DumpDlmallocStatsToFile(const char *file_name);

void TrimHeaps();
void TrimHeap(const FDMemoryPoolType eHeap);

#endif // #ifndef FDMEMORY_H

