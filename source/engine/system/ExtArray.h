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

  void          ReadSize(CDataFile* File)
	{
    FDASSERT(File);
#ifdef WIN32
    File->Read(&m_iHowMany, sizeof(m_iHowMany));
#endif
	}

	void					ReadSizeFromResource(CBinaryReader& _Resource)
	{
		m_iHowMany = _Resource.ReadUInt16();
	}

  void          WriteSize(CDataFile* File)
	{
    FDASSERT(File);
#ifdef WIN32
    File->Write(&m_iHowMany, sizeof(m_iHowMany));
#endif
	}
};
