#include <ucos_ii.h>
#include <aikit/kprint.h>
#include <hardware.h>

// Size of each task's stacks (# of WORDs)
#define  TASK_STK_SIZE    512
// Number of identical tasks
#define  N_TASKS            2

// Tasks stacks
OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
// TaskStart  task stack   
OS_STK TaskStartStk[TASK_STK_SIZE];
// Parameters to pass to each task
INT8U  TaskData[N_TASKS];

// Tasks
void Task0(void *pdata);
// Startup task
void Task1(void *pdata);
// Creat task 
void TaskStart(void *pdata);
// Create task
static void TaskStartCreateTasks(void);

// Initialize count
INT32U count = 0;

/**
 * main function
 */
void  main (void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(10000);
  OSInit();
  kprint("%u: Ucos-II loaded\n");
  OSTaskCreate(TaskStart, (void *)0, (OS_STK*)&TaskStartStk[TASK_STK_SIZE - 1], 4);
  OSStart();
}

/**
 * Start task
 */
void  TaskStart (void *pdata) {
  // Allocate storage for CPU status register
  OS_CPU_SR  cpu_sr;
  // Create all the application tasks
  TaskStartCreateTasks();
  // Suspend the TaskStart
  OSTaskSuspend(OS_PRIO_SELF);
}

/**
 * Task start create tasks
 */
void  TaskStartCreateTasks (void) {
  INT8U  i;

  for (i = 0; i < N_TASKS; i++) // Create tasks
    // Each task will display its own information
    TaskData[i] =  i;
  OSTaskCreate(Task0, (void *)&TaskData[0], &TaskStk[0][TASK_STK_SIZE - 1], 5);
  OSTaskCreate(Task1, (void *)&TaskData[1], &TaskStk[1][TASK_STK_SIZE - 1], 6);
}

/**
 * Task #0
 * Description: This task is suspended by itself,then it will be resumed by other tasks.
 */
void  Task0 (void *pdata) {
  INT8U i;
  INT8U err;
 	i=*(int *)pdata;
	while(1) {
    // light LED
    LEDR_ON;
    kprint("");
    kprint("The application tasks switch counts: 0x%u",++count);
    kprint("Task0 is   running.");
    kprint("Task1 is   suspended.");
    kprint("");
    // light LEDc
    LEDR_OFF;
    // suspend itself 
    err=OSTaskSuspend(5);
	}
}

/**
 * TASK #1
 * Description: This task resume task0
 */
void  Task1 (void *pdata) {
  INT8U i;
  INT8U err;
  i=*(int *)pdata;
  while(1) {
    // change led
    LED1_ON;
    LED2_OFF;
    OSTimeDly(150);
    kprint("");
    kprint("The application tasks switch counts: 0x%u",++count);
    kprint("Task0 is   suspended.");
    kprint("Task1 is   running.");
    kprint("");
    OSTimeDly(150);
    // change led
    LED1_OFF;
    LED2_ON;
    // resume task0 
    err=OSTaskResume(5);
  }
}
