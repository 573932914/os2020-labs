#include "../../include/task_sched.h"

myTCB* rqSJF;  //ready queue

void rqSJFInit(void)
{
	rqSJF = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList*)(&(rqSJF->thisNode)));
}

myTCB* nextSJFTsk(void)
{
	dLinkedList* head = (dLinkedList*)rqSJF;
	dLinkedList* plist = head;
	myTCB* ptcb = (myTCB*)head;
	do
	{
		//节点移动
		plist = dLinkListFirstNode(plist);
		//检测到更短执行时间
		if (((myTCB*)plist)->para.exeTime < ptcb->para.exeTime)
		{
			//更新最短执行时间
			ptcb = (myTCB*)plist;
		}
	} while (plist != head);
	return ptcb;
}

void tskEnqueueSJF(myTCB* tsk)
{
	dLinkInsertFifo((dLinkedList*)rqSJF, (dLink_node*)tsk);
}

void tskDequeueSJF(myTCB* tsk)
{
	dLinkDelete((dLinkedList*)rqSJF, (dLink_node*)tsk);
}

void schedulerInit_SJF(void)
{
	rqSJFInit();
	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_SJF;
	/* special for idleTsk*/
	_setTskPara(idleTsk, &defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;
}

struct scheduler scheduler_SJF = {
	.type = SCHEDULER_SJF,
	.nextTsk_func = nextSJFTsk,
	.enqueueTsk_func = tskEnqueueSJF,
	.dequeueTsk_func = tskDequeueSJF,
	.schedulerInit_func = schedulerInit_SJF,
	.createTsk_hook = NULL,
	.tick_hook = NULL
};

