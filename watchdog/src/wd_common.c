/*******************************************************************************
****** Author 	- Atheer
****** Reviewer - Rayan
****** Date		- 03.09.2023
****** Version	- 1
*******************************************************************************/
#define _OPEN_SYS_ITOA_EXT
#define _POSIX_C_SOURCE

#include <stdio.h>		/*	printf	*/
#include <stdlib.h>		/*	exit	*/
#include <assert.h>		/*	assert 	*/
#include <signal.h>		/*	sigmask, SIGUSR1, SIGUSR2	*/
#include <pthread.h>	/*	pthread_create	*/	
#include <semaphore.h>	/*	sem_post	*/	
#include <sys/types.h>	/*	getpid	*/
#include <unistd.h>		/*	execv	*/
#include <sys/wait.h>	/*	wait	*/
#include <errno.h>		/*	errno 	*/
#include "utils.h"		/*	EXIT_IF_BAD	*/

#include "wd_common.h"	/*	params_ty	*/
#include "watchdog.h"	/*	status_t	*/
#include "scheduler.h"	/*	scheduler_t, SchedCreate, SchedRun, SchedDestroy*/
#include "uid.h"		/*	UidIsEqual	*/
		  
enum {MAX_ARGV = 128};

/*******************************************************************************
						static functoin declaration
*******************************************************************************/
/******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
******************************************************************************/
static void Resuscitate(params_ty *arg_);


/******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
******************************************************************************/
static int sendSigNCheck(params_ty *arg_);


/******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
******************************************************************************/
static void SigUsr1Handler(int sig_);


/*******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
*******************************************************************************/
static void SigUsr2Handler(int sig_);

/*******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
*******************************************************************************/
static void PrepWdArgs(char **argv_, params_ty *data, 
								fixed_buffer_ty args_buffer[]);

/*******************************************************************************
						global variables definetion
*******************************************************************************/
volatile int g_fail_counter = 0;
volatile int g_got_signal = 0;
volatile int g_resuscitate_flag = 1; 
volatile int g_reciever_pid = 0;
char g_exe_file[] = "watchdog_main.out"; 

/******************************************************************************/
void* WatchdogLoop(params_ty *args_)
{	
	scheduler_t *sched = NULL;
	sigset_t new_set, old_set;
	struct sigaction usr1 = {0}, usr2 ={0};
	
	/* assert */
	assert(args_);

	if(args_->is_user_ps)
	{
		g_fail_counter = args_->max_fail_count;
	}
	else
	{
		g_got_signal = 1;
	}
	
	/*	unblock signals */
	sigemptyset( &new_set );
    sigaddset( &new_set, SIGUSR1 );
    sigaddset( &new_set, SIGUSR2 );
    sigprocmask( SIG_UNBLOCK, &new_set, &old_set);
    
	usr1.sa_handler = SigUsr1Handler;
	
	/* sigaction	*/
	EXIT_IF_BAD(!sigaction(SIGUSR1, &usr1, NULL), "sigaction failed", 
				PTHREAD_ERROR);
	/* sigaction	*/
	usr2.sa_handler = SigUsr2Handler;
	EXIT_IF_BAD(!sigaction(SIGUSR2, &usr2, NULL), "sigaction failed", 
				PTHREAD_ERROR);
	
	/*	create sched	*/
	EXIT_IF_BAD(sched = SchedCreate(), "schedulerCreate failed", MEMORY_ERROR);
	
	EXIT_IF_BAD(!UidIsEqual(UID_BAD, SchedAdd(sched, 
				(int (*)(void* ))sendSigNCheck,(void *)args_, args_->freq)) 
				,"TaskCreate faild", MEMORY_ERROR);
		
	/*	run sched	*/
	SchedRun(sched);		
	
	/* destroy shed	*/	
	SchedDestroy(sched);
	
	return NULL;
}


