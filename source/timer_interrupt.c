#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>


// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Application tasks IDs 
#define  TASK_START_ID       0
// Application tasks priorities
#define  TASK_START_PRIO    10
#define  TASK_1_PRIO        12
#define  TASK_2_PRIO        13
#define  TASK_3_PRIO        14
// Number of identical tasks
#define  N_TASKS            5

// TaskStart  task stack   
OS_STK TaskStartStk[TASK_STK_SIZE];
// Tasks stacks
OS_STK Task1Stk[TASK_STK_SIZE];
OS_STK Task2Stk[TASK_STK_SIZE];
OS_STK Task3Stk[TASK_STK_SIZE];
// Tasks stacks
OS_STK TaskStk[N_TASKS][TASK_STK_SIZE/sizeof(OS_STK)];
// Parameters to pass to each task
INT8U  TaskData1[N_TASKS];
INT8U  TaskData2[N_TASKS];

OS_EVENT *InterruptSem;

// tasks
void TaskStart(void *data);
static void TaskStartCreateTasks(void);
void Task1(void *data);
void Task2(void *data);
void Task3(void *data);

typedef struct {
    char    TaskName[30];
    INT16U  TaskCtr;
    INT16U  TaskExecTime;
    INT32U  TaskTotExecTime;
} TASK_USER_DATA;

TASK_USER_DATA  TaskUserData[7];

/**
 * Main function
 */
void main(void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(100000);
  OSInit();
  kprint("uC/OS-II loaded\n");
  OSTaskCreateExt(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], TASK_START_PRIO, TASK_START_ID, &TaskStartStk[0], TASK_STK_SIZE, &TaskUserData[TASK_START_ID], 0);
  OSStart();
}

/**
 * Task Start
 */
void  TaskStart(void *pdata) {
  INT16S key;
  int i;
  // create 1 signal
  InterruptSem=OSSemCreate(1);
  // Create all the application tasks
  TaskStartCreateTasks();
  OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task Start Create Tasks
 */
void  TaskStartCreateTasks(void) {
	OSTaskCreate(Task1,(void *)0,&Task1Stk[TASK_STK_SIZE - 1],TASK_1_PRIO);
	OSTaskCreate(Task2,(void *)0,&Task2Stk[TASK_STK_SIZE - 1],TASK_2_PRIO);
	OSTaskCreate(Task3,(void *)0,&Task3Stk[TASK_STK_SIZE - 1],TASK_3_PRIO);
}

/**
 * Task 1
 */
void  Task1 (void *pdata) {
  INT8U  err;
  while(1) {
    OSTimeDly(100);
    kprint( "Task1 is try to get semaphore.");
    // Acquire semaphore to get into the room
    OSSemPend(InterruptSem, 0, &err);
    kprint("Task1 has Succeed to obtain semaphore.");
    kprint("Task1 is delayed.");
    OSTimeDly(200);
    kprint("The delay of Task1 finished .");
    kprint("Task1 release semaphore.");
    // Release semaphore 
    OSSemPost(InterruptSem);
    OSTimeDly(200);
  }
}

void Task2(void *pdata) {
  INT8U err;
  while(1) {
    kprint( "Task2 is try to get semaphore.");
    // Acquire semaphore to get into the room
    OSSemPend(InterruptSem, 0, &err);
    kprint( "Task2 has Succeed to obtain semaphore.");
    kprint("Task2 is delayed.");
    OSTimeDly(500);
    kprint("The delay of Task2 finished .");
    kprint("Task2 release semaphore.");
    // Release semaphore
    OSSemPost(InterruptSem);
    OSTimeDly(200);
  }
}

void  Task3 (void *pdata) {
  while(1) {
		  kprint("Task3 has got the CPU");
		  OSTimeDly(100);
   }
}