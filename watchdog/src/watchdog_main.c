/*******************************************************************************
****** Author 	- Atheer
****** Reviewer - Rayan
****** Date		- 03.09.2023
****** Version	- 1
*******************************************************************************/
#define _OPEN_SYS_ITOA_EXT
#define _POSIX_C_SOURCE
#include <stdio.h>
#include <stdlib.h>		/*	exit	*/
#include <assert.h>		/*	assert 	*/
#include <sys/types.h>	/*	getppid	*/
#include <unistd.h>		/*	execv	*/

#include "utils.h"		/* 	RET_IF_BAD */
#include "wd_common.h"	/*	WatchdogLoop	*/
/******************************************************************************/
int main(int argc, char *argv[])
{
	params_ty wd_params = {0};
	
	printf("watch_dog pid %d \n", getpid());
	
	RET_IF_BAD(argc >= 4, "too few arguments", 9);
	/*	extract arguments */
	wd_params.argc = argc - 3;
	wd_params.argv = argv + 3;
	wd_params.freq = atoi(argv[1]);	
	wd_params.max_fail_count = atoi(argv[2]);
	wd_params.is_user_ps = 0;
	wd_params.reciever_pid = getppid();
	wd_params.wd_thread_id = 0;
	
	/*	start watchdog	*/
	WatchdogLoop(&wd_params);
	
	printf("watchdog - just finished \n");

	return 0;
}

