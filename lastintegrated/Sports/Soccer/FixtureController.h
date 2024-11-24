
/*
	File:- FixtureController.h

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#if !defined(AFX_FIXTURECONTROLLER_H__EAAE4AB4_9C06_4A52_A40B_3438B9B9D09F__INCLUDED_)
#define AFX_FIXTURECONTROLLER_H__EAAE4AB4_9C06_4A52_A40B_3438B9B9D09F__INCLUDED_

    const ushort			FIXT26MATCHDAYS = 50;
    const ushort			FIXT26CLUBS = 26;
    const ushort			FIXT24MATCHDAYS = 46;
    const ushort			FIXT24CLUBS = 24;
    const ushort			FIXT22MATCHDAYS = 42;
    const ushort			FIXT22CLUBS = 22;
    const ushort			FIXT20MATCHDAYS = 38;
    const ushort			FIXT20CLUBS = 20;
    const ushort			FIXT19MATCHDAYS = 38;
    const ushort			FIXT19CLUBS = 18;		// Only 9 games per date, 1 club rests
    const ushort			FIXT18MATCHDAYS = 34;
    const ushort			FIXT18CLUBS = 18;
    const ushort			FIXT17MATCHDAYS = 34;
    const ushort			FIXT17CLUBS = 16;		// Only 8 games per date, 1 club rests
    const ushort			FIXT16MATCHDAYS = 30;
    const ushort			FIXT16CLUBS = 16;
    const ushort			FIXT15MATCHDAYS = 30;
    const ushort			FIXT15CLUBS = 14;		// Only 7 games per date, 1 club rests
    const ushort			FIXT14MATCHDAYS = 26;
    const ushort			FIXT14CLUBS = 14;
    const ushort			FIXT13MATCHDAYS = 26;
    const ushort			FIXT13CLUBS = 12;		// Only 6 games per date, 1 club rests
    const ushort			FIXT12MATCHDAYS = 22;
    const ushort			FIXT12CLUBS = 12;
    const ushort			FIXT11MATCHDAYS = 22;
    const ushort			FIXT11CLUBS = 10;		// Only 5 games per date, 1 club rests
    const ushort			FIXT10MATCHDAYS = 18;
    const ushort			FIXT10CLUBS = 10;
    const ushort			FIXT09MATCHDAYS = 18;
    const ushort			FIXT09CLUBS =  8;		// Only 4 games per date, 1 club rests
    const ushort			FIXT08MATCHDAYS = 14;
    const ushort			FIXT08CLUBS =  8;
    const ushort			FIXT06MATCHDAYS = 10;
    const ushort			FIXT06CLUBS =  6;


class CFixtureController  
{
public:
	CFixtureController();

	CNumericArray<byte>&	GetFixtureList(const int _nNumberOfClubs);

protected:
    CNumericArray<byte>	m_FixtureList24Clubs;
    CNumericArray<byte>	m_FixtureList20Clubs;
};

#endif // !defined(AFX_FIXTURECONTROLLER_H__EAAE4AB4_9C06_4A52_A40B_3438B9B9D09F__INCLUDED_)
