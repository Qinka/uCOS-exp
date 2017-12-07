#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>

// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Application tasks
#define  TASK_START_ID      0
// Application tasks priorities
#define  TASK_START_PRIO    0
// Number of tasks
#define	 N_TASKS			      5

typedef struct {
    char    TaskName[30];
    INT16U  TaskCtr;
    INT16U  TaskExecTime;
    INT32U  TaskTotExecTime;
} TASK_USER_DATA;

// Tasks stacks
OS_STK	TaskStk[N_TASKS+1][TASK_STK_SIZE];
// Startup task stack
OS_STK          TaskStartStk[TASK_STK_SIZE];
TASK_USER_DATA  TaskUserData[7];
// 5 tasks semaphore 
OS_EVENT	 *fork[N_TASKS];

void  Task(void *pdata);
void  TaskStart(void *data); 
static  void  TaskStartCreateTasks(void);
static  void  TASK_Thinking_To_Hungry(INT8U);
static  void  TASK_Eat(INT8U);

INT8U  TaskData[N_TASKS]; 


/**
 * main function
 */
void  main (void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(10000);
  OSInit();
  kprint("Ucos-II loaded\n");
  OSTaskCreateExt(TaskStart,
                  (void *)0,
                  &TaskStartStk[TASK_STK_SIZE - 1],
                  TASK_START_PRIO,
                  TASK_START_ID,
                  &TaskStartStk[0],
                  TASK_STK_SIZE,
                  &TaskUserData[TASK_START_ID],
                  0);
  OSStart();
}

/**
 * Start task
 */
void  TaskStart (void *pdata) {
  INT16S key;
  int i;
  for (i=0 ;i<N_TASKS;i++ )
    fork[i] = OSSemCreate(1);
  TaskStartCreateTasks();
  OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task start create tasks
 */
void  TaskStartCreateTasks (void) {
  INT8U  i;
  for (i = 0; i <N_TASKS; i++) {
    TaskData[i] = i;
    OSTaskCreate(Task, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i+1);
  }
}

// Hunger
static void TASK_Thinking_To_Hungry(INT8U data) {
  kprint("Philosopher %b is thinking.\n", data+1);
  OSTimeDly(100);
	kprint("Philosopher %b is hungry.  \n ", data+1);
  OSTimeDly(100);
}

// Eat
static void TASK_Eat(INT8U data){
    kprint("Philosopher %b is eating.   \n ", data+1);

	OSTimeDly(100);
}

// Task
void  Task (void *pdata) {
  INT8U  err;
  INT8U  i;
  INT8U  j;

  i=*(int *)pdata;
  j=(i+1) % 5;

  while(1) {
    TASK_Thinking_To_Hungry(i);
		OSSemPend(fork[i], 0, &err);
		OSSemPend(fork[j], 0, &err);
		TASK_Eat(i);
		OSSemPost(fork[j]);
		OSSemPost(fork[i]);
		OSTimeDly(200);
  }
}