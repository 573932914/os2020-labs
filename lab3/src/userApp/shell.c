#include "../myOS/lib/color.h"
#include "../myOS/dev/uart.h"
#include "../myOS/lib/string.h"
#include "../myOS/printk/myPrintk.h"
#define NULL 0

extern void append2screen(char* str, int color);

// 命令处理函数
int cmd_handler(int, char**);
int help_handler(int, char**);

// 帮助处理函数
void help_help(void);

struct command
{
	char* cmd;
	int (*func)(int argc, char** argv);
	void (*help_func)(void);
	const char* desc;
} cmds[] = {
	{"cmd", cmd_handler, NULL, "list all commands"},
	{"help", help_handler, help_help, "help [cmd]"},
	{"", NULL, NULL, ""}
};


// help 的帮助
void help_help(void)
{
	myPrintk(WHITE, "using \"help cmd_name\" to get help of the command\n");
}

// help 命令处理函数
int help_handler(int argc, char** argv)
{
	if (strcmp(*argv, "") == 0)
	{
		help_help();
	}
	//查找命令
	for (int i = 0;; i++)
	{
		//查找失败
		if (strcmp("", cmds[i].cmd) == 0)
		{
			myPrintk(WHITE, "help does not know the command:%s\n", *argv);
			break;
		}
		//查找成功
		if (strcmp(*argv, cmds[i].cmd) == 0)
		{
			//描述对应命令
			myPrintk(WHITE, "%s:%s\n", cmds[i].cmd, cmds[i].desc);
			//执行对应命令的help函数 
			if (cmds[i].help_func)
				(*cmds[i].help_func)();
			break;
		}
	}
	return 0;
}

// cmd 命令处理函数
int cmd_handler(int argc, char** argv)
{
	myPrintk(WHITE, "List all registered commands:\ncommand name:description\n");
	//遍历命令并输出信息
	int i = 0;
	while (strcmp(cmds[i].cmd, "") != 0)
	{
		myPrintk(WHITE, "%s:%s\n", cmds[i].cmd, cmds[i].desc);
		i++;
	}
	return 0;
}

int startShell()
{
	myPrintk(LIGHT_BLUE, "Waiting for a command >:");
	char line[100] = { 0 };
	char cmdline[20] = { 0 };
	char arrayline[80] = { 0 };
	int i = 0;
	char c;
	//命令输入
	while (1)
	{
		//获取命令
		c = uart_get_char();
		uart_put_char(c);
		//持续输入
		line[i] = c;
		i++;
		//终止输入
		if (ischarendline(c) == 1)
			break;
		 //超出输入长度限制
		if (i == 99)
		{
			myPrintk(WHITE, "Warning:command too long.\n");
			return 0;
		}
	}
	//打印一遍输入的命令
	myPrintk(WHITE,line);
	int k = 0;
	//分离参数
	for (; ischarempty(line[k]) == 0 && k < 20; k++)
	{
		cmdline[k] = line[k];
	}
	cmdline[k] = '\0';
	for (; ischarempty(line[k]) != 0 && k <= i; k++);
	if (k < i)
		for (int j = 0; ischarendline(line[k + j]) == 0 && j < 80; j++)
		{
			arrayline[j] = line[k + j];
		}
	//命令判断
	for (i = 0;; i++)
	{
		//查找失败
		if (strcmp("\0", cmds[i].cmd) == 0)
		{
			myPrintk(WHITE, "\nUNKNOWN command:%s\n", line);
			break;
		}
		//查找成功
		if (strcmp(cmdline, cmds[i].cmd) == 0)
		{
			//UART修正输出
			uart_put_char('\n');
			//执行命令 
			myPrintk(WHITE, "%s", line);
			char* arrayc = arrayline;
			(*cmds[i].func)(0, (char**)(&arrayc));
			break;
		}
	}
}