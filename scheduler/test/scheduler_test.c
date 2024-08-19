/*******************************************************************************
*****Auther     - Atheer Shannan
*****Reviewer	- Gaby
*****Date       - 3 jul 2023
*****Version    - 1
*******************************************************************************/
#include <stdlib.h>		/* malloc, free 	*/
#include <time.h> 		/* time_t 			*/
#include <assert.h>		/* assert */
#include <stdio.h>		/* printf */
#include "scheduler.h"
#include "pqueue.h"		/* p_queue, PQueueCreate, PQueueDestroy, PQueueEnqueue,
						   PQueueDequeue, PQueuePeek, PQueueIsEmpty, PQueueSize,
						   PQueueErase	*/
#include "task.h"		/* task_t, TaskCreate, TaskDestroy, TaskGetUID, 
						  TaskExecute, TaskGetExeTime, TaskSetExeTime */
#include "op_funcs.h" 


#define KWHT  "\x1B[37m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define TASKSNUM 10

time_t run_time = 0;

/*******************************************************************************
							function declaration
*******************************************************************************/
/******************************************************************************/
static void Tests();
static void TestSchedCreate();
static void TestSchedAdd();
static void TestSchedRemove();
static void TestSchedRun();
static void TestSchedClear();


int Test1();
int Test2();
int Test3();
int Test4();
int Test5();


/*******************************************************************************
									main
*******************************************************************************/
int main()
{
	time_t t;
	
	/* Intializes random number generator */
	srand((unsigned) time(&t));
	
	run_time = time(NULL);
	
	Tests();
	
	return 0;
}



/*******************************************************************************
									TESTS
*******************************************************************************/
/******************************************************************************/
static void Tests()
{
	TestSchedCreate();
	TestSchedAdd();
	TestSchedRemove();
	TestSchedRun();
	TestSchedClear();
}


/******************************************************************************/
static void TestSchedCreate()
{
	int suc = 1;
	scheduler_t *sched = SchedCreate();
	
	if(!sched)
	{
		suc = 0;
		printf("%sfaild	", KRED);
		printf("%sTestSchedCreate line -  %d\n" ,KWHT, __LINE__ - 4);
	}
	
	if(suc)
	{
		printf("%spass	", KGRN); 
		printf("%sTestSchedCreate\n", KWHT);
	}
	
	/*Destoy sched*/
	SchedDestroy(sched);
}


/******************************************************************************/
static void TestSchedAdd()
{
	scheduler_t *sched;
	op_params_t op_params;
	size_t size = 0, suc = 1, n = 0;
	
	/* Initialize parametrs */
	/* Create sched 		*/
	sched = SchedCreate();
	n = 1 + rand()% 100;
	op_params.repeats = rand() % 100;
	op_params.sched = sched;

	/* add random num of tasks */
	while(n--)
	{
		if(!UidIsEqual(UID_BAD,SchedAdd(sched, printHello, &op_params, 1)))
		{
			size++;
		}
	}
	
	if(SchedIsEmpty(sched) ||  SchedSize(sched)!= size)
	{
		suc = 0;
		printf("%sfaild	", KRED);
		printf("%sTestSchedAdd line -  %d\n" ,KWHT, __LINE__ - 4);	
	}
	
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(suc)
	{
		printf("%spass	", KGRN); 
		printf("%sTestSchedAdd\n", KWHT);
	}
}


/******************************************************************************/
static void TestSchedRemove()
{
	scheduler_t *sched = SchedCreate();
	op_params_t op_params;
	size_t size = 0, suc = 1, n = 0;
	Uid_t uid;
	
	op_params.repeats = rand() % 100;
	op_params.sched = sched;
	n = 1 + rand()% 100;
	while(n--)
	{
		uid = SchedAdd(sched, printHello, &op_params, 1);
		
		if(!UidIsEqual(UID_BAD, uid))
		{
			size++;
		}
		
		if(!SchedRemove(sched, uid))
		{
			size--;
		}
	}
	
	if(!SchedIsEmpty(sched) ||  SchedSize(sched)!= size)
	{
		suc = 0;
		printf("%sfaild	", KRED);
		printf("%sTestSchedRemove line -  %d\n" ,KWHT, __LINE__ - 4);	
	}
	
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(suc)
	{
		printf("%spass	", KGRN); 
		printf("%sTestSchedRemove\n", KWHT);
	}
}


