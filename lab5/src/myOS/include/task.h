#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "../../userApp/userApp.h"
#endif

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define STACK_SIZE 0x8000            // size of a task stack

#define initTskBody myMain         // connect initTask with myMain

#define NEW 0                   //新的
#define READY 1                 //就绪
#define WAIT 2                  //等待
#define RUN 3                   //运行
#define DEATH 4                 //销毁

void initTskBody(void);

void CTX_SW(void*prev_stkTop, void*next_stkTop);

typedef struct myTCB {
     unsigned long* stkTop;                   //栈顶指针
	 unsigned long stkBase[STACK_SIZE];       //栈底指针
	 unsigned short int state;                //状态
	 unsigned short int pid;                  //任务ID
	 struct myTCB* next;                      //下一个结点
	 void (*task)(void);                   //任务
} myTCB;

myTCB tcbPool[TASK_NUM];

myTCB * idleTsk;                /* idle 任务 */
myTCB * currentTsk;             /* 当前任务 */
myTCB * firstFreeTsk;           /* 下一个空 TCB */

void TaskManagerInit(void);

#endif
