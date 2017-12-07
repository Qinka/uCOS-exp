#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>


// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Number of identical tasks
#define  N_TASKS            2

// TaskStart  task stack   
OS_STK TaskStartStk[TASK_STK_SIZE];
// Tasks stacks
OS_STK TaskStk1[N_TASKS][TASK_STK_SIZE];
OS_STK TaskStk2[N_TASKS][TASK_STK_SIZE];
// Control task
OS_STK TaskConStk[TASK_STK_SIZE];
// Parameters to pass to each task
INT8U  TaskData1[N_TASKS];
INT8U  TaskData2[N_TASKS];

// msg queue
OS_EVENT* q1;
OS_EVENT* q2;
// array for msg
void* Msg1[6];
void* Msg2[6];

// Tasks
void Taskq1(void *pdata);
void Taskq2(void *pdata);
// Startup task
void TaskStart(void *pdata);
void TaskCon(void *pdata);
// Create task
static void TaskStartCreateTasks(void);

void main(void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(100000);
  OSInit();
  kprint("uC/OS-II loaded\n");
  q1 = OSQCreate(&Msg1[0],6);
  q2 = OSQCreate(&Msg2[0],6);
  OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1],4);
  OSStart();
}

/**
 * Task Start
 */
void TaskStart(void *pdata) {
  OS_CPU_SR cpu_sr;
  TaskStartCreateTasks();
 	OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task Start Create Tasks
 */
void TaskStartCreateTasks(void) {
  INT8U  i;
  for (i = 0; i < N_TASKS; i++) { //Create N_TASKS identical tasks
    // Each task will pass its own id
    TaskData1[i] = i;
		OSTaskCreate(Taskq1, (void *)&TaskData1[i], &TaskStk1[i][TASK_STK_SIZE - 1], i+1);
  }
  for (i = 0; i <N_TASKS; i++) { //Create N_TASKS identical tasks
    // Each task will pass its own id
	  TaskData2[i] = i;
    OSTaskCreate(Taskq2, (void *)&TaskData2[i], &TaskStk2[i][TASK_STK_SIZE - 1], i+4);
  }
  // Create control tasks
	OSTaskCreate(TaskCon, (void *)0, &TaskConStk[TASK_STK_SIZE - 1],i+4);
}

/**
 * Task: queue 1
 */
void Taskq1(void *pdata) {
  INT8U  err;
  INT8U  id;
	char   s[30];
	void   *mg;
	id=*(int *)pdata;
  while(1) {
    // Wait 2 second
		OSTimeDlyHMSM(0, 0, 2, 0);
    // apply for message
		mg=OSQPend(q1,0,&err);
		switch(err) {
			case OS_ERR_NONE:
        kprint("Task %u has got the %s", id, (char *)mg);
        OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
        break;
			default :
        kprint("Queue1 %u is empty.",id);
        OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
        break;
		}
 	}
}

/**
 * Task: queue 2
 */
void Taskq2(void *pdata) {
	INT8U  err;
	INT8U  id;
	char   s[30];
	void   *mg;
	id=*(int *)pdata;
  while(1) {
    OSTimeDlyHMSM(0, 0, 2, 0);
		mg=OSQPend(q2,0,&err);
		switch(err) {
      case OS_ERR_NONE:
        kprint("Task %u has got the %s. \n", id+3, (char *)mg);
        OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
        break;
      default:
        kprint("queue2  is empty,%u can't got the message.", id+3);
        OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
        break;
			}
		}
}

/**
 * Control task
 */

void  TaskCon (void *pdata) {
	INT8U  i,j;
	INT8U  err;
	INT8U  note=1;
	INT16U  del=3;
	OS_EVENT *q;
	char   ch[50];
	OS_Q_DATA data;
	static char *s[]={
		"message0","message1","message2","message3","message4","message5"
	};
	static char *t[]={
		"messageA","messageB","messageC","messageD","messageE","messageF"
	};
  while(1) {
 		kprint("Add message to queue1");
		for( i = 0 ; i < 6 ; i++ ) {
      // post message to q1 LIFO
			err = OSQPostFront(q1,(void*)s[i]);
			switch(err)	{
				case OS_ERR_NONE:
					kprint("the queue1 %u add %s",i,s[i]);
          OSTimeDlyHMSM(0, 0, 0, 150);
					break;
				case OS_ERR_Q_FULL:
					kprint("the queue1 is full, don't add.");
					OSTimeDlyHMSM(0, 0, 0, 150);
					break;
				default:
          break;
      }
    }
    if(del>=0)
      kprint("Add message to queue2");
		for( j = 0 ; j < 6 ; j++ ) {
      // post message to q2 FIFO
			err = OSQPost(q2,(void*)t[j]);
			switch(err)	{
				case OS_ERR_NONE:
					kprint("the queue2 %u add %s",j,t[j]);
          OSTimeDlyHMSM(0, 0, 0, 150);
					break;
				case OS_ERR_Q_FULL:
					kprint("the queue2 is full, don't add.");
					OSTimeDlyHMSM(0, 0, 0, 150);
					break;
				default:
          break;
      }
    }
		if(del>=0)
			if(note==1) {
				OSQFlush(q2);
				kprint("Clear up the queue2");
				note=0;
      }
			else
				note=1;
    // get the information about q2
		err=OSQQuery(q2,&data);
		if(err==OS_ERR_NONE) {
			kprint("the queue2'information");
			kprint("NextMsg:%s,  NumMsg:%u,  QSize:%u.",(char *)data.OSMsg,data.OSNMsgs,data.OSQSize);
			}
		OSTimeDlyHMSM(0, 0, 0, 500);
 		if(del==0){
      // delete the  q2 
			q=OSQDel(q2,OS_DEL_ALWAYS,&err);
			if(q==(OS_EVENT *)0)
	    	kprint("Already successful delete queue2");
    }
		else {
			del--;
			kprint("Not successful delete queue2");
		}
  }
}