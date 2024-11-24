/******************************************************************************
 *
 * Copyright(c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DataMgr.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header for the Data Manager
 *
 * History:
 *   	11/14/94  RM - Created by Ron Marianetti
 *
 *****************************************************************************/

#ifndef __DATAMGR_H__
#define __DATAMGR_H__


#define MemHandle char*
#define Err UInt32
#define Char char
#define Boolean bool
#define MemPtr char*
#define LocalID UInt16

MemHandle MemHandleNew(Int32 nBufferSize);
MemHandle MemHandleLock(MemHandle pBuffer);
Err MemHandleUnlock(MemHandle pBuffer);
//void ErrFatalDisplayIf(Err ErrorNum, LPCSTR szError)        { FDASSERT(0); }

typedef UInt32	DmResType;
typedef UInt16	DmResID;

/************************************************************
 * Category equates
 *************************************************************/
#define	dmRecAttrCategoryMask	0x0F	// mask for category #
#define	dmRecNumCategories		16		// number of categories
#define	dmCategoryLength		16		// 15 chars + 1 null terminator

#define  dmAllCategories		0xff
#define  dmUnfiledCategory 		0

#define	dmMaxRecordIndex		0xffff



// Record Attributes
//
// *** IMPORTANT:
// ***
// *** Any changes to record attributes must be reflected in dmAllRecAttrs and dmSysOnlyRecAttrs ***
// ***
// *** Only one nibble is available for record attributes
//
#define	dmRecAttrDelete			0x80	// delete this record next sync
#define	dmRecAttrDirty			0x40	// archive this record next sync
#define	dmRecAttrBusy			0x20	// record currently in use
#define	dmRecAttrSecret			0x10	// "secret" record - password protected


// All record atributes(for error-checking)
#define	dmAllRecAttrs	(dmRecAttrDelete |			\
						 dmRecAttrDirty |			\
						 dmRecAttrBusy |			\
						 dmRecAttrSecret )

// Record attributes which only the system is allowed to change(for error-checking)
#define	dmSysOnlyRecAttrs( dmRecAttrBusy )


/************************************************************
 * Database Header equates
 *************************************************************/
#define	dmDBNameLength				32			// 31 chars + 1 null terminator

// Attributes of a Database
//
// *** IMPORTANT:
// ***
// *** Any changes to database attributes must be reflected in dmAllHdrAttrs and dmSysOnlyHdrAttrs ***
// ***
#define	dmHdrAttrResDB				0x0001	// Resource database
#define	dmHdrAttrReadOnly			0x0002	// Read Only database
#define	dmHdrAttrAppInfoDirty		0x0004	// Set if Application Info block is dirty
															// Optionally supported by an App's conduit
#define	dmHdrAttrBackup				0x0008	//	Set if database should be backed up to PC if
															//	no app-specific synchronization conduit has
															//	been supplied.
#define	dmHdrAttrOKToInstallNewer 	0x0010	// This tells the backup conduit that it's OK
															//  for it to install a newer version of this database
															//  with a different name if the current database is
															//  open. This mechanism is used to update the
															//  Graffiti Shortcuts database, for example.
#define	dmHdrAttrResetAfterInstall	0x0020 	// Device requires a reset after this database is
															// installed.
#define	dmHdrAttrCopyPrevention		0x0040	// This database should not be copied to

#define	dmHdrAttrStream				0x0080	// This database is used for file stream implementation.
#define	dmHdrAttrHidden				0x0100	// This database should generally be hidden from view
															//  used to hide some apps from the main view of the
															//  launcher for example.
															// For data (non-resource) databases, this hides the record
															//	 count within the launcher info screen.
#define	dmHdrAttrLaunchableData		0x0200	// This data database(not applicable for executables)
															//  can be "launched" by passing it's name to it's owner
															//  app('appl' database with same creator) using
															//  the sysAppLaunchCmdOpenNamedDB action code.

#define	dmHdrAttrRecyclable			0x0400	// This database(resource or record) is recyclable:
															//  it will be deleted Real Soon Now, generally the next
															//  time the database is closed.

#define	dmHdrAttrBundle				0x0800	// This database(resource or record) is associated with
															// the application with the same creator. It will be beamed
															// and copied along with the application.

#define	dmHdrAttrOpen				0x8000	// Database not closed properly


