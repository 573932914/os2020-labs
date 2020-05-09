#include "../../include/myPrintk.h"
#define asign_byte (4)                                      //对齐字节
#define dPartitionsize (sizeof(dPartition))                 //内存结构体大小
#define EMBsize (sizeof(EMB))                               //块结构体大小

//整个动态分区内存的数据结构
typedef struct
{
	unsigned long size;
	unsigned long EMBStart;
}dPartition;

void showdPartition(dPartition* dp)
{
	myPrintk(PURPLE, "dPartition(start=0x%x, size=0x%x, EMBStart=0x%x)\n", dp, dp->size, dp->EMBStart);
}

//每一个block的数据结构
typedef struct
{
	unsigned long size;
	union
	{
		unsigned long nextStart;
		unsigned long userData;
	};
	int isfree;
}EMB;

void showEMB(EMB* emb)
{
	myPrintk(BLUE, "EMB(start=0x%x, size=0x%x, nextStart=0x%x, isfree=%d)\n", emb, emb->size, emb->nextStart, emb->isfree);
}

//内存初始化
unsigned long dPartitionInit(unsigned long start, unsigned long totalSize)
{
	//判断大小
	if (totalSize <= dPartitionsize + EMBsize)
		return 0;
	//打印内存相关结构体的大小信息
	myPrintk(DARK_GREEN, "The size of dPartitionpoint is 0x%x bytes, the size of EMB is 0x%x bytes\n", dPartitionsize, EMBsize);
	//创建一个dPartition结构体 
	dPartition* dPartitionpoint = (dPartition*)start;
	dPartitionpoint->EMBStart = start + dPartitionsize;
	dPartitionpoint->size = totalSize - dPartitionsize;
	//分配整块EMB
	EMB* embpoint = (EMB*)dPartitionpoint->EMBStart;
	embpoint->size = totalSize - dPartitionsize;
	embpoint->nextStart = 0;
	embpoint->isfree = 1;
	return dPartitionpoint->EMBStart;
}

void dPartitionWalkByAddr(unsigned long dp)
{
	dp -= dPartitionsize;
	//打印内存整体信息
	showdPartition((dPartition*)dp);
	//遍历打印块信息
	unsigned long p = ((dPartition*)dp)->EMBStart;
	EMB* embpoint;
	while (p != 0)
	{
		embpoint = (EMB*)p;
		showEMB(embpoint);
		p = embpoint->nextStart;
	}
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size)
{
	dp -= dPartitionsize;
	//计算对齐所需大小
	unsigned long realsize = ((size - 1) / asign_byte + 1) * asign_byte + EMBsize;
	//first fit查找块
	unsigned long p = ((dPartition*)dp)->EMBStart;
	EMB* embpoint;
	while (p != 0)
	{
		embpoint = (EMB*)p;
		if (embpoint->isfree == 1 && embpoint->size >= realsize)
		{
			embpoint->isfree = 0;
			//考虑是否分割块
			if (embpoint->size > realsize + EMBsize)
			{
				((EMB*)(p + realsize))->isfree = 1;
				((EMB*)(p + realsize))->nextStart = embpoint->nextStart;
				((EMB*)(p + realsize))->size = embpoint->size - realsize;
				embpoint->nextStart = p + realsize;
				embpoint->size = realsize;
			}
			return p + EMBsize;
		}
		p = embpoint->nextStart;
	}
	return 0;
}

//按照对应的fit的算法释放空间
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start)
{
	dp -= dPartitionsize;
	start -= EMBsize;
	unsigned long p = ((dPartition*)dp)->EMBStart;
	EMB* embpoint;
	unsigned long lastEMBsize = 0;
	while (p != 0)
	{
		embpoint = (EMB*)p;
		//检测到地址
		if (p == start)
		{
			//向上合并
			if (lastEMBsize > 0)
			{
				unsigned long lastp = p - lastEMBsize;
				EMB* lastembpoint = (EMB*)lastp;
				lastembpoint->size += embpoint->size;
				lastembpoint->nextStart = embpoint->nextStart;
				p = lastp;
				embpoint = (EMB*)lastp;
			}
			else
				embpoint->isfree = 1;
			//向下合并
			if (embpoint->nextStart != 0)
			{
				unsigned long nextp = embpoint->nextStart;
				EMB* nextembpoint = (EMB*)nextp;
				if (nextembpoint->isfree == 1)
				{
					embpoint->size += nextembpoint->size;
					embpoint->nextStart = nextembpoint->nextStart;
				}
			}
			return 1;
		}
		//若当前块空闲则记录大小
		if (embpoint->isfree == 1)
			lastEMBsize = embpoint->size;
		else
			lastEMBsize = 0;
		p = embpoint->nextStart;
	}
	return 0;
}

unsigned long dPartitionAlloc(unsigned long dp, unsigned long size)
{
	return dPartitionAllocFirstFit(dp, size);
}

unsigned long dPartitionFree(unsigned long dp, unsigned long start)
{
	return dPartitionFreeFirstFit(dp, start);
}
