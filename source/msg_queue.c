#include <ucos_ii.h>
#include <logger.h>
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
void Task1(void *pdata);
void Task2(void *pdata);
// Startup task
void TaskStart(void *pdata);
void TaskCon(void *pdata);
// Create task
static void TaskStartCreateTasks(void);

void main(void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(10000);
  OSInit();
  put_with_unix_time_ln("uC/OS-II loaded\n");
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
			case OS_NO_ERR:
        put_with_unix_time("task ");
        put_hex_u32(id);
        put_str(" has got the ");
        put_char((char *)mg);
        put_str_ln("");
        OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
        break;
			default :
        put_with_unix_time("queue1 ");
        put_hex_u32(id);
        put_str_ln(" is empty.");
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
      case OS_NO_ERR:
					printk("    task %d has got the %s. \n", id+3, (char *)mg);
					OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
					break;
				default :
					{
					printk( "queue2  is empty,%d can't got the message.       \n",id+3);
					OSTimeDlyHMSM(0, 0, 0, 200*(4-id));
					break;
					}                           /* If the queue is empty or has been deleted, print another string.*/
			}
		}
}