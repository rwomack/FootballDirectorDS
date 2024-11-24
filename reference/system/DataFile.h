#pragma once

class CDataFile
{
public:
	CDataFile(void);
	virtual ~CDataFile(void);

public:
	bool					Close();
	bool					OpenForRead(CString& _Str);
	bool					OpenForWrite(CString& _Str, const bool _AskOverWrite = true);
	virtual void			Read(void* _Data, uint _Size);
	virtual void			Write(const void* _Data, uint _Size);

protected:
	FILE*					m_pFile;

};
/////////////////////////////////////////////////////////////////////////////


class CFileID
{
public:
	bool					CheckID(CDataFile* _File);
	void					Write(CDataFile* _File);
protected:
};
