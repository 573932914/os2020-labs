#include "../i386/io.h"

//串口地址
#define uart_base 0x3F8

void uart_put_char(unsigned char c)
{
	outb(uart_base, c);	
	if (c == '\n')
		outb(uart_base, '\r');
}

unsigned char uart_get_char(void)
{
	while (!(inb(uart_base + 5) & 1));
	char c = inb(uart_base);
	return c;
}

void uart_put_chars(char* str)
{
	//判断字符串尾
	while (*str != '\0')
	{
		uart_put_char(*str);
		str++;
	}
}