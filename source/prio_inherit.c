#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>


// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Number of identical tasks
#define  N_TASKS            2

// Tasks stacks 
OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];
// Startup    task stack
OS_STK        TaskStartStk[TASK_STK_SIZE];
// Parameters to pass to each task
INT8U         TaskData[N_TASKS];
// 1 mutual exclusive semaphore 
OS_EVENT      *mutex;

// Task
void Task(void *pdata);
// Task startup
void TaskStart(void *pdata);
// Create tasks
static void TaskStartCreateTasks(void);


/**
 * Main function
 */
void main(void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(100000);
  OSInit();
  mutex = OSMutexCreate(8,&err);
  kprint("uC/OS-II loaded\n");
  OSTaskCreate(TaskStart,  (void *)0,  &TaskStartStk[TASK_STK_SIZE - 1], 9);
  OSStart();
}

/**
 * Task Start
 */
void TaskStart(void *pdata) {
  INT16S key;
  int i;
  // Create all the application tasks
  TaskStartCreateTasks();
  OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task Start create tasks
 */
void TaskStartCreateTasks(void) {
    INT8U  i;
    for (i = 0; i <N_TASKS; i++) {
		  TaskData[i] = i;
      OSTaskCreate(Task, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], 12-i);
	}
}

void  Task (void *pdata) {
  INT8U  err;
  INT8U  id;
	id=*(int *)pdata;
  while(1) {
		kprint("task %u try to get the mutex.", id);
    // Acquire mutex to get continue
		OSMutexPend(mutex, 0, &err);
		kprint("task %u is getting the mutex.", id);
    // Wait 200 minisecond 
		OSTimeDlyHMSM(0, 0, 0, 200);
    kprint("task %u   releases the mutex.", id);
    // elease mutex
		OSMutexPost(mutex); 
    // Wait (3-id)*150 minisecond
		OSTimeDlyHMSM(0, 0, 0, (3-id)*150);
	}
}
