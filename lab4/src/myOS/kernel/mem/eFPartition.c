#include "../../include/myPrintk.h"
#define asign_byte (4)                                      //对齐字节
#define eFPartitionsize (sizeof(eFPartition))               //内存结构体大小
#define EEBsize (sizeof(EEB))                               //块结构体大小

// 一个EEB表示一个空闲可用的Block
typedef struct
{
	unsigned long next_start;
	int isfree;
}EEB;

void showEEB(EEB* eeb)
{
	myPrintk(GREY, "EEB(address=0x%x, isfree=%d, next=0x%x)\n", eeb, eeb->isfree, eeb->next_start);
}

//eFPartition是表示整个内存的数据结构
typedef struct
{
	unsigned long totalN;
	unsigned long perSize;  //unit: byte    
	unsigned long firstEEB;
}eFPartition;

//打印内存整体信息
void showeFPartition(eFPartition* efp)
{
	myPrintk(PURPLE, "eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstEEB=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstEEB);
}

//打印内存信息
void eFPartitionWalkByAddr(unsigned long efpHandler)
{
	efpHandler -= eFPartitionsize;
	//打印内存整体信息
	showeFPartition((eFPartition*)efpHandler);
	//打印块信息
	unsigned long p = ((eFPartition*)efpHandler)->firstEEB;
	EEB* eebpoint;
	while (p != 0)
	{
		eebpoint = (EEB*)p;
		showEEB(eebpoint);
		p = eebpoint->next_start;
	}
}

//计算内存大小
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n)
{
	return ((((perSize - 1) / asign_byte) + 1) * asign_byte + EEBsize) * n + eFPartitionsize;
}

//内存初始化
unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n)
{
	myPrintk(DARK_GREEN, "The size of eFPartitionpoint is 0x%x bytes, the size of EEB is 0x%x bytes\n", eFPartitionsize, EEBsize);
	//创建一个eFPartition结构体 
	eFPartition* partitionpoint = (eFPartition*)start;
	unsigned long realperSize = ((perSize - 1) / asign_byte + 1) * asign_byte + EEBsize;
	partitionpoint->perSize = realperSize;
	partitionpoint->totalN = n;
	unsigned long p = start + eFPartitionsize;
	partitionpoint->firstEEB = p;
	//对每一块的内存创建EEB连成链
	for (int i = 0; i < n; i++)
	{
		EEB* eebpoint = (EEB*)p;
		p += realperSize;
		eebpoint->isfree = 1;
		if (i < n - 1)
			eebpoint->next_start = p;
		else
			eebpoint->next_start = 0;
	}
	return partitionpoint->firstEEB;
}

//分配一个空闲块的内存并返回相应的地址
unsigned long eFPartitionAlloc(unsigned long EFPHandler)
{
	EFPHandler -= eFPartitionsize;
	//记录返回地址
	unsigned long p = ((eFPartition*)EFPHandler)->firstEEB;
	EEB* eebpoint;
	//检测下一个空闲块
	while (p != 0)
	{
		eebpoint = (EEB*)p;
		if (eebpoint->isfree == 1)
		{
			eebpoint->isfree = 0;
			return p + EEBsize;
		}
		p = eebpoint->next_start;
	}
	return 0;
}

unsigned long eFPartitionFree(unsigned long EFPHandler, unsigned long mbStart)
{
	EFPHandler -= eFPartitionsize;
	mbStart -= EEBsize;
	//查找指定块
	unsigned long p = ((eFPartition*)EFPHandler)->firstEEB;
	EEB* eebpoint;
	while (p != 0)
	{
		myPrintk(GREEN, "now addr is 0x%x\n", p);
		eebpoint = (EEB*)p;
		if (p == mbStart)
		{	
			eebpoint->isfree = 1;
			return 1;
		}
		p = eebpoint->next_start;
	}
	return 0;
}
