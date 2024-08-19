/*******************************************************************************
****** Author: HRD31
******	Date: 
******	Version: 1
******	Description: header file for .
*******************************************************************************/
#ifndef __WATCH_COMMON_H__
#define __WATCH_COMMON_H__

#include <pthread.h> 	/*	pthread_t	*/
#include <semaphore.h>	/*	sem_t	*/
/******************************************************************************/
typedef struct
{
	int argc; 
	char *const *argv; 
	int freq;
	int max_fail_count;
	int is_user_ps;
	pid_t reciever_pid;
	pthread_t wd_thread_id;
	sem_t *sema;
	int sem_alive;
}params_ty;


typedef char fixed_buffer_ty[15]; 
/******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
******************************************************************************/
void* WatchdogLoop(params_ty *args_);

#endif    /*__WATCH_COMMON_H__*/ 