/******************************************************************************/
static void TestSchedRun()
{
	int counter = 0;
	counter += Test1();
	counter += Test2();
	counter += Test3();
	counter += Test4();
	counter += Test5();
	if(!counter)
	{
		printf("%spass	", KGRN); 
		printf("%sTestSchedRun\n", KWHT);
	}
}

/******************************************************************************/
int Test1()
{
	scheduler_t *sched = SchedCreate();
	int i = 0;
	op_params_t params_arr[TASKSNUM];
	FILE *stream = NULL;
	char *my_output = "./tmp/output/output1.txt";

	run_time = time(NULL);
		
	/* fill op_params */
	fillParamsArr(params_arr, TASKSNUM, sched, my_output);
	
	stream = fopen(my_output ,"w");
	fclose(stream);
	
	SchedAdd(sched, printHello, &params_arr[i++], 3);
	SchedAdd(sched, printWorld, &params_arr[i++], 4);
	SchedAdd(sched, printAtheer, &params_arr[i++], 5);
	SchedAdd(sched, getNumOfTasks, &params_arr[i++], 2);
	
	SchedRun(sched);
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(system("diff tmp/output/output1.txt tmp/exp_output/exp_output1.txt"))
	{
		printf("%sfaild	", KRED);
		printf("%sTestSchedRun line -  %d\n" ,KWHT, __LINE__ - 3);	
		return 1;
	}
	
	return 0;
}


/******************************************************************************/
int Test2()
{
	scheduler_t *sched = SchedCreate();
	int i = 0;
	op_params_t params_arr[TASKSNUM];
	FILE *stream = NULL;
	char *my_output = "./tmp/output/output2.txt";
	
	run_time = time(NULL);
	
	/* fill op_params */
	fillParamsArr(params_arr, TASKSNUM, sched, my_output);
	
	stream = fopen(my_output ,"w");
	fclose(stream);
	
	SchedAdd(sched, printHello, &params_arr[i++], 3);
	SchedAdd(sched, printWorld, &params_arr[i++], 4);
	SchedAdd(sched, printAtheer, &params_arr[i++], 5);
	SchedAdd(sched, getNumOfTasks, &params_arr[i++], 2);
	SchedAdd(sched, stop, &params_arr[i++], 6);
	
	SchedRun(sched);
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(system("diff tmp/output/output2.txt tmp/exp_output/exp_output2.txt"))
	{
		printf("%sfaild	", KRED);
		printf("%sTestSchedRun line -  %d\n" ,KWHT, __LINE__ - 3);	
		return 1;
	}
	
	return 0;
}


/******************************************************************************/
int Test3()
{
	scheduler_t *sched = SchedCreate();
	int i = 0;
	op_params_t params_arr[TASKSNUM];
	FILE *stream = NULL;
	char *my_output = "./tmp/output/output3.txt";
	
	run_time = time(NULL);
	
	/* fill op_params */
	fillParamsArr(params_arr, TASKSNUM, sched, my_output);
	
	stream = fopen(my_output ,"w");
	fclose(stream);
	
	SchedAdd(sched, printHello, &params_arr[i++], 3);
	SchedAdd(sched, printWorld, &params_arr[i++], 4);
	SchedAdd(sched, printAtheer, &params_arr[i++], 5);
	SchedAdd(sched, getNumOfTasks, &params_arr[i++], 2);
	SchedAdd(sched, stop, &params_arr[i++], 1);
	
	SchedRun(sched);
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(system("diff tmp/output/output3.txt tmp/exp_output/exp_output3.txt"))
	{
		printf("%sfaild	", KRED);
		printf("%sTestSchedRun line -  %d\n" ,KWHT, __LINE__ - 3);	
		return 1;
	}
	
	return 0;
}



