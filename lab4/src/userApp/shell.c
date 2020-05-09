#include "../myOS/userInterface.h"

#define NULL (void*)0

int getCmdline(unsigned char* buf, int limit)
{
	unsigned char* ptr = buf;
	int n = 0;
	while (n < limit)
	{
		*ptr = uart_get_char();
		if (*ptr == 0xd)
		{
			*ptr++ = '\n';
			*ptr = '\0';
			uart_put_char('\r');
			uart_put_char('\n');
			return n + 2;
		}
		uart_put_char(*ptr);
		ptr++;
		n++;
	}

	return n;
}

typedef struct command
{
	unsigned char cmd[20 + 1];
	int (*func)(int argc, unsigned char** argv);
	void (*help_func)(void);
	unsigned char description[100 + 1];
	struct command* nextCmd;
}Cmd;

Cmd* ourCmds = NULL;

int listCmds(int argc, unsigned char** argv)
{
	Cmd* tmpCmd = ourCmds;
	myPrintf(GREY, "list all registered commands:\n");
	myPrintf(GREY, "command name: description\n");

	while (tmpCmd != NULL)
	{
		myPrintf(GREY, "% 12s: %s\n", tmpCmd->cmd, tmpCmd->description);
		tmpCmd = tmpCmd->nextCmd;
	}
	return 0;
}

void addNewCmd(unsigned char* cmd,
	int (*func)(int argc, unsigned char** argv),
	void (*help_func)(void),
	unsigned char* description)
{
	Cmd* newcmd = (Cmd*)malloc(sizeof(Cmd));
	strcpy(cmd, newcmd->cmd);
	strcpy(description, newcmd->description);
	newcmd->func = func;
	newcmd->help_func = help_func;
	newcmd->nextCmd = NULL;
	//第一次创建
	if (ourCmds == NULL)
	{
		ourCmds = newcmd;
	}
	else
	{
		//遍历
		Cmd* cmdlist = ourCmds;
		while (cmdlist->nextCmd != NULL)
		{
			cmdlist = cmdlist->nextCmd;
		}
		cmdlist->nextCmd = newcmd;
	}
}

void help_help(void)
{
	myPrintf(GREY, "USAGE: help [cmd]\n\n");
}

int help(int argc, unsigned char** argv)
{
	int i;
	Cmd* tmpCmd = ourCmds;
	help_help();

	if (argc == 1) return listCmds(argc, argv);
	if (argc > 2) return 1;

	while (tmpCmd != NULL)
	{
		if (strcmp(argv[1], tmpCmd->cmd) == 0)
		{
			if (tmpCmd->help_func != NULL)
				tmpCmd->help_func();
			else myPrintf(GREY, "%s\n", tmpCmd->description);
			break;
		}
		tmpCmd = tmpCmd->nextCmd;
	}
	return 0;
}

Cmd* findCmd(unsigned char* cmd)
{
	Cmd* tmpCmd = ourCmds;
	int found = 0;
	while (tmpCmd != NULL)
	{  //at lease 2 cmds            
		if (strcmp(cmd, tmpCmd->cmd) == 0)
		{
			found = 1;
			break;
		}
		tmpCmd = tmpCmd->nextCmd;
	}
	return found ? tmpCmd : NULL;
}

int split2Words(unsigned char* cmdline, unsigned char** argv, int limit)
{
	unsigned char c, * ptr = cmdline;
	int argc = 0;
	int inAWord = 0;

	while (c = *ptr)
	{  // not '\0'
		if (argc >= limit)
		{
			myPrintf(GREY, "cmdline is tooooo long\n");
			break;
		}
		switch (c)
		{
		case ' ':  *ptr = '\0'; inAWord = 0; break; //skip white space
		case '\n': *ptr = '\0'; inAWord = 0; break; //end of cmdline?
		default:  //a word
			if (!inAWord)* (argv + argc++) = ptr;
			inAWord = 1;
			break;
		}
		ptr++;
	}
	return argc;
}

void initShell(void)
{
	addNewCmd("cmd\0", listCmds, NULL, "list all registered commands\0");
	addNewCmd("help\0", help, help_help, "help [cmd]\0");
}

unsigned char cmdline[100];
void startShell(void)
{
	unsigned char* argv[10];  //max 10
	int argc;
	Cmd* tmpCmd;
	//myPrintf(GREY,"StartShell:\n");     

	while (1)
	{
		myPrintf(BLUE, "Input a command >:");
		getCmdline(&cmdline[0], 100);
		myPrintf(GREY, cmdline);

		argc = split2Words(cmdline, &argv[0], 10);
		if (argc == 0) continue;

		tmpCmd = findCmd(argv[0]);
		if (tmpCmd)
			tmpCmd->func(argc, argv);
		else
			myPrintf(GREY, "UNKOWN command: %s\n", argv[0]);
	}
}
