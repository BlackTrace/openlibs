/*
���comm_dll��comm_sys���еĵ����ʺϷ���comm.h�е���Ϣ
*/
#if !defined(COMM_EXT_H)
	#define COMM_EXT_H
	
	struct _XDT
	{
		WORD Limit;
		PVOID Base;
	}__attribute__ ((packed));	//ǿ�Ƹýṹ���ֽ����ȶ��룬����Ѱַ�ṹԪ�ش���
	
	typedef struct _XDT XDT,*PXDT;
	
	typedef struct _GATE
	{
		USHORT OffsetLo;
		USHORT Selector;
		BYTE Count;
		//BYTE Type;
		BYTE Type:4;
		BYTE DT:1;
		BYTE DPL:2;
		BYTE P:1;
		USHORT OffsetHi;
	}GATE,*PGATE;
	
	#define FULLOFFSET(gate) ((gate).OffsetHi<<16|(gate).OffsetLo)
	
	typedef struct _SEGMENT
	{
		USHORT LimitLo;
		USHORT BaseLo;
		BYTE BaseMi;
		BYTE Type:4;
		BYTE DT:1;
		BYTE DPL:2;
		BYTE P:1;
		BYTE LimitHi:4;
		BYTE AVL:1;
		BYTE X:1;
		BYTE D:1;
		BYTE G:1;
		BYTE BaseHi;
	}SEGMENT,*PSEGMENT;
	
	#define FULLBASE(seg) ((seg).BaseHi<<24|(seg).BaseMi<<16|(seg).BaseLo)
	#define FULLLIMIT(seg) ((seg).LimitHi<<16|(seg).LimitLo)
#endif
