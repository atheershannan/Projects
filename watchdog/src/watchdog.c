/*******************************************************************************
****** Author 	- Atheer
****** Reviewer - Rayan
****** Date		- 03.09.2023
****** Version	- 1
*******************************************************************************/
#define _POSIX_C_SOURCE

#include <stdio.h>		
#include <stdlib.h>		/*	exit	*/
#include <assert.h>		/*	assert */
#include <signal.h>		/*	sigmask, SIGUSR1, SIGUSR2		*/
#include <pthread.h>	/*	pthread_t, pthread_create ,pthread"_join	*/	
#include <sys/types.h>	/*	getpid			*/
#include <unistd.h>		
#include <semaphore.h>	/*	sem_init, sem_destroy */
#include <errno.h>		/*	errno	*/

#include "utils.h"		/*	RET_IF_BAD	*/
#include "wd_common.h"	/*	params_ty	*/
#include "watchdog.h"	/*	WatchdogMakeMeImmortal, WatchdogDoNotResuscitate */

pthread_t g_wd_thread_id = 0;

void* Wd_thread(params_ty *args_);
/******************************************************************************/
int WatchdogMakeMeImmortal(	int argc_, char * const * const argv_, size_t freq_,	
							int fail_count_)
{
	params_ty wd_thread_params = {0};
	sigset_t new_set, old_set;
	sem_t sema;
	
	assert(argc_);
	assert(argv_);
	assert(freq_);
	assert(fail_count_);
	
	printf("user pid %d \n", getpid());
	
	/*	mask signals	*/
	sigemptyset( &new_set );
    sigaddset( &new_set, SIGUSR1 );
    sigaddset( &new_set, SIGUSR2 );
    sigprocmask( SIG_BLOCK, &new_set, &old_set);
	
	RET_IF_BAD(!sem_init(&sema, 0, 0), "sem_init", SYSTEM_ERROR);
    	
	/*	fill wd_thread_params feilds	*/	
	wd_thread_params.argc = argc_;
	wd_thread_params.argv = argv_;
	wd_thread_params.freq = freq_;
	wd_thread_params.max_fail_count = fail_count_;
	wd_thread_params.is_user_ps = 1;
	wd_thread_params.reciever_pid = __INT_MAX__;
	wd_thread_params.sema = &sema;
	wd_thread_params.sem_alive = 1;

	/*	create thread	- WatchdogCreate() */
	pthread_create(	&g_wd_thread_id, NULL, (void * (*)(void *))Wd_thread, 
					&wd_thread_params);
	sem_wait(&sema);
	
	sem_destroy(&sema);

	return 0;
}

/******************************************************************************/
void* Wd_thread(params_ty *args_)
{
	params_ty wd_thread_params = {0};

	/*	fill wd_thread_params feilds	*/	
	wd_thread_params.argc = args_->argc;
	wd_thread_params.argv = args_->argv;
	wd_thread_params.freq = args_->freq;
	wd_thread_params.max_fail_count = args_->max_fail_count;
	wd_thread_params.is_user_ps = args_->is_user_ps;
	wd_thread_params.reciever_pid = args_->reciever_pid;
	wd_thread_params.wd_thread_id = pthread_self();
	wd_thread_params.sema = args_->sema;
	wd_thread_params.sem_alive = args_->sem_alive;

	WatchdogLoop(&wd_thread_params);
		
	return NULL;
}


/******************************************************************************/
int WatchdogDoNotResuscitate()
{
	int kill_status = 0;
	/*	send signal sig2	*/
	do
	{
		kill_status = kill(getpid(), SIGUSR2);
	}while(kill_status && errno != ESRCH);
	
	RET_IF_BAD(!kill_status, "kill failed", SYSTEM_ERROR);
	
	/*	clean up	*/	
	pthread_join(g_wd_thread_id, NULL);

	return 0;
}