/******************************************************************************/
static int sendSigNCheck(params_ty *arg_)
{
	assert(arg_);
	
	
	if(g_resuscitate_flag)
	{
		if(arg_->max_fail_count <= g_fail_counter)
		{
			Resuscitate(arg_);	
		}
	}
	else /* DNR was called */
	{
		if(arg_->is_user_ps)
		{
			int kill_status = 0 ;
			
			do
			{
				kill_status = kill(g_reciever_pid, SIGUSR2);
			}while(kill_status && errno != ESRCH);
		}
				
		/*	don't return to sched	*/
		return NO_RET_TO_SCHED;
	}
	
	if(g_resuscitate_flag && g_got_signal)
	{
		int kill_status = 0 ;
			
		do
		{
			kill_status = kill(arg_->reciever_pid, SIGUSR1);
		}while(kill_status && errno != ESRCH);

		if(g_resuscitate_flag && errno == ESRCH)
		{
			Resuscitate(arg_);
		}
		
		if(arg_->sem_alive)
		{
			sem_post(arg_->sema);
			arg_->sem_alive = 0;
		}
	}

	++g_fail_counter;
	
	return RET_TO_SCHED; 
}


/******************************************************************************/
static void Resuscitate(params_ty *arg_)
{
	assert(arg_);
	
	/* other side alive */
	if(0 == kill(arg_->reciever_pid, 0))
	{
		int kill_status = 0 ;
		
		do
		{
			kill(arg_->reciever_pid, SIGKILL);	
		}while(kill_status && errno != ESRCH);
	}
		
	if(arg_->is_user_ps)
	{			
		pid_t new_pid = 0;
		
		char *wd_argv[MAX_ARGV] = {0};
		
		waitpid(arg_->reciever_pid ,NULL, WNOHANG);
				
		new_pid = fork();
		
		EXIT_IF_BAD(new_pid >= 0, "fork failed", SYSTEM_ERROR);
		
		if(new_pid == 0)
		{
			fixed_buffer_ty args_buffer[2] = {{0}};
			
			arg_->reciever_pid = getppid();
			g_reciever_pid = arg_->reciever_pid;
			PrepWdArgs(wd_argv, arg_, args_buffer);
				
			execv(g_exe_file, wd_argv);	
			
			EXIT_IF_BAD(0, "execv failed", SYSTEM_ERROR);
		}
		else
		{
			arg_->reciever_pid = new_pid;
			g_reciever_pid = arg_->reciever_pid;
			g_fail_counter = 0;			
		}	
	}
	else /*	watchdog process*/
	{
		execv(arg_->argv[0], arg_->argv);
		EXIT_IF_BAD(0, "execv failed", SYSTEM_ERROR);
	}
}

/******************************************************************************/
static void SigUsr1Handler(int sig_)
{
	/*	assert */
	assert(sig_ == SIGUSR1);
	
	/*	reset counter */
	g_fail_counter = 0;
	
	/*	turn on got signal flag	*/
	g_got_signal = 1;
}


/******************************************************************************/
static void SigUsr2Handler(int sig_)
{
	/*	assert */
	assert(sig_ == SIGUSR2);
	
	g_resuscitate_flag = 0;
	
	printf("***********************************************\n");
	printf("%d got USR2\n", getpid());
		
}


/******************************************************************************/
static void PrepWdArgs(char **argv_, params_ty *data, 
								fixed_buffer_ty args_buffer[])
{	
	int argc = data->argc, i = 0;
	
	argv_[0] = g_exe_file;	
	/*	add spcial args	*/
	sprintf(args_buffer[0], "%d",data->freq);
	argv_[1] = args_buffer[0];
	sprintf(args_buffer[1], "%d", data->max_fail_count);
	argv_[2] = args_buffer[1];

	/* append user argv */
	for(i = 0; i < argc; ++i)
	{
		argv_[i + 3] =data->argv[i] ;
	}
	
	argv_[i + 3] = NULL;		
}
