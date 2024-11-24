
/*

	File:- BinaryResource.h
  
	Interface for the CBinaryResource class.
	
	(c) Rw Software 2004 - 2008
	  
  ---------------------------------------------------------------------------------------
*/


#if !defined(AFX_BINARYRESOURCE_H__82DAA0C5_FD39_4E95_8748_F84EF382EE77__INCLUDED_)
#define AFX_BINARYRESOURCE_H__82DAA0C5_FD39_4E95_8748_F84EF382EE77__INCLUDED_



class CBinaryReader
{
public:
	CBinaryReader(HMODULE _hModule, const uint _ResourceID, LPCTSTR _pcszClass);
	~CBinaryReader();
	
	bool					IsLoaded() const;
	PBYTE					GetData();	
	uint					GetSize() const;	
	void					SetUserData(const uint _Value);
	uint					GetUserData() const;
	void					ReadBytes(byte* _Dest, const ushort _HowMany);
	byte					ReadByte();
	ushort					ReadUInt16(); 
	uint					ReadUInt32(); 
	void					ReadUInt8Array(byte* _Val, uint _Count);
	void					ReadUInt16Array(ushort* _Val, uint _Count);
	void					LoadData(HMODULE _hModule, uint _ResourceID, LPCTSTR _pcszClass);
	void					UnloadData();
	
#ifdef RESOURCESFROMFILE
	FILE*					GetFile()
	{
		return m_pFile;
	}
#endif

protected:
	uint					m_dwUser;
	uint					m_dwSize;
	PBYTE					m_pCurrentPosition;
	PBYTE					m_pData;
	HGLOBAL					m_hGlobal;
	HRSRC					m_hrsrc;
#ifdef RESOURCESFROMFILE
	bool					m_bLoadFromFile;
	FILE*					m_pFile;
#endif
};
#endif