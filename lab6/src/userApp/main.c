#include "../myOS/userInterface.h"   //interface from kernel
#include "shell.h"
#include "memTestCase.h"
#define SCHED_INSTANCE SCHEDULER_PRIORITY           //改变宏定义实现算法选择

void wallClock_hook_main(void)
{
	int _h, _m, _s;
	char hhmmss[] = "hh:mm:ss\0\0\0\0";
	getWallClock(&_h, &_m, &_s);
	sprintf(hhmmss, "%02d:%02d:%02d", _h, _m, _s);
	put_chars(hhmmss, 0x7E, 24, 72);
}

// should be named as scheduler_hook_main
// Be executed very early
void scheduler_hook_main(void)
{
	//prior settings
	setSysScheduler(SCHED_INSTANCE);
}

void doSomeTestBefore(void)
{
	setWallClock(18, 59, 59);		//set time 18:59:59
	setWallClockHook(&wallClock_hook_main);
}

void myTSK0(void)
{
#if(SCHED_INSTANCE == SCHEDULER_FCFS)    //FCFS
	int j = 1;
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK0::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_SJF)    //SJF
	int j = 1;
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK0::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_PRIORITY)    //PRIO
	int j = 1;
	myPrintf(0x7, "priority of myTSK0 : 3\n");
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK0::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#endif 
	tskEnd();   //the task is end
}

void myTSK1(void)
{
#if(SCHED_INSTANCE == SCHEDULER_FCFS)    //FCFS
	int j = 1;
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK1::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_SJF)    //SJF
	int j = 1;
	while (j <= 8)
	{
		myPrintf(0x7, "myTSK1::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_PRIORITY)    //PRIO
	int j = 1;
	myPrintf(0x7, "priority of myTSK0 : 5\n");
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK1::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#endif 
	tskEnd();   //the task is end
}

void myTSK2(void)
{
#if(SCHED_INSTANCE == SCHEDULER_FCFS)    //FCFS
	int j = 1;
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK2::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_SJF)    //SJF
	int j = 1;
	while (j <= 6)
	{
		myPrintf(0x7, "myTSK2::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#elif(SCHED_INSTANCE == SCHEDULER_PRIORITY)    //PRIO
	int j = 1;
	myPrintf(0x7, "priority of myTSK0 : 4\n");
	while (j <= 4)
	{
		myPrintf(0x7, "myTSK2::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
#endif 
	tskEnd();   //the task is end
}

void testScheduler(void)
{
	//FCFS or RR or SJF or PRIORITY0
	tskPara tskParas[4];
	int i;
	for (i = 0; i < 4; i++)
		initTskPara(&tskParas[i]);
	//创建task 并且根据调度算法 设置task的参数
#if(SCHED_INSTANCE == SCHEDULER_FCFS)    //FCFS
	setTskPara(ARRTIME, 50, &tskParas[0]);
	createTsk(myTSK0, &tskParas[0]);
	setTskPara(ARRTIME, 100, &tskParas[1]);
	createTsk(myTSK1, &tskParas[1]);
	setTskPara(ARRTIME, 0, &tskParas[2]);
	createTsk(myTSK2, &tskParas[2]);
#elif(SCHED_INSTANCE == SCHEDULER_SJF)    //SJF
	setTskPara(EXETIME, 100, &tskParas[0]);
	setTskPara(ARRTIME, 0, &tskParas[0]);
	createTsk(myTSK0, &tskParas[0]);
	setTskPara(EXETIME, 200, &tskParas[1]);
	setTskPara(ARRTIME, 0, &tskParas[1]);
	createTsk(myTSK1, &tskParas[1]);
	setTskPara(EXETIME, 150, &tskParas[2]);
	setTskPara(ARRTIME, 0, &tskParas[2]);
	createTsk(myTSK2, &tskParas[2]);
#elif(SCHED_INSTANCE == SCHEDULER_PRIORITY)    //PRIO
	setTskPara(PRIORITY, 3, &tskParas[0]);
	setTskPara(ARRTIME, 0, &tskParas[0]);
	createTsk(myTSK0, &tskParas[0]);
	setTskPara(PRIORITY, 5, &tskParas[1]);
	setTskPara(ARRTIME, 0, &tskParas[1]);
	createTsk(myTSK1, &tskParas[1]);
	setTskPara(PRIORITY, 4, &tskParas[2]);
	setTskPara(ARRTIME, 0, &tskParas[2]);
	createTsk(myTSK2, &tskParas[2]);
#elif    //ELSE
	return;
#endif 
	initShell();
	memTestCaseInit();
	setTskPara(ARRTIME, 120, &tskParas[3]);
	setTskPara(EXETIME, 1000, &tskParas[3]);
	setTskPara(PRIORITY, 2, &tskParas[3]);
	createTsk(startShell, &tskParas[3]);  //    startShell();
}

void myMain(void)
{    
	//main is our init task
	clear_screen();
	doSomeTestBefore();
	myPrintf(0x7, "********INIT START\n\n");
	testScheduler();
	myPrintf(0x7, "********INIT END\n\n");
	tskEnd();  // init end	
}