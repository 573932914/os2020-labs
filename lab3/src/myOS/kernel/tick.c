#include "../kernel/wallClock.h"

static unsigned long int TickCount = 0;

//获取计数
int getTickCount()
{
	return TickCount;
}

//每秒被调用100次的技术函数
void tick(void)
{
	TickCount += 1;
	//达到100次则更新时钟
	if (TickCount >= 100)
	{
		int h, m, s;
		getWallClock(&h, &m, &s);
		setWallClock(h, m, s + (TickCount / 100));
		TickCount %= 100;
	}
	//更新时钟毫秒
	setms(TickCount * 10);
}