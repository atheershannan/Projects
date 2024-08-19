/*******************************************************************************
****** Author: HRD31
****** Date: 03.07.2023
****** Version: 4
*******************************************************************************/
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>  /* size_t */

#include "uid.h"     /* Uid_t  */

typedef struct scheduler scheduler_t;

typedef enum
{
	RET_TO_SCHED = 0, 
	NO_RET_TO_SCHED = 1, 
	EXE_FAIL = 2
}op_ret_vals; 

/******************************************************************************
Description:     	Function that will be executed when scheduler will execute
                    the task associated with this function.
Return value:    	0 insert to sched
                    1 finish task execution with no insertion to sched
                    2 stops the scheduler
*******************************************************************************/
typedef int (*op_func_t)(void *params);


/*******************************************************************************
Description:     	Creates an empty scheduler.
Return value:    	Pointer to scheduler in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'SchedDestroy()' at end of use.
                    Undefined behaviour if used inside operation function.
*******************************************************************************/
scheduler_t *SchedCreate(void);


/*******************************************************************************
Description:     	Deletes a scheduler pointed by 'sched' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if scheduler is NULL.
                    Undefined behaviour if used inside operation function.
*******************************************************************************/
void SchedDestroy(scheduler_t *sched);


/*******************************************************************************
Description:     	Add 'task' to scheduler pointed by 'sched'.
Return value:    	task 'Uid' for success, 'UID_BAD' for fail.         
Time Complexity: 	O(n)
Notes:              interval in seconds
                    Undefined behaviour if 'sched' is invalid pointer.
                    Undefined behaviour if 'op_func' is invalid pointer.
*******************************************************************************/
Uid_t SchedAdd(
    scheduler_t *sched,
    op_func_t op_func,
    void *params,
    size_t interval
);


/*******************************************************************************
Description:     	Remove task with 'task_uid_to_remove' from scheduler pointed
                    by 'sched'.
Return value:    	0 for success, 1 for fail (not found).         
Time Complexity: 	O(n)
Notes:              Undefined behaviour if 'sched' is invalid pointer.
*******************************************************************************/
int SchedRemove(scheduler_t *sched, Uid_t task_uid_to_remove);


/*******************************************************************************
Description:     	Run all tasks in scheduler pointed by 'sched'.
Return value:    	0 for success, 1 for fail (not found).     
Time Complexity: 	Depends Tasks to be executed
Notes:              Undefined behaviour if 'sched' is invalid pointer.
                    Undefined behaviour if used inside operation function.
*******************************************************************************/
int SchedRun(scheduler_t *sched);


/*******************************************************************************
Description:     	stops the run of all tasks in scheduler pointed by
                    'sched'.
Time Complexity: 	O(1)
Notes:              Can be used as an operation function inside tasks to stop
                    the running of the scheduler.
                    Undefined behaviour if 'sched' is invalid pointer.
*******************************************************************************/
void SchedStop(scheduler_t *sched);


/*******************************************************************************
Description:     	Returns number of elements in 'sched'
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if 'sched' is invalid pointer
*******************************************************************************/
size_t SchedSize(const scheduler_t *sched);


/*******************************************************************************
Description: 		Checks if 'sched' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'sched' is invalid pointer.
*******************************************************************************/
int SchedIsEmpty(const scheduler_t *sched);


/*******************************************************************************
Description: 		remove all tasks from 'schedr'.
Time complexity:  	O(n)
Note: 				Undefined behaviour if 'sched' is invalid pointer.
*******************************************************************************/
void SchedClear(scheduler_t *sched);

#endif    /*__SCHEDULER_H__*/
