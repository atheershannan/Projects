/*******************************************************************************
*****Auther     - Atheer Shannan
*****Reviewer	- Gaby
*****Date       - 3 jul 2023
*****Version    - 1
*******************************************************************************/
#include <stdlib.h>		/* malloc, free 	*/
#include <time.h> 		/* time 			*/
#include <assert.h>		/* assert			*/
#include<unistd.h>		/* sleep			*/

#include "scheduler.h"
#include "pqueue.h"		/* 	p_queue, PQueueCreate, PQueueDestroy, 
							PQueueEnqueue, PQueueDequeue, 
							PQueuePeek, PQueueIsEmpty, 
						   	PQueueSize, PQueueErase					*/
#include "task.h"		/* 	task_t, TaskCreate, TaskDestroy, 
							TaskGetUID, TaskExecute, 
						  	TaskGetExeTime, TaskSetExeTime 			*/
#include "uid.h" 		/* 	Uid_t , UidCreate, UidIsEqual 			*/
#define SUC 0
#define FAIL 1

typedef enum
{
 	NEG = -1,
 	ZERO = 0,
	POS = 1
}sort_ret_vals;

typedef enum
{
	OFF = 0, 
	ON  = 1
}sched_status;


typedef enum
{
	FALSE = 0, 
	TRUE = 1
}bool_status;

/*******************************************************************************
							functions declaration
*******************************************************************************/
int priorityFunc(const void *data, const void *new_data);
int isMatchUID(const void *task, const void *uid);

/*******************************************************************************
									STRUCTS
*******************************************************************************/
struct scheduler
{
	p_queue_t *queue;
	int is_running;
	task_t *running_task;
};

/******************************************************************************/
int priorityFunc(const void *data, const void *new_data)
{
	task_t *data_task = (task_t *)data ;
	task_t *new_data_task = (task_t *)new_data;
	
	if(TaskGetExeTime(data_task) > TaskGetExeTime(new_data_task))
	{
		return NEG;
	}

	if(TaskGetExeTime(data_task) < TaskGetExeTime(new_data_task))
	{
		return POS;
	}
	
	return ZERO;
}


/******************************************************************************/
int isMatchUID(const void *task, const void *uid)
{
	Uid_t task_uid = TaskGetUID((task_t *)task);
	Uid_t uid_to_compare_to = *(Uid_t *)uid;

	if(UidIsEqual(task_uid, uid_to_compare_to))
	{
		TaskDestroy((task_t *)task);
		return TRUE;
	}	
	
	return FALSE;
}


/*******************************************************************************
							functions implementation
*******************************************************************************/
scheduler_t *SchedCreate(void)
{
	/* Allocates scheduler */
	scheduler_t *new_sched = (scheduler_t*)malloc(sizeof(*new_sched));
	
	if(!new_sched)
	{
		return NULL;	
	}
	
	/* Creates new priority queue */
	new_sched->queue = PQueueCreate(priorityFunc);
	
	if(!new_sched->queue)
	{
		free(new_sched); new_sched = NULL;
		return NULL; 
	}
	
	/* fill scheduler fields*/
	new_sched->is_running = OFF;
	new_sched->running_task = NULL;
	
	return new_sched;
}


/******************************************************************************/
void SchedDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	SchedClear(scheduler);
	
	PQueueDestroy(scheduler->queue); scheduler->queue = NULL;
	
	free(scheduler); scheduler = NULL;
}


/******************************************************************************/
Uid_t SchedAdd(
    scheduler_t *scheduler,
    op_func_t op_func,
    void *params,
    size_t interval
) 
{
	task_t *new_task = NULL;
	
	assert(NULL != scheduler);
	assert(NULL != op_func);
	
	/* Creates new task */
	new_task = TaskCreate(op_func, params, interval);
	
	if(!new_task)
	{
		return UID_BAD;
	}
	
	/* Insert task to queue */
	if(PQueueEnqueue(scheduler->queue, new_task))
	{
		TaskDestroy(new_task); new_task = NULL;
		return UID_BAD;
	}
	
	/* return task's uid */
	return TaskGetUID(new_task);
}


