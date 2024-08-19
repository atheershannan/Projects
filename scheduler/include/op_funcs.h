/*******************************************************************************
****** Author: Atheer
****** Date: 04.07.2023
****** Version: 1
*******************************************************************************/
#ifndef __OP_FUNCS_H__
#define __OP_FUNCS_H__

typedef struct{

	int repeats;
	scheduler_t *sched;
	char *filename;
	
}op_params_t;

/******************************************************************************
Description:     	Performs specific action on data.
Return value:    	Returns 0 for success, 1 for fail
*******************************************************************************/
void fillParamsArr(op_params_t *params_arr, int size, scheduler_t *sched
							,char *filename);

int printHello(void *params);
int printWorld(void *params);
int printAtheer(void *params);
int getNumOfTasks(void * params);
int addTasks(void *params);
int stop(void * params);
int clearTasks(void *params);

int isEmpty(void *params);
int readFromFolder(void * params);

#endif    /*__OP_FUNCS_H__*/
