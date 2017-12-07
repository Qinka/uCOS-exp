#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>


// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Application tasks
#define  TASK_START_ID      0
// Application tasks priorities
#define  TASK_START_PRIO   17
// Number of identical tasks
#define  N_TASKS            3

// Tasks stacks 
OS_STK	      TaskStk[N_TASKS][TASK_STK_SIZE/sizeof(OS_STK)];	
// Startup    task stack
OS_STK        TaskStartStk[TASK_STK_SIZE];
// Parameters to pass to each task
INT8U         TaskData[N_TASKS];
// 1 mutual exclusive semaphore 
OS_EVENT      *mutex;

// Create tasks
static  void  TaskStartCreateTasks(void);
// Tasks
void  Task0(void *pdata);
void  Task1(void *pdata);
void  Task2(void *pdata);
// Startup task
void  TaskStart(void *pdata);

/**
 * Main function
 */
void main(void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(100000);
  OSInit();
  // Set a mutual semaphore 
  kprint("uC/OS-II loaded\n");
  OSTaskCreate(TaskStart,  (void *)0,  &TaskStartStk[TASK_STK_SIZE - 1], 4);
  OSStart();
}


/**
 * Task Start
 */
void TaskStart(void *pdata) {
  INT16S key;
  int i;
  mutex = OSSemCreate(1);
  // Create all the application tasks
  TaskStartCreateTasks();
  OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task Start Create tasks
 */
static void TaskStartCreateTasks(void) {
  INT8U  i;
  for (i = 0; i <N_TASKS; i++)
    TaskData[i] = i;
	OSTaskCreate(Task0, (void *)&TaskData[0], &TaskStk[0][TASK_STK_SIZE - 1], 5);
	OSTaskCreate(Task1, (void *)&TaskData[1], &TaskStk[1][TASK_STK_SIZE - 1], 6);
	OSTaskCreate(Task2, (void *)&TaskData[2], &TaskStk[2][TASK_STK_SIZE - 1], 7);
}

/**
 * Task 0
 */
void Task0(void *pdata) {
  INT8U  err;
  INT8U  id;
  INT16U value;
	id=*(int *)pdata;
  while(1) {
    kprint("Task %u is waitting a event.",id);
    OSTimeDly(200);
    kprint("The event of Task %u come.",id);
    kprint("Task %u is try to get mutex.",id);
    OSSemPend(mutex,0,&err);
		switch(err) {
			case OS_ERR_NONE:
        kprint("Task %u has got the mutex.\n",id);
        break;
			default:
				kprint("Task %u is suspended.\n",id);
        break;
		}
		OSTimeDly(200);
		kprint("Task %u release mutex.",id);
    OSSemPost(mutex);
	 }
}

void Task1(void *pdata) {
  INT8U  err;
  INT8U  id;
  int i;
	id=*(int *)pdata;
  while(1) {
    kprint("Task %u is waitting a event.",id);
    OSTimeDly(100);
    kprint("The event of Task %u come.",id);
    OSTimeDly(100);
	 }
}

void Task2(void *pdata) {
  INT8U  err;
  INT8U  id;
  INT16U value;
	id=*(int *)pdata;
	int i;
  while(1) {
		kprint("Task %u is trying to get mutex.\n",id);
    // Acquire mutex
    OSSemPend(mutex,0,&err);
		switch(err)	{
      case OS_ERR_NONE:
        kprint("Task %u has got the mutex.\n",id);
        OSTimeDly(200);
        break;
      default :
				kprint("Task %u  is failed to get mutex.\n",id);
				OSTimeDly(200);
        break;
		}
    kprint("Task %u release mutex.\n",id);
    // Release
    OSSemPost(mutex);
	}
}
