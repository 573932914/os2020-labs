#include "../lib/color.h"
extern void append2screenAtpoint(int line, int column, char* str, int color);
extern int myvsprintf(char* buf, const char* format, ...);

int hh = 0, mm = 0, ss = 0, ms = 0;

//采用hook机制处理时钟设置
void setWallClockHook(void (*func)(void))
{
	(*func)();
}

//更新时钟的显示
void UpdateWallClock(void)
{
	char buf[9];
	myvsprintf(buf, "%t", hh, mm, ss);
	append2screenAtpoint(24, 71, buf, YELLOW);
}

//设置时钟
void setWallClock(int h, int m, int s)
{
	//进位处理
	ss = s % 60;
	m += s / 60;
	mm = m % 60;
	h += m / 60;
	h = h % 24;
	setWallClockHook(UpdateWallClock);
}

//设置毫秒
void setms(int _ms)
{
	ms = _ms;
}

//获取时钟
void getWallClock(int* h, int* m, int* s)
{
	*h = hh;
	*m = mm;
	*s = ss;
}