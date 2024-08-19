/*******************************************************************************
*****Auther     - Atheer Shannan
*****Reviewer	- Gaby
*****Date       - 3 jul 2023
*****Version    - 1
*******************************************************************************/
#include <stdlib.h>		/* malloc, free 	*/
#include <assert.h>		/* assert			*/
#include <stdio.h> 		/* FILE, fopen, fclose, sprintf */
#include "scheduler.h"
#include "op_funcs.h"

#define TASKSNUM 10

extern time_t run_time;
char buffer[256] = {0};

int printHello(void *params)
{	
	op_params_t *cast_params = (op_params_t *) params;
	FILE * stream = NULL;
	
	if(!cast_params->repeats)
	{
		return 1;
	}
	
	(cast_params->repeats)--;
	
	stream = fopen( cast_params->filename,"a+");
	
	fputs("Hello ", stream);
	
	fclose(stream);
	
	
	return  0;
}

int printWorld(void *params)
{
	op_params_t *cast_params = (op_params_t *) params;
	FILE * stream = NULL;
	
	if(!cast_params->repeats)
	{
		return 1;
	}

	
	stream = fopen( cast_params->filename,"a+");
	
	
	fputs("World\n", stream);
	
	fclose(stream);
	
	cast_params->repeats--;
	
	return  0;
}


int printAtheer(void *params)
{
	op_params_t *cast_params = (op_params_t *) params;
	FILE * stream = NULL;
	
	assert(NULL != params);
	
	if(!cast_params->repeats)
	{
		return 1;
	}

	stream = fopen( cast_params->filename,"a+");
	
	
	sprintf(buffer, "Run time %lu	-		Atheer\n",time(NULL) - run_time);
	fputs(buffer, stream);
	
	fclose(stream);
	
	cast_params->repeats--;
	
	return  0;
}

int getNumOfTasks(void * params)
{
	op_params_t *cast_params = (op_params_t *) params;
	int repeats = cast_params->repeats;
	scheduler_t *sched = cast_params->sched;
	FILE * stream = NULL;
	
	if(!repeats)
	{
		return 1;
	}
		
	stream = fopen(cast_params->filename ,"a+");
	
	sprintf(buffer, "Run time %lu	-		",time(NULL) - run_time);
	fputs(buffer, stream);
	sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
	fputs(buffer, stream);
	
	
	
	fclose(stream);
	cast_params->repeats--;
	return 0;
}


/******************************************************************************/
int isEmpty(void *params)
{
	op_params_t *cast_params = (op_params_t *) params;
	int repeats = cast_params->repeats;
	scheduler_t *sched = cast_params->sched;
	FILE * stream = NULL;
	
	if(!repeats)
	{
		return 1;
	}
	
	stream = fopen("./tmp/isEmpty.txt" ,"a+");
	fputs("size", stream);
	if(SchedIsEmpty(sched))
	{
		fputs("sched is empty", stream);	
	}
	else
	{
		fputs("sched not empty", stream);
	}
	
	fclose(stream);

	cast_params->repeats--;
	return 0;	

}


/******************************************************************************/
int stop(void * params)
{
	op_params_t *cast_params = (op_params_t *) params;
	int repeats = cast_params->repeats;
	scheduler_t *sched = cast_params->sched;
	
	if(!repeats)
	{
		return 1;
	}
	
	SchedStop(sched);
	cast_params->repeats--;
	
	return 0;	
}


/******************************************************************************/
int addTasks(void *params)
{
	op_params_t *cast_params = (op_params_t *) params;
	scheduler_t *sched = cast_params->sched;
	int n = TASKSNUM, i = 0;
	FILE * stream = NULL;
	
	if(!cast_params->repeats)
	{
		return 1;
	}
	
	stream = fopen(cast_params->filename ,"a+");
	
	sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
	fputs(buffer, stream);
	
	
	while(n)
	{
		SchedAdd(sched, printAtheer, cast_params + i, n);
		
		sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
		fputs(buffer, stream);
		
		i++;
		n--;
	}

	fclose(stream);
	cast_params->repeats--;
	
	return 0;	
}


/******************************************************************************/
int clearTasks(void *params)
{
	op_params_t *cast_params = (op_params_t *) params;
	scheduler_t *sched = cast_params->sched;
	int n = TASKSNUM, i = 0 ;
	FILE * stream = NULL;
	
	stream = fopen(cast_params->filename ,"a+");

	while(n)
	{
		sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
		fputs(buffer, stream);

		SchedAdd(sched, printAtheer, cast_params + i, n);
				
		i++;
		n--;
	}
	
	sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
	fputs(buffer, stream);
	
	SchedClear(sched);
		
	sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
	fputs(buffer, stream);
	
	fclose(stream);

	return 0;	
}


/******************************************************************************/
void fillParamsArr(op_params_t *params_arr, int size, scheduler_t *sched
							,char *filename)
{
	int i = 0;
	
	for(i = 0; i < size; i++)
	{
		if( i == 1)
		{
			params_arr[i].repeats = params_arr[i-1].repeats;
		}
		else
		{
			params_arr[i].repeats = i + 1;
		}
		params_arr[i].sched = sched;
		params_arr[i].filename = filename;
	}
}


