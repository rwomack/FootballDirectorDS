#include <fdmemory.h>

#include <lemon/platform/lassert.h>

FDMemoryBinManager g_MemoryBinManager;

FDMemoryBinManager::FDMemoryBinManager() : m_bin_count(0)
{
}

FDMemoryBinManager::~FDMemoryBinManager()
{
}

unsigned char *FDMemoryBinManager::GetBlockOfSize(const size_t size)
{
	for (int i = 0; i < m_bin_count; i++)
	{
		if ((m_bin_sizes[i] == size) && (m_bin_free_ptr[i] != 0))
		{
			// Return the pointer.
			unsigned int *pReturn = (unsigned int *)m_bin_free_ptr[i];

			m_bin_free_ptr[i] = (unsigned char *)(*pReturn);
			return (unsigned char *)pReturn;
		}
	}

	// Create a new bin.
	const int iNewBinIndex = CreateBinOfSize(size);

	// Return the pointer.
	unsigned int *pReturn = (unsigned int *)m_bin_free_ptr[iNewBinIndex];
	m_bin_free_ptr[iNewBinIndex] = (unsigned char *)(*pReturn);
	return (unsigned char *)pReturn;
}

void FDMemoryBinManager::FreeBlockOfSize(void *pBlock, const size_t size)
{
	const int iBinIndex = FindBinIndex(size);
	lAssert(iBinIndex >= 0);

	// Set the pointer as the first free one of that size.
	unsigned int *pFreed = (unsigned int *)pBlock;
	*pFreed = (unsigned int)m_bin_free_ptr[iBinIndex];
	m_bin_free_ptr[iBinIndex] = (unsigned char *)pFreed;
}

int FDMemoryBinManager::FindBinIndex(const size_t size) const
{
	for (int i = 0; i < m_bin_count; i++)
	{
		if (m_bin_sizes[i] == size)
			return i;
	}

	return -1;
}

int FDMemoryBinManager::CreateBinOfSize(const size_t size)
{
	// Need enough space to store a pointer.
	lAssert(size >= 4);
  lAssert((size&0x3) == 0); // check size is 32bit aligned

  const int bin_units = 100;
	m_bins[m_bin_count] = FD_ENGINE_NEW unsigned char[size * bin_units];

//  lAssert( lIsPtrAligned32( m_bins[m_bin_count] ) ); // check pointer is 32bit aligned

	unsigned char *pArray = m_bins[m_bin_count];
	for (int i = 0; i < bin_units - 1; i++)
	{
		unsigned int *pCurrent = ((unsigned int *)&pArray[i * size]);
		unsigned int *pNext = ((unsigned int *)&pArray[(i + 1) * size]);
		*pCurrent = (unsigned int)pNext;
	}

	unsigned int *pLast = ((unsigned int *)&pArray[(bin_units - 1) * size]);
	*pLast = 0;

	m_bin_free_ptr[m_bin_count] = m_bins[m_bin_count];
	m_bin_sizes[m_bin_count] = size;

	m_bin_count++;

  lAssert( m_bin_count < FDMEMORY_MAX_BINS );

	return m_bin_count - 1;
}

