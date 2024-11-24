
/*
		StadiumPlan.cpp: implementation of the StadiumPlan class.

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "StadiumPlan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//---------------------------------------------------------------------------------------
CStadiumPlan::CStadiumPlan(CDataFile* File)
{
    myFile->Read(&m_ID, sizeof(m_ID));
    m_Plan.Read(File);
}

///////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------
CStadiumPlanList::CStadiumPlanList()
{
    CDataFile* File = myfopen(AddPath("Data\\Plans.bin"), "rb");
    ASSERT (File != NULL);        // File missing
	int HowMany;
    myFile->Read(&HowMany, sizeof(HowMany));
    SetSize(HowMany);
    for (uint32 ListPos = 0; ListPos < HowMany; ListPos ++) {
        SetAt(ListPos, FD_ENGINE_NEW CStadiumPlan(File));
   }
   myfclose(File);
}


//---------------------------------------------------------------------------------------
CStadiumPlanList::~CStadiumPlanList()
{
    CStadiumPlan* cPtr;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++) {
        cPtr = GetAt(ListPos);
		if (cPtr != NULL) {
			delete cPtr;
		}
	}
}

