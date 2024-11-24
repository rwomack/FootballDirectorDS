
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
CStadiumPlan::CStadiumPlan(FILE* File)
{
    myfread(&m_ID, sizeof m_ID, 1, File);
    m_Plan.Read(File);
}

///////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------
CStadiumPlanList::CStadiumPlanList()
{
    FILE* File = myfopen(AddPath("Data\\Plans.bin"), "rb");
    ASSERT (File != NULL);        // File missing
	int HowMany;
    myfread(&HowMany, sizeof HowMany, 1, File);
    SetSize(HowMany);
    for (uint32 ListPos = 0; ListPos < HowMany; ListPos ++) {
        SetAt(ListPos, new CStadiumPlan(File));
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

