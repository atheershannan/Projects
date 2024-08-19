/*******************************************************************************
*****Auther     - Atheer Shannan
*****Reviewer	-  
*****Date       - 3 jul 2023
*****Version    - 1
*******************************************************************************/
#include <stdlib.h>		/* malloc, free 	*/
#include <time.h> 		/* time_t 			*/
#include <assert.h>		/* assert			*/

#include "task.h"
#include "uid.h" 		/* Uid , UidCreate 	*/
#include "scheduler.h"  /* op_func_t		*/

#define SUC 0
#define FAIL 1


/*******************************************************************************
									STRUCTS
*******************************************************************************/
struct task
{
	Uid_t uid;
	op_func_t op_func;
	void *op_params;
	size_t interval;
	time_t exe_time;
};

/*******************************************************************************
							functions implementation
*******************************************************************************/
task_t *TaskCreate(op_func_t op_func, void *op_params, size_t interval)
{
	task_t *new_task = NULL;
	time_t curr_time = time(NULL);
	
	assert(NULL != op_func);
	
	if(curr_time == (time_t)-1)
	{
		return NULL;
	}
	
	/* create new task */
	new_task = (task_t *)malloc(sizeof(*new_task));
	
	if(!new_task)
	{
		return NULL;
	}
	
	/* create new task */
	new_task->uid = UidCreate();
	
	if(UidIsEqual(UID_BAD, new_task->uid))
	{
		free(new_task); new_task = NULL;
		return NULL;
	}
	
	/*initialize struct task's fields */
	new_task->op_func 	= op_func;
	new_task->op_params = op_params;
	new_task->interval 	= interval;
	new_task->exe_time 	= interval + curr_time;
	
	return new_task;
}


/******************************************************************************/
void TaskDestroy(task_t *task)
{
	assert( NULL != task);
	
	free(task); task = NULL;
}


/******************************************************************************/
Uid_t TaskGetUID(task_t *task)
{
	assert( NULL != task);
	
	return task->uid;
}


/******************************************************************************/
int TaskExecute(task_t *task)
{
	assert( NULL != task );
	assert( NULL != task->op_func );
	
	return (task->op_func)(task->op_params);
}


/******************************************************************************/
size_t TaskGetInterval(task_t *task)
{
	assert( NULL != task);
	
	return task->interval;
}


/******************************************************************************/
time_t TaskGetExeTime(task_t *task)
{
	assert( NULL != task);
	
	return task->exe_time; 
}


/******************************************************************************/
void TaskSetExeTime(task_t *task, time_t run_time)
{
	assert( NULL != task);
	
	task->exe_time = run_time + task->interval;
}