// All database atributes(for error-checking)
#define	dmAllHdrAttrs (	dmHdrAttrResDB |						\
						dmHdrAttrReadOnly |					\
						dmHdrAttrAppInfoDirty |				\
						dmHdrAttrBackup |						\
						dmHdrAttrOKToInstallNewer |		\
						dmHdrAttrResetAfterInstall |		\
						dmHdrAttrCopyPrevention |			\
						dmHdrAttrStream |						\
						dmHdrAttrHidden |						\
						dmHdrAttrLaunchableData |			\
						dmHdrAttrRecyclable |				\
						dmHdrAttrBundle |						\
						dmHdrAttrOpen	)

// Database attributes which only the system is allowed to change(for error-checking)
#define	dmSysOnlyHdrAttrs				(	dmHdrAttrResDB | dmHdrAttrOpen	)


/************************************************************
 * Unique ID equates
 *************************************************************/
#define	dmRecordIDReservedRange		1			// The range of upper bits in the database's
												// uniqueIDSeed from 0 to this number are
												// reserved and not randomly picked when a
												// database is created.
#define	dmDefaultRecordsID			0			// Records in a default database are copied
												// with their uniqueIDSeeds set in this range.
#define	dmUnusedRecordID			0			// Record ID not allowed on the device


/************************************************************
 * Mode flags passed to DmOpenDatabase
 *************************************************************/
#define	dmModeReadOnly			0x0001		// read  access
#define	dmModeWrite				0x0002		// write access
#define	dmModeReadWrite			0x0003		// read & write access
#define	dmModeLeaveOpen			0x0004		// leave open when app quits
#define	dmModeExclusive			0x0008		// don't let anyone else open it
#define	dmModeShowSecret		0x0010		// force show of secret records

// Generic type used to represent an open Database
class CEndianFile;
typedef CEndianFile*					DmOpenRef;


/************************************************************
 * Structure passed to DmGetNextDatabaseByTypeCreator and used
 *  to cache search information between multiple searches.
 *************************************************************/
	typedef struct
	{
		UInt32		info[8];
	} DmSearchStateType;
typedef DmSearchStateType*	DmSearchStatePtr;



/************************************************************
 * Structures used by the sorting routines
 *************************************************************/
	typedef struct
	{
		UInt8			attributes;							// record attributes;
		UInt8			uniqueID[3];						// unique ID of record
	} SortRecordInfoType;
typedef SortRecordInfoType *SortRecordInfoPtr;

typedef Int16 DmComparF(void *, void *, Int16 other, SortRecordInfoPtr, SortRecordInfoPtr, MemHandle appInfoH);



/************************************************************
 * Database manager error codes
 * the constant dmErrorClass is defined in ErrorBase.h
 *************************************************************/
#define	dmErrMemError 1
#define	dmErrIndexOutOfRange 2
#define	dmErrInvalidParam 3
#define	dmErrReadOnly 4
#define	dmErrDatabaseOpen 5
#define	dmErrCantOpen 6
#define	dmErrCantFind 7
#define	dmErrRecordInWrongCard 8
#define	dmErrCorruptDatabase 9
#define	dmErrRecordDeleted 10
#define	dmErrRecordArchived	11
#define	dmErrNotRecordDB 12
#define	dmErrNotResourceDB 13
#define	dmErrROMBased 14
#define	dmErrRecordBusy	15
#define	dmErrResourceNotFound 16
#define	dmErrNoOpenDatabase	17
#define	dmErrInvalidCategory 18
#define	dmErrNotValidRecord	19
#define	dmErrWriteOutOfBounds 20
#define	dmErrSeekFailed	21
#define	dmErrAlreadyOpenForWrites 22
#define	dmErrOpenedByAnotherTask 23
#define dmErrUniqueIDNotFound 24
#define dmErrAlreadyExists 25
#define	dmErrInvalidDatabaseName 26
#define	dmErrDatabaseProtected 27
#define	dmErrDatabaseNotProtected 28

/************************************************************
 * Values for the direction parameter of DmSeekRecordInCategory
 *************************************************************/
#define dmSeekForward 1
#define dmSeekBackward -1


/************************************************************
 * Data Manager procedures
 *************************************************************/

// Initialization
Err			DmInit(void);

