#include "../../include/myPrintk.h"
#include "../../include/mem.h"
#include "../../include/io.h"

unsigned long pMemStart;               //可用的内存的起始地址
unsigned long pMemSize;                //可用的大小

void memTest(unsigned long start, unsigned long grainSize)
{
	//检测范围
	if (start <= 1 << 20)
		start = 1 << 20;
	if (grainSize < (1 << 10))
		grainSize = 1 << 10;
	unsigned long addr = start;
	//每个步长检测
	unsigned short a;
	unsigned long i = 0;
	for (;; i++)
	{
		//检测头两字节
		a = *((short int*)addr);
		*((unsigned short*)addr) = (unsigned short)0xAA55;
		if (*((unsigned short*)addr) != (unsigned short)0xAA55)
		{
			break;
		}
		*((unsigned short*)addr) = (unsigned short)0x55AA;
		if (*((unsigned short*)addr) != (unsigned short)0x55AA)
		{
			break;
		}
		//写回
		*((unsigned short*)addr) = a;
		//检测后两字节
		addr += grainSize - 2;	
		a = *((short int*)addr);
		*((unsigned short*)addr) = (unsigned short)0xAA55;
		if (*((unsigned short*)addr) != (unsigned short)0xAA55)
			break;
		*((unsigned short*)addr) = (unsigned short)0x55AA;
		if (*((unsigned short*)addr) != (unsigned short)0x55AA)
			break;
		//写回
		*((short int*)addr) = a;
		addr += 2;
	}
	//打印信息
	pMemStart = start;
	pMemSize = addr - start;
	myPrintk(GREY, "MemStart: 0x%x  \n", pMemStart);
	myPrintk(GREY, "MemSize:  0x%x  \n", pMemSize);
}

extern unsigned long _end;
void pMemInit(void)
{
	unsigned long _end_addr = (unsigned long)& _end;
	memTest(0x100000, 0x1000);
	myPrintk(GREY, "_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr)
	{
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}
	pMemHandler = dPartitionInit(pMemStart, pMemSize);
}
