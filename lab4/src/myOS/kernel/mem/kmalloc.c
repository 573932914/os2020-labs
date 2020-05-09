#include "../../include/mem.h"

unsigned long kmalloc(unsigned long size)
{
	dPartitionAlloc(pMemHandler, size);
}

unsigned long kfree(unsigned long start)
{
	dPartitionFree(pMemHandler, start);
}