// Directory Lists
Err			DmCreateDatabase(UInt16 cardNo, const Char *nameP, UInt32 creator, UInt32 type, Boolean resDB);
Err			DmCreateDatabaseFromImage(MemPtr bufferP);
Err			DmDeleteDatabase(UInt16 cardNo, LocalID dbID);
UInt16		DmNumDatabases(UInt16 cardNo);
LocalID		DmGetDatabase(UInt16 cardNo, UInt16 index);
LocalID		DmFindDatabase(UInt16 cardNo, const Char *nameP);
Err			DmGetNextDatabaseByTypeCreator(Boolean newSearch, DmSearchStatePtr stateInfoP, UInt32	type, UInt32 creator, Boolean onlyLatestVers, UInt16 *cardNoP, LocalID *dbIDP);

// Database info
Err			DmDatabaseInfo(UInt16 cardNo, LocalID	dbID, Char *nameP, UInt16 *attributesP, UInt16 *versionP, UInt32 *crDateP,
						   UInt32 *	modDateP, UInt32 *bckUpDateP, UInt32 *	modNumP, LocalID *appInfoIDP,
						   LocalID *sortInfoIDP, UInt32 *typeP, UInt32 *creatorP);

Err			DmSetDatabaseInfo(UInt16 cardNo, LocalID	dbID, const Char *nameP, UInt16 *attributesP, UInt16 *versionP, UInt32 *crDateP,
							  UInt32* modDateP, UInt32 *bckUpDateP, UInt32 *	modNumP, LocalID *appInfoIDP,
							  LocalID *sortInfoIDP, UInt32 *typeP, UInt32 *creatorP);
Err			DmDatabaseSize(UInt16 cardNo, LocalID dbID, UInt32* numRecordsP, UInt32* totalBytesP, UInt32* dataBytesP);


// This routine can be used to prevent a database from being deleted(by passing
//  true for 'protect'). It will increment the protect count if 'protect' is true
//  and decrement it if 'protect' is false. This is used by code that wants to
//  keep a particular record or resource in a database locked down but doesn't
//  want to keep the database open. This information is keep in the dynamic heap so
//  all databases are "unprotected" at system reset.
Err			DmDatabaseProtect(UInt16 cardNo, LocalID dbID, Boolean protect);

// Open/close Databases
DmOpenRef	DmOpenDatabase(UInt16 cardNo, LocalID dbID, UInt16 mode);
DmOpenRef	DmOpenDatabaseByTypeCreator(UInt32 type, UInt32 creator, UInt16 mode);
DmOpenRef	DmOpenDBNoOverlay(UInt16 cardNo, LocalID dbID, UInt16 mode);
Err			DmCloseDatabase(DmOpenRef dbP);


// Info on open databases
DmOpenRef	DmNextOpenDatabase(DmOpenRef currentP);
Err			DmOpenDatabaseInfo(DmOpenRef dbP, LocalID *dbIDP, UInt16 *openCountP, UInt16 *modeP, UInt16 *cardNoP, Boolean *resDBP);
LocalID		DmGetAppInfoID (DmOpenRef dbP);
void		DmGetDatabaseLockState(DmOpenRef dbR, UInt8 *highest, UInt32 *count, UInt32 *busy);

// Utility to unlock all records and clear busy bits
Err			DmResetRecordStates(DmOpenRef dbP);


// Error Query
Err			DmGetLastErr(void);


//------------------------------------------------------------
// Record based access routines
//------------------------------------------------------------

// Record Info
UInt16		DmNumRecords(DmOpenRef dbP);
UInt16		DmNumRecordsInCategory(DmOpenRef dbP, UInt16 category);
Err			DmRecordInfo(DmOpenRef dbP, UInt16 index, UInt16 *attrP, UInt32 *uniqueIDP, LocalID *chunkIDP);
Err			DmSetRecordInfo(DmOpenRef dbP, UInt16 index, UInt16 *attrP, UInt32 *uniqueIDP);


// Record attaching and detaching
Err			DmAttachRecord(DmOpenRef dbP, UInt16 *atP, MemHandle newH, MemHandle *oldHP);
Err			DmDetachRecord(DmOpenRef dbP, UInt16 index, MemHandle *oldHP);
Err			DmMoveRecord(DmOpenRef dbP, UInt16 from, UInt16 to);


