#include <stdarg.h>
extern int vsprintf(char* buf, const char* fmt, va_list args);
extern void append2screen(char* str, int color);
extern void uart_put_chars(char* str);

char uBuf[400];  //TODO: fix me
int myPrintf(int color, const char* format, ...)
{
	va_list args;
	int i;
	va_start(args, format);
	//将获取到的fmt格式字符串写入到buf这个缓存里去   
	i = vsprintf(uBuf, format, args);
	//释放args   
	va_end(args);
	append2screen(uBuf, color);
	uart_put_chars(uBuf);
	return i;
}

char kBuf[400];  //TODO: fix me
int myPrintk(int color, const char* format, ...)
{
	va_list args;
	int i;
	va_start(args, format);
	//将获取到的fmt格式字符串写入到buf这个缓存里去   
	i = vsprintf(kBuf, format, args);
	//释放args   
	va_end(args);
	append2screen(kBuf, color);
	uart_put_chars(kBuf);
	return i;
}