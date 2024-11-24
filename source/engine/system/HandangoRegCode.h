
/*

	HandangoRegCode.h: interface for the CHandangoRegCode class.

	(c) RW Software 2003

  ---------------------------------------------------------------
*/

#ifndef __APPPREFERENCES_H__
#define __APPPREFERENCES_H__



// CHandangoRegCode

class CHandangoRegCode
{
#define		RPN_STACK_SIZE		256
#define		RPN_TRUE			1
#define		RPN_FALSE			0

#define BIT0	0x00000001
#define BIT1	0x00000002
#define BIT2	0x00000004
#define BIT3	0x00000008
#define BIT4	0x00000010
#define BIT5	0x00000020
#define BIT6	0x00000040
#define BIT7	0x00000080

#define BIT8	0x00000100
#define BIT9	0x00000200
#define BIT10	0x00000400
#define BIT11	0x00000800
#define BIT12	0x00001000
#define BIT13	0x00002000
#define BIT14	0x00004000
#define BIT15	0x00008000

#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000

#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#define RPN_OP_ADD				(BIT31 | BIT0)
#define	RPN_OP_SUB				(BIT31 | BIT1)
#define	RPN_OP_MULT				(BIT31 | BIT2)
#define	RPN_OP_DIV				(BIT31 | BIT3)
#define	RPN_OP_LSHIFT			(BIT31 | BIT4)
#define	RPN_OP_RSHIFT			(BIT31 | BIT5)
#define	RPN_OP_ISNOTEQ			(BIT31 | BIT6)
#define	RPN_OP_ISLESSTHAN		(BIT31 | BIT7)
#define	RPN_OP_ISGREATERTHAN	(BIT31 | BIT8)
#define	RPN_OP_ISEQUAL			(BIT31 | BIT9)
#define	RPN_OP_ISGREATEREQUAL	(BIT31 | BIT10)
#define	RPN_OP_ISLESSEQUAL		(BIT31 | BIT11)
#define	RPN_OP_BITAND			(BIT31 | BIT12)
#define	RPN_OP_BITOR			(BIT31 | BIT13)
#define	RPN_OP_BITXOR			(BIT31 | BIT14)

#define	RPN_VAR_I				(BIT30 | BIT0)
#define	RPN_VAR_C				(BIT30 | BIT1)
#define	RPN_VAR_KEY				(BIT30 | BIT2)
#define	RPN_END					0xffffffff

public:
	CHandangoRegCode();
	virtual ~CHandangoRegCode();

	ushort					DoCalculateRegCode (uint* RPN, char* FullPUN);
	void					RPN_Push(ushort Arg)
	{
    FDASSERT (RPN_StackPtr < RPN_STACK_SIZE);
		RPN_Stack[RPN_StackPtr++] = Arg;
	}


	ushort					RPN_Pop()
	{
    FDASSERT (RPN_StackPtr > 0);
		return (RPN_Stack[--RPN_StackPtr]);
	}

protected:
	int					RPN_StackPtr;
	ushort					RPN_Stack[RPN_STACK_SIZE];
};

#endif