// Record creation and deletion
MemHandle	DmNewRecord(DmOpenRef dbP, UInt16 *atP, UInt32 size);
Err			DmRemoveRecord(DmOpenRef dbP, UInt16 index);
Err			DmDeleteRecord(DmOpenRef dbP, UInt16 index);
Err			DmArchiveRecord(DmOpenRef dbP, UInt16 index);
MemHandle	DmNewHandle(DmOpenRef dbP, UInt32 size);
Err			DmRemoveSecretRecords(DmOpenRef dbP);


// Record viewing manipulation
Err			DmFindRecordByID(DmOpenRef dbP, UInt32 uniqueID, UInt16 *indexP);
MemHandle	DmQueryRecord(DmOpenRef dbP, UInt16 index);
MemHandle	DmGetRecord(DmOpenRef dbP, UInt16 index);
MemHandle	DmQueryNextInCategory(DmOpenRef dbP, UInt16 *indexP, UInt16 category);
UInt16		DmPositionInCategory(DmOpenRef dbP, UInt16 index, UInt16 category);
Err			DmSeekRecordInCategory(DmOpenRef dbP, UInt16 *indexP, UInt16 offset, Int16 direction, UInt16 category);
MemHandle	DmResizeRecord(DmOpenRef dbP, UInt16 index, UInt32 newSize);
Err			DmReleaseRecord(DmOpenRef dbP, UInt16 index, Boolean dirty);
UInt16		DmSearchRecord(MemHandle recH, DmOpenRef *dbPP);


// Category manipulation
Err			DmMoveCategory(DmOpenRef dbP, UInt16 toCategory, UInt16 fromCategory, Boolean dirty);
Err			DmDeleteCategory(DmOpenRef dbR, UInt16 categoryNum);


// Validation for writing
Err			DmWriteCheck(void *recordP, UInt32 offset, UInt32 bytes);

// Writing
Err			DmWrite(void *recordP, UInt32 offset, const void *srcP, UInt32 bytes);
Err			DmStrCopy(void *recordP, UInt32 offset, const Char *srcP);
Err			DmSet(void *recordP, UInt32 offset, UInt32 bytes, UInt8 value);


//------------------------------------------------------------
// Resource based access routines
//------------------------------------------------------------

// High level access routines
MemHandle	DmGetResource(DmResType type, DmResID resID);
MemHandle	DmGet1Resource(DmResType type, DmResID resID);
Err			DmReleaseResource(MemHandle resourceH);
MemHandle	DmResizeResource(MemHandle resourceH, UInt32 newSize);


// Searching resource databases
DmOpenRef	DmNextOpenResDatabase(DmOpenRef dbP);
UInt16		DmFindResourceType(DmOpenRef dbP, DmResType resType, UInt16 typeIndex);
UInt16		DmFindResource(DmOpenRef dbP, DmResType resType, DmResID resID, MemHandle resH);
UInt16		DmSearchResource(DmResType resType, DmResID resID, MemHandle resH, DmOpenRef *dbPP);


// Resource Info
UInt16		DmNumResources(DmOpenRef dbP);
Err			DmResourceInfo(DmOpenRef dbP, UInt16 index, DmResType *resTypeP, DmResID *resIDP, LocalID *chunkLocalIDP);
Err			DmSetResourceInfo(DmOpenRef dbP, UInt16 index, DmResType *resTypeP, DmResID *resIDP);


// Resource attaching and detaching
Err			DmAttachResource(DmOpenRef dbP, MemHandle newH, DmResType resType, DmResID resID);
Err			DmDetachResource(DmOpenRef dbP, UInt16 index, MemHandle *oldHP);


// Resource creation and deletion
MemHandle	DmNewResource(DmOpenRef dbP, DmResType resType, DmResID resID, UInt32 size);
Err			DmRemoveResource(DmOpenRef dbP, UInt16 index);


// Resource manipulation
MemHandle	DmGetResourceIndex(DmOpenRef dbP, UInt16 index);


// Record sorting
Err 		DmQuickSort(DmOpenRef dbP, DmComparF *compar, Int16 other);
Err			DmInsertionSort(DmOpenRef dbR, DmComparF *compar, Int16 other);
UInt16		DmFindSortPosition(DmOpenRef dbP, void *newRecord, SortRecordInfoPtr newRecordInfo, DmComparF *compar, Int16 other);
UInt16		DmFindSortPositionV10(DmOpenRef dbP, void *newRecord, DmComparF *compar, Int16 other);

#endif // __DATAMGR_H__