/******************************************************************************/
int Test4()
{
	scheduler_t *sched = SchedCreate();
	op_params_t params_arr[TASKSNUM];
	FILE *stream = NULL;
	char *my_output = "./tmp/output/output4.txt";
	
	run_time = time(NULL);
	
	/* fill op_params */
	fillParamsArr(params_arr, TASKSNUM, sched, my_output);
	
	stream = fopen(my_output ,"w");
	fclose(stream);
	
	SchedAdd(sched, addTasks, params_arr, 1);
	
	SchedRun(sched);
	
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(system("diff tmp/output/output4.txt tmp/exp_output/exp_output4.txt"))
	{
		printf("%sfaild	", KRED);
		printf("%sTestSchedRun line -  %d\n" ,KWHT, __LINE__ - 3);	
		
		return 1;
	}
	
	return 0;
}


/******************************************************************************/
int Test5()
{
	scheduler_t *sched = SchedCreate();
	op_params_t params_arr[TASKSNUM];
	FILE *stream = NULL;
	char *my_output = "./tmp/output/output5.txt";
	char buffer[256] = {0};
	
	run_time = time(NULL);
	
	/* fill op_params */
	fillParamsArr(params_arr, TASKSNUM, sched, my_output);
	
	stream = fopen(my_output ,"w");
	fclose(stream);
	
	SchedAdd(sched, clearTasks, params_arr, 1);
	
	SchedRun(sched);

	stream = fopen(my_output ,"a+");
	sprintf(buffer, "sched size -%lu\n",SchedSize(sched));
	fputs(buffer, stream);
	
	fclose(stream);
	
		
	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(system("diff tmp/output/output5.txt tmp/exp_output/exp_output5.txt"))
	{
		printf("%sfaild	", KRED);
		printf("%sTestSchedRun line -  %d\n" ,KWHT, __LINE__ - 3);	
		
		return 1;
	}
	
	return 0;
}


/******************************************************************************/
static void TestSchedClear()
{
	scheduler_t *sched = NULL;
	op_params_t op_params;
	size_t size = 0, suc = 1 , n = 0;
	
	/*Create sched */
	sched = SchedCreate();
	
	n = 1 + rand() % 100;
	op_params.repeats = rand() % 100;
	op_params.sched = sched;

	/* add random num of tasks */
	while(n--)
	{
		if(!UidIsEqual(UID_BAD,SchedAdd(sched, printHello, &op_params, 1)))
		{
			size++;
		}
	}
	
	n = 1 + rand() % 100;
	/* clear sched */
	while(n--)
	{
		SchedClear(sched);
	}
	
	
	if(!SchedIsEmpty(sched) || SchedSize(sched))
	{
		suc = 0;
		printf("%sfaild	", KRED);
		printf("%sTestSchedClear -  %d" ,KWHT, __LINE__ - 4);	
	}

	/*Destoy sched*/
	SchedDestroy(sched);
	
	if(suc)
	{
		printf("%spass	", KGRN); 
		printf("%sTestSchedClear\n", KWHT);
	}
}

/******************************************************************************/
int compareFolders(const char *dest_file_name, const char *src_file_name)
{
	FILE *src_stream = NULL, *dest_stream = NULL;
	char c1 = '\0' ,c2 = '\0';	
	
	src_stream = fopen(src_file_name ,"r");
	
	if(!src_stream)
	{	
		return 1;
	}
	
	dest_stream = fopen(dest_file_name ,"r");
	
	if(!dest_stream)
	{	
		return 1;
	}

	c1 = fgetc(src_stream);
	c2 = fgetc(dest_stream);
	
		
	while(c1 != EOF || c2 != EOF)
	{
      	if(c1 != c2)
      	{
      		printf("%c	%c",c1 ,c2 );
      		return 1;
      	}
      	
      	c1 = fgetc(src_stream);
		c2 = fgetc(dest_stream);
	}
	
	fclose(src_stream);
	fclose(dest_stream);
	return 0;	
}

