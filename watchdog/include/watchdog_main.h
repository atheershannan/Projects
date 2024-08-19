/*******************************************************************************
****** Author:	Atheer
******	Date:	03.09.2023	
******	Version: 1
******	Description: watchdog.
*******************************************************************************/
#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

typedef struct
{
	int argc; 
	char *const *argv; 
	int freq;
	int max_fail_count;
	int is_thread;
	pid_t reciever_pid;
	char ** new_allocated_argv;
}params_ty;




/*******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
*******************************************************************************/
void* WatchdogLoop(params_ty *args_);


#endif    /*__WATCH_DOG_H__*/ 
