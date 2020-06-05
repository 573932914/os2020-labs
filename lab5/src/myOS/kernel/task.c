#include "../include/task.h"
#include "../include/myPrintk.h"

void schedule(void);
void destroyTsk(int takIndex);

/**
 * 内部接口参考
 */
typedef struct rdyQueueFCFS
{
	myTCB* head;
	myTCB* tail;
	myTCB* idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;

void rqFCFSInit(myTCB* idleTsk)
{
	rqFCFS.head = (myTCB*)0;
	rqFCFS.tail = (myTCB*)0;
	rqFCFS.idleTsk = idleTsk;
}

int rqFCFSIsEmpty(void)
{
	if ((rqFCFS.head == (myTCB*)0) && (rqFCFS.tail == (myTCB*)0))
		return 1;
	else
		return 0;
}

myTCB* nextFCFSTsk(void)
{
	if (rqFCFSIsEmpty() == 1)
		return rqFCFS.idleTsk;
	else
		return rqFCFS.head;
}

/* tskEnqueueFCFS: insert into the tail node */
void tskEnqueueFCFS(myTCB* tsk)
{
	if (rqFCFSIsEmpty() == 1)
		rqFCFS.head = tsk;
	else
		rqFCFS.tail->next = tsk;
	rqFCFS.tail = tsk;
}

/* tskDequeueFCFS: delete the first node */
void tskDequeueFCFS(myTCB* tsk)
{
	rqFCFS.head = rqFCFS.head->next;
	if (tsk == rqFCFS.tail)
		rqFCFS.tail = (myTCB*)0;
}

// 用于初始化新创建的 task 的栈
// 这样切换到该任务时不会 stack underflow
void stack_init(unsigned long** stk, void (*task)(void))
{
	*(*stk)-- = (unsigned long)0x08;       //CS高地址
	*(*stk)-- = (unsigned long)task;       //eip
	*(*stk)-- = (unsigned long)0x0202;     //init eflags: IF=1,BIT1=1
	*(*stk)-- = (unsigned long)0xAAAAAAAA; //EAX	
	*(*stk)-- = (unsigned long)0xCCCCCCCC; //ECX
	*(*stk)-- = (unsigned long)0xDDDDDDDD; //EDX
	*(*stk)-- = (unsigned long)0xBBBBBBBB; //EBX
	*(*stk)-- = (unsigned long)0x44444444; //ESP
	*(*stk)-- = (unsigned long)0x55555555; //EBP
	*(*stk)-- = (unsigned long)0x66666666; //ESI
	*(*stk) = (unsigned long)0x77777777;   //EDI低地址
}

void tskStart(myTCB* tsk)
{
	tsk->state = READY;
	tskEnqueueFCFS(tsk);
}

void tskEnd(void)
{
	myPrintk(GREEN, "this task is end\n");
	tskDequeueFCFS(currentTsk);
	destroyTsk(currentTsk->pid);
	schedule();
}

/* createTsk
 * tskBody():
 * return value: taskIndex or, if failed, -1
 */
int createTsk(void (*tskBody)(void))
{
	myPrintk(DARK_GREEN, "Creat a new task...\n");
	if (firstFreeTsk != (myTCB*)0)
	{
		myTCB* newTCB = firstFreeTsk;
		firstFreeTsk = newTCB->next;
		newTCB->state = WAIT;
		newTCB->task = tskBody;	
		newTCB->stkTop = newTCB->stkBase + STACK_SIZE - 1;
		newTCB->next = (myTCB*)0;
		stack_init(&(newTCB->stkTop), tskBody);
		myPrintk(GREEN, "Succecss,now,Start\n");
		tskStart(newTCB);
		return newTCB->pid;
	}
	else
		return -1;
}

/* destroyTsk
 * takIndex:
 * return value: void
 */
void destroyTsk(int takIndex)
{
	//查找
	for (int i = 0; i < TASK_NUM; i++)
		if (takIndex == tcbPool[i].pid)
		{
			tcbPool[i].state = DEATH;
			break;
		}
	//空闲Tsk查找
	for(int i=0;i<TASK_NUM;i++)
		if (tcbPool[i].state == DEATH || tcbPool[i].state == NEW)
		{
			firstFreeTsk = tcbPool + i; 
			return;
		}
}

unsigned long** prevTSK_StackPtr;
unsigned long* nextTSK_StackPtr;
//对CTX_SW的封装
void context_switch(myTCB* prevTsk, myTCB* nextTsk)
{
	prevTSK_StackPtr = &(prevTsk->stkTop);
	nextTSK_StackPtr = nextTsk->stkTop;
	currentTsk = nextFCFSTsk();
	myPrintk(GREEN, "CTX_SW...\n");
	CTX_SW(prevTSK_StackPtr, nextTSK_StackPtr);
}

void scheduleFCFS(void)
{
	context_switch(currentTsk, nextFCFSTsk());
}

void schedule(void)
{
	scheduleFCFS();
}

/**
 * idle 任务
 */
void tskIdleBdy(void)
{
	while (1)
	{
		schedule();
	}
}

unsigned long BspContextBase[STACK_SIZE];
unsigned long* BspContext;

//start multitasking
void startMultitask(void)
{
	BspContext = BspContextBase + STACK_SIZE - 1;
	prevTSK_StackPtr = &BspContext;
	currentTsk = nextFCFSTsk();
	nextTSK_StackPtr = currentTsk->stkTop;
	myPrintk(GREEN, "CTX_SW runing start...\n");
	CTX_SW(prevTSK_StackPtr, nextTSK_StackPtr);
	myPrintk(GREEN, "SuccessInit!\n");
}

void TaskManagerInit(void)
{
	//初始化 TCB 数组
	for (int i = 0; i < TASK_NUM; i++)
	{
		tcbPool[i].pid = i;
		if (i < TASK_NUM - 1)
			tcbPool[i].next = tcbPool + i + 1;
		else
			tcbPool[i].next = (myTCB*)0;
		tcbPool[i].stkTop = tcbPool[i].stkBase + STACK_SIZE - 1;
	}
	//创建 idle 任务
	idleTsk = tcbPool;
	stack_init(&(idleTsk->stkTop), tskIdleBdy);
	rqFCFSInit(idleTsk);
	firstFreeTsk = tcbPool + 1;
	//创建 init 任务（使用 initTskBody）
	createTsk(initTskBody);
	//切入多任务状态
	myPrintk(DARK_GREEN, "START MULTITASKING......\n");
	startMultitask();
	myPrintk(DARK_GREEN, "STOP MULTITASKING......ShutDown\n");
}
