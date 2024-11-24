//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdpackedbitarray.h
// Description : A templated array type that packs the data using the specified bit width.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDPACKEDBITARRAY_H_INCLUDED
#define FDPACKEDBITARRAY_H_INCLUDED

#include <lemon/math/lmath.h>

template <typename ARRAYTYPE, unsigned int BITSPER, unsigned int NUMELEMENTS, int VALUESHIFT>
class FDPackedBitArray
{
public:
	FDPackedBitArray()
	{
		ClearAll();
	};

	~FDPackedBitArray()
	{
	}

  int GetNumElements()
  {
    return NUMELEMENTS;
  }

	void ClearAll()
	{
		for (unsigned int i = 0; i < GetRawStorageSize(); i++)
			m_storage[i] = 0;
	}

	ARRAYTYPE Get(const unsigned int uiElement) const
	{
		ARRAYTYPE iReturnResult = 0;
		const unsigned int uiStartBit = (uiElement * BITSPER);
		const unsigned int uiStartIndex = uiStartBit / 8;
		unsigned int uiBitsRemainingToGet = BITSPER;
		unsigned int uiBitsGotten = 0;
		unsigned int uiCurrentIndex = uiStartIndex;
		unsigned int uiCurrentBit = uiStartBit % 8;

		while (uiBitsRemainingToGet > 0)
		{
			const unsigned int uiBitsGottenOnThisPass = lmMin(8 - uiCurrentBit, uiBitsRemainingToGet);
			iReturnResult |= GetBitsFromStorage(uiCurrentIndex, uiCurrentBit, uiBitsGottenOnThisPass) << uiBitsGotten;
			uiCurrentBit = 0;
			uiBitsRemainingToGet -= uiBitsGottenOnThisPass;
			uiBitsGotten += uiBitsGottenOnThisPass;
			uiCurrentIndex++;
		}

		return iReturnResult + VALUESHIFT;
	}

	void Set(const unsigned int uiElement, const ARRAYTYPE newValue)
	{
		ARRAYTYPE iValueToSave = newValue - VALUESHIFT;
		const unsigned int uiStartBit = (uiElement * BITSPER);
		const unsigned int uiStartIndex = uiStartBit / 8;
		unsigned int uiBitsRemainingToStore = BITSPER;
		unsigned int uiCurrentIndex = uiStartIndex;
		unsigned int uiCurrentBit = uiStartBit % 8;

		while (uiBitsRemainingToStore > 0)
		{
			const unsigned int uiBitsSavedOnThisPass = lmMin(8 - uiCurrentBit, uiBitsRemainingToStore);
			ARRAYTYPE iValueToSaveThisPass;

			if (uiBitsSavedOnThisPass == 8)
			{
        iValueToSaveThisPass = (ARRAYTYPE)( iValueToSave & 0xFF );
				iValueToSave >>= 8;
			}
			else
			{
        iValueToSaveThisPass = (ARRAYTYPE)( (iValueToSave & ((1 << uiBitsSavedOnThisPass) - 1)) );
				iValueToSave >>= uiBitsSavedOnThisPass;
			}

      lAssert( iValueToSaveThisPass < 256 );
      SetBitsInStorage(uiCurrentIndex, uiCurrentBit, uiBitsSavedOnThisPass, (unsigned char)iValueToSaveThisPass);
			uiCurrentBit = 0;
			uiBitsRemainingToStore -= uiBitsSavedOnThisPass;
			uiCurrentIndex++;
		}
	}

	unsigned char *GetRawStorage()
	{
		return m_storage;
	}

	unsigned int GetRawStorageSize() const
	{
		return ((NUMELEMENTS * BITSPER) + 7) / 8;
	}
protected:
	unsigned char GetBitsFromStorage(const unsigned int uiStorageIndex, const unsigned int uiStartBit, const unsigned int uiNumBits) const
	{
    unsigned char ucResult = (unsigned char)(m_storage[uiStorageIndex] >> uiStartBit);
    if (uiNumBits < 8)
		{
			ucResult &= (1 << uiNumBits) - 1;
		}

		return ucResult;
	}

	void SetBitsInStorage(const unsigned int uiStorageIndex, const unsigned int uiStartBit, const unsigned int uiNumBits, const unsigned char uiValue)
	{
    const unsigned char ucNewChar = (unsigned char)(uiValue << uiStartBit);
    const unsigned char ucMask    = (unsigned char)(((1 << uiNumBits) - 1) << uiStartBit);
		unsigned char ucCurrentValue = GetBitsFromStorage(uiStorageIndex, 0, 8);

		m_storage[uiStorageIndex] = (ucCurrentValue & ~ucMask) | ucNewChar;
	}

	unsigned char m_storage[((NUMELEMENTS * BITSPER) + 7) / 8];
};

template <unsigned int NUMSTRINGS, unsigned int MAXSTRINGLENGTH>
class FDPackedNameCharBitArray : public FDPackedBitArray<unsigned char, 7, NUMSTRINGS * MAXSTRINGLENGTH, 0>
{
public:
	FDPackedNameCharBitArray()
	{
	}

	const char *GetString(const unsigned int uiStringIndex)
	{
		for (unsigned int i = 0; i < MAXSTRINGLENGTH; i++)
		{
      unsigned char ucTemp = this->Get((uiStringIndex * MAXSTRINGLENGTH) + i);
			if (ucTemp != 0)
			{
				ucTemp += 31;
			}
			ms_tempBuffer[i] = ucTemp;
		}

		return ms_tempBuffer;
	}

	void SetString(const unsigned int uiStringIndex, const char *pcString)
	{
		if (pcString == NULL)
		{
      this->Set((uiStringIndex * MAXSTRINGLENGTH), 0);
			return;
		}

		const char *pcIter = pcString;
		unsigned int i = 0;

		while ((*pcIter != 0) && (i < (MAXSTRINGLENGTH - 1)))
		{
			unsigned char ucTemp = *pcIter;
			if (ucTemp != 0)
			{
				ucTemp -= 31;
			}

      this->Set((uiStringIndex * MAXSTRINGLENGTH) + i, ucTemp);
			pcIter++;
			i++;
		}

    this->Set((uiStringIndex * MAXSTRINGLENGTH) + i, 0);
	}
private:
	char ms_tempBuffer[MAXSTRINGLENGTH];
};

#endif // #ifndef FDPACKEDBITARRAY_H_INCLUDED


