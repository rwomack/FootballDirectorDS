
 /*

	File:- Array.h

	(c) RW Software 1994 - 2003

 ---------------------------------------------------------------------------------------
 */


#ifndef __ARRAY_H__
#define __ARRAY_H__


template <class Ttype> class CExtArray
{
public:
	CExtArray(void) {};
	~CExtArray(void)
	{
		delete [] m_pList;
	};

    // Methods
	// ---------------------------------------------------------------------------
	inline void				SetSize(const UInt16 x)
	{
		m_iHowMany = x;
	}

	// ---------------------------------------------------------------------------
	inline UInt16			GetSize(void) const
	{
		return m_iHowMany;
	}

	// ---------------------------------------------------------------------------
	inline UInt16			GetUpperBound(void)	const
	{
		return m_iHowMany - 1;
	}

	// ---------------------------------------------------------------------------
  inline void       ReadSize(CDataFile* File)
	{
    FDASSERT(File != NULL);
    File->Read(&m_iHowMany, sizeof(m_iHowMany));
	}

	// ---------------------------------------------------------------------------
  inline void       WriteSize(CDataFile* File)
	{
    FDASSERT(File != NULL);
    File->Write(&m_iHowMany, sizeof(m_iHowMany));
	}

	// ---------------------------------------------------------------------------
	void					Read(CPalmPDB& rFile)
	{
		for (int nLoopCount = 0; nLoopCount < m_iHowMany; nLoopCount++)
		{
			GetAt(nLoopCount)->Read(rFile, nLoopCount);
		}
	}

	// ---------------------------------------------------------------------------
	void					Write(CPalmPDB& rFile)
	{
		for (int nLoopCount = 0; nLoopCount < m_iHowMany; nLoopCount++)
		{
			GetAt(nLoopCount)->Write(rFile, nLoopCount);
		}
	}

	// ---------------------------------------------------------------------------
	void					DoClearSelectedItems()
	{
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			GetAt(nLoopCount)->SetSelected(false);
		}
	}

	// ---------------------------------------------------------------------------
	CString					GetCountStr()
	{
		CString szRet;
		int nHowManySelected = DoCountSelectedItems();
		if (nHowManySelected == 0)
		{
      LoadText( szRet, IDS_NOPREFERENCE );
			return szRet;
		}
		if (nHowManySelected == 1)
		{
			return pFirstSelected()->GetTitle();
		}
    szRet.Format(lText::Text(IDS_CHOICES), nHowManySelected);
		return szRet;
	}

	// ---------------------------------------------------------------------------
	void					Dump()
	{
#ifdef DEBUG
		afxDump << "\n";
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			afxDump << nLoopCount << " " << GetAt(nLoopCount)->GetTitle() << "\n";;
		}
		afxDump << "\n";
#endif
	}


	// ---------------------------------------------------------------------------
	Ttype*					pFirstSelected()
	{
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsSelected() == true)
			{
				return GetAt(nLoopCount);
			}
		}
    FDASSERT(0);
		return NULL;
	}

	// ---------------------------------------------------------------------------
	int						DoCountMajorItems()
	{
		int nCount = 0;
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsMajor() == true)
			{
				nCount++;
			}
		}
		return nCount;
	}

	// ---------------------------------------------------------------------------
	int						DoCountSelectedItems()
	{
		int nCount = 0;
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsSelected() == true)
			{
				nCount++;
			}
		}
		return nCount;
	}

#ifdef CPALMPDB
	// ---------------------------------------------------------------------------
    void					DoInitialise(CPalmPDB& rFile, int RecordType)
	{
		UInt16 RecordIndexNum = 0;
		SetSize(rFile.DmNumRecordsInCategory(RecordType));

		m_pList = FD_ENGINE_NEW Ttype[GetSize()];
		Ttype* pList = m_pList;
		Err err = rFile.DmSeekRecordInCategory(&RecordIndexNum, 0, dmSeekForward, RecordType);
		while(rFile.GetCategory(RecordIndexNum) == RecordType)
		{
			pList->Read(rFile, RecordIndexNum);
			RecordIndexNum++;
			pList++;
		}
	}
#endif

	// ---------------------------------------------------------------------------
    Ttype*				GetAt(const int x)
	{
    FDASSERT(x < m_iHowMany);
		return static_cast<Ttype*>(&m_pList[x]);
	}

	// ---------------------------------------------------------------------------
	const int			DoFindItemNearest(CString& strToFind, CDWordArray& nListIds)
	{
		CString strInput(strToFind);
		CString strListText;
		int nCurSel = -1;
		int low;
		int mid;
		int high;
		int nTextLength = strToFind.GetLength();
		if (nTextLength > 0)
		{
			if (strToFind.GetLength() > 1)
			{
				strListText = strToFind.Left(1);
				strListText.MakeUpper();
				strToFind = strListText + strToFind.Right(strToFind.GetLength() - 1);
			}
			else
			{
				strToFind.MakeUpper();
			}

			do
			{
				low = 0;
				high = nListIds.GetSize() - 1;

				while(low <= high)
				{
					mid = (low + high) / 2;
					strListText = GetAt(nListIds.GetAt(mid))->GetName().Left(nTextLength);
					if(strListText < strToFind.Left(nTextLength))
					{
						low = mid + 1;
					}
					else if(strListText > strToFind.Left(nTextLength))
					{
						high = mid - 1;
					}
					else
					{
						nCurSel = mid; // found a match

						// Step back to find first matching
						while(1)
						{
							if (nCurSel > -1)
							{
								strListText = GetAt(nListIds.GetAt(nCurSel))->GetName().Left(nTextLength);
							}
							if (strListText != strToFind.Left(nTextLength) || nCurSel < 0)
							{
								nCurSel++;		// Back to last matching or 0 if gone past start
								break;
							}
							nCurSel--;
						}
						break;
					}
				}
				// Check if match found
				if (nCurSel >= 0)
				{
					break;
				}
				// Check 1 character less off rh end of lookup
				nTextLength--;
			}
			while(nTextLength);
		}
		strToFind = strInput.Left(nTextLength);
		return nCurSel;
	}

	// ---------------------------------------------------------------------------
	int 					AddAll(CDWordArray& nListIds)
	{
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			nListIds.Add(nLoopCount);
		}
		return GetSize();
	}

	// ---------------------------------------------------------------------------
	int 					AddEditorsChoice(CDWordArray& nListIds)
	{
		int nCount = 0;
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsEditorsChoice(0) == true)
			{
				nListIds.Add(nLoopCount);
				nCount++;
			}
		}
		return nCount;
	}

	// ---------------------------------------------------------------------------
	int 					AddCellarSelection(CDWordArray& nListIds)
	{
		int nCount = 0;
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsCellarSelection(0) == true)
			{
				nListIds.Add(nLoopCount);
				nCount++;
			}
		}
		return nCount;
	}

	// ---------------------------------------------------------------------------
	int 					AddBestBuy(CDWordArray& nListIds)
	{
		int nCount = 0;
		for (int nLoopCount = 0; nLoopCount < GetSize(); nLoopCount++)
		{
			if (GetAt(nLoopCount)->IsBestBuy(0) == true)
			{
				nListIds.Add(nLoopCount);
				nCount++;
			}
		}
		return nCount;
	}

	// Variables
private:
	UInt16					m_iHowMany;
	Ttype*					m_pList;
};
#endif
