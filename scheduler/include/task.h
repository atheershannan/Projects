/*******************************************************************************
****** Author: HRD31
****** Date: 03.07.2023
****** Version: 1
*******************************************************************************/
#ifndef __TASK_H__
#define __TASK_H__

#include <time.h>  /* time_t */

#include "uid.h"   /* Uid_t  */
#include "scheduler.h"  /* op_func_t */

typedef struct task task_t;
/*******************************************************************************
Description:     	Creates a task.
Return value:    	Pointer to task in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'TaskDestroy()' at end of use.
*******************************************************************************/
task_t *TaskCreate(op_func_t op_func, void *op_params, size_t interval);


/*******************************************************************************
Description:     	Deletes a task pointed by 'task' from memory.
Time Complexity: 	O(1) 
Notes:           	Undefined behaviour if task is NULL.
*******************************************************************************/
void TaskDestroy(task_t *task);


/*******************************************************************************
Description:     	Gets UID for 'task'.
Return value:    	task's Uid.         
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'task' is invalid pointer.
*******************************************************************************/
Uid_t TaskGetUID(task_t *task);


/*******************************************************************************
Description:     	Executes 'task'.
Return value:    	0 insert to sched.
                    1 finish task execution with no insirtion to sched.
                    2 stops the schedualer.         
Time Complexity: 	Depends on 'task' complexity 
Notes:              Undefined behaviour if task is invalid pointer.
*******************************************************************************/
int TaskExecute(task_t *task);


/*******************************************************************************
Description:     	Get task's interval.
Return value:    	Task's interval
Time Complexity: 	O(1).
Notes:              Undefined behaviour if task is invalid pointer.
*******************************************************************************/
size_t TaskGetInterval(task_t *task);


/*******************************************************************************
Description:     	Get task's execution time.
Return value:    	Task's execution time.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if task is invalid pointer.
*******************************************************************************/
time_t TaskGetExeTime(task_t *task);


/*******************************************************************************
Description:     	Set task's execution time to current time (time(NULL)) 
					+ interval.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if task is invalid pointer.
*******************************************************************************/
void TaskSetExeTime(task_t *task, time_t run_time);


#endif    /*__TASK_H__*/
