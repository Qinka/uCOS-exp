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

// Store OSMemGet return value
INT8U  *CommMsg1;			
INT8U  *CommMsg2;
INT8U  *CommMsg3;
// Store OSMemCreate return value
INT8U  *CommMem;
// init the data
INT8U   CommBuf[2][128];

// Tasks stacks
OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];
OS_STK        TaskStartStk[TASK_STK_SIZE];
// Parameters to pass to each task 
INT8U         TaskData[N_TASKS];
// Mutual exclusive semaphore
OS_EVENT      *mutex;

// Tasks
void Task(void *pdata);
// Startup task
void TaskStart(void *pdata);
// Create task
static void TaskStartCreateTasks(void);
// memory create
static void MemoryCreate(void);

/**
 * main
 */
void  main (void) {
	INT8U err;
  hardware_init();
  OS_CPU_SysTickInit(10000);
  OSInit();
  kprint("uC/OS-II loaded\n");
  OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1],4);
  OSStart();
}

/**
 * Memory Info
 */
void MemInfo(void *pdata) {
  INT8U  err;
  OS_MEM_DATA mem_data;
  err = OSMemQuery(CommMem, &mem_data);
}

/**
 * Display funcrtion
 */
static void DisplayMemStat() {
  INT8U        err;
  INT8U		 s[4][50];
	OS_MEM_DATA mem_data;
	OSMemQuery(CommMem, &mem_data);

	kprint("");
	kprint("The pointer to the begining of memory address is: %u",(uint32_t)(mem_data.OSAddr));
	kprint("the size of blocks in this memory area is: %u", (uint32_t)(mem_data.OSBlkSize));
	kprint("the number of free blocks in this memory area is: %u", (uint32_t)(mem_data.OSNFree));
	kprint("the number of using blocks in this memory area is: %u", (uint32_t)(mem_data.OSNUsed));
	kprint("");
	OSTimeDlyHMSM(0,0,2,0);
}

/**
 * Task Start
 */
void  TaskStart (void *pdata) {
  OS_CPU_SR cpu_sr;
  INT16S    key;
  MemoryCreate();
  TaskStartCreateTasks();
  OSTaskSuspend(OS_PRIO_SELF);
}


/**
 * Release Memory
 */
void ReleaseMem(int i) {
	INT8U err;
	switch(i) {
		case 3:
      err=OSMemPut(CommMem,CommMsg3);
      if (err == OS_ERR_NONE) // Release Memory Block
        kprint("Third memory has been released.");
			else
				kprint("Third memory didn't exist.");
      break;

		case 2:
      err=OSMemPut(CommMem,CommMsg2);
		  if (err == OS_ERR_NONE) // Release Memory Block
		    kprint("Second memory has been released.");
			else
			  kprint("Second memory didn't been released.");
		break;

		case 1:
      err=OSMemPut(CommMem,CommMsg1);
		   	if (err == OS_ERR_NONE) // Release Memory Block
          kprint("First memory has been released.");
				else
				  kprint("First memory didn't been released.");
		break;
	}
}

/**
 * Task Start Create Tasks
 */
static void TaskStartCreateTasks(void) {
  INT8U  i;
  OSTaskCreate(Task, (void *)&TaskData[0], &TaskStk[0][TASK_STK_SIZE - 1], 5);
}

/**
 * Task
 */
void Task(void *pdata) {
  INT8U  err;
  INT16S i=0;

	MemInfo(pdata);
	DisplayMemStat();

	OSTimeDly(100);

  while(1) {
		if(i==0) {
		  CommMsg1 = OSMemGet(CommMem,&err);
			if (err == OS_ERR_NONE) // Released
        kprint("First memory application HAS accept.");
			else
				kprint("First memory alpplication NOT accept.");
  		MemInfo(pdata);
  		DisplayMemStat();
  		OSTimeDly(100);
	  	i++;
		}
		else if(i==3)	{
			for(i=3;i>0;i--) {
        ReleaseMem(i);
        MemInfo(pdata);
        DisplayMemStat();
        OSTimeDly(10);
			}
	 	}
		else if(i==1)	{
      CommMsg2=OSMemGet(CommMem,&err);
			if (err == OS_ERR_NONE) // Allocated
				kprint("Second memory application HAS accept.");
			else
			  kprint("Second memory alpplication NOT accept.");
  		MemInfo(pdata);
      DisplayMemStat();
      OSTimeDly(100);
      i++;
		}
		else {
			CommMsg3=OSMemGet(CommMem,&err);
			if (err == OS_ERR_NONE) // Allocated
			  kprint("Third memory application HAS accept.");
			else
        kprint("Third memory alpplication NOT accept.\n");
			MemInfo(pdata);
			DisplayMemStat();
			OSTimeDly(100);
			i++;
		}
	}
}

/**
 * Memory Create
 */
static void MemoryCreate(void) {
  INT8U err;			                                    
  // Set up and initialize a memory. 2 Block ，for each 128 Bytes.
  CommMem = OSMemCreate(&CommBuf[0][0], 2, 128, &err);
}