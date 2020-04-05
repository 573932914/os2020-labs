//取字符串长
int strlen(char* str)
{
	int n = 0;
	while (*str++) n++;
	return n;
}

//字符串拷贝
int strcpy(char* src, char* dst)
{
	int n = 0;
	while (*src)
	{
		*dst++ = *src++;
		n++;
	}
	return n;
}

//字符串比较
int strcmp(char* str1, char* str2)
{
	char c1, c2;
	do
	{
		c1 = *str1++;
		c2 = *str2++;
		if (c1 != c2) return (c1 > c2) ? 1 : -1;
		if (c1 == '\0') break;
	} while (1);
	return 0;
}

//判断字符是否为换输信号
int ischarempty(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\0') ? 1 : 0;
}

//判断字符是否为确认输入信号
int ischarendline(char c)
{
	return (c == '\n' || c == '\r' || c == '\0') ? 1 : 0;
}