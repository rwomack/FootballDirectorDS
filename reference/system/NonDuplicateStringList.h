

 /*
 
	C++ Class - CNonDuplicateStringList


	(c) RW Software 1994 - 2005

  	---------------------------------------------------------------
 */

 #ifndef _NONDUPLICATESTRINGLIST_H__
 #define __NONDUPLICATESTRINGLIST_H__


class CNonDuplicateStringList
{
public:
	CNonDuplicateStringList() :
		 m_nOrginalListSize(0) {};

		 int Add(const CString& _Str);
		 void Read(CDataFile* _pDataFile, bool _bObfuscate = false);
		 void Write(CDataFile* _pDataFile);
	void ReadNew(CDataFile* _pDataFile);
	void WriteNew(CDataFile* _pFile);
	void DoInitialiseFromResource(CBinaryReader& _Resource, bool _Obfuscate = false);
	void DoWriteStringsToFile(CDataFile* _pDataFile, ushort _nFirstString, ushort _HowMany);

	CString& GetString(const ushort _ID);
	ushort DoFindStringID(const CString& _NewName);

protected:
	ushort					m_nOrginalListSize;
	CStringArray			m_strList;
};
#endif