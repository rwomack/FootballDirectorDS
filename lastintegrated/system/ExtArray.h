class CExtArray
{
private:
	ushort					m_iHowMany;

public:
    // Methods
	void					SetSize(const ushort x)
	{ 
		m_iHowMany = x; 
	}
	
	const ushort			GetSize() const									
	{ 
		return m_iHowMany; 
	}

	const ushort			GetUpperBound()	const
	{ 
		return m_iHowMany - 1; 
	}
	
	void					ReadSize(CDataFile* _pDataFile)
	{ 
		EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
#ifdef WIN32
		_pDataFile->Read(&m_iHowMany, sizeof m_iHowMany); 
#endif
	}

	void					ReadSizeFromResource(CBinaryReader& _Resource)							
	{ 
		m_iHowMany = _Resource.ReadUInt16(); 
	}

	void					WriteSize(CDataFile* _pDataFile)
	{ 
		EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
#ifdef WIN32
		_pDataFile->Write(&m_iHowMany, sizeof m_iHowMany); 
#endif
	}
};