/******************************************************************************/
int SchedRemove(scheduler_t *scheduler, Uid_t task_uid_to_remove)
{
	assert(NULL != scheduler);
	
	assert(!UidIsEqual(UID_BAD, task_uid_to_remove));
	
	if(scheduler->running_task && 
	   UidIsEqual(TaskGetUID(scheduler->running_task), task_uid_to_remove))
	{
		scheduler->running_task = NULL;
		return SUC;
	}
	
	return PQueueErase(scheduler->queue, isMatchUID, &task_uid_to_remove);
}


/******************************************************************************/
int SchedRun(scheduler_t *scheduler)
{
	time_t curr_time = 0, time_to_sleep = 0;
	p_queue_t *tasks_queue = NULL;
	task_t *curr_task = NULL;
	int exec_return_val = 0;
	
	assert(NULL != scheduler);
	
	tasks_queue = scheduler->queue;
	
	if (PQueueIsEmpty(tasks_queue))
	{
		return SUC;
	}
	
	/* Update running Flags */
	scheduler->is_running = ON;
	
	/* Execute all tasks   */
	while (scheduler->is_running && !PQueueIsEmpty(tasks_queue))
	{
		curr_time = time(NULL);
		
		if(curr_time == (time_t)-1)
		{
			SchedStop(scheduler);
			
			return FAIL;
		}
		
		curr_task = PQueuePeek(tasks_queue);
		PQueueDequeue(tasks_queue);
		
		scheduler->running_task = curr_task;
		
		time_to_sleep = TaskGetExeTime(curr_task) - curr_time;
		
		while(time_to_sleep > 0)
		{
			time_to_sleep = sleep(time_to_sleep);
		}
		
		/* Execute 'curr_task' */
		exec_return_val = TaskExecute(curr_task);
		
		if (exec_return_val == NO_RET_TO_SCHED || !scheduler->running_task)
		{
			TaskDestroy(curr_task);	curr_task = NULL;
			scheduler->running_task = NULL;

			continue;
		}
		
		curr_time = time(NULL);
		
		if (exec_return_val == EXE_FAIL || curr_time == (time_t)-1)
		{
			TaskDestroy(curr_task);	curr_task = NULL;
			
			SchedStop(scheduler);
			
			return FAIL;
		}
		
		/* 'exec_return_val' == 0 then 		 	*/
		/*	• Set curr_task's execution time 	*/
		TaskSetExeTime(curr_task, curr_time);
		
		/* 	• Insert 'curr_task' to sched */
		if (PQueueEnqueue(tasks_queue, curr_task))
		{
			TaskDestroy(curr_task);	curr_task = NULL;
			
			SchedStop(scheduler);
			
			return FAIL;
		}
	}
	
	SchedStop(scheduler);
	
	return SUC;
}


/******************************************************************************/
void SchedStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	scheduler->running_task = NULL;			
	scheduler->is_running = OFF;
}


/******************************************************************************/
size_t SchedSize(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	return PQueueSize(scheduler->queue) + scheduler->is_running;
}


/******************************************************************************/
int SchedIsEmpty(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	return PQueueIsEmpty(scheduler->queue) && !scheduler->is_running;
}


/******************************************************************************/
void SchedClear(scheduler_t *scheduler)
{
	task_t *task_to_destroy = NULL;
	
	assert(NULL != scheduler);
	
	if(scheduler->running_task)
	{
		scheduler->running_task = NULL;
	}
	
	/* clear all queue elements */
	while(!PQueueIsEmpty(scheduler->queue))
	{
		task_to_destroy = PQueuePeek(scheduler->queue);
		TaskDestroy(task_to_destroy); task_to_destroy = NULL;
		PQueueDequeue(scheduler->queue);
	}	
}
