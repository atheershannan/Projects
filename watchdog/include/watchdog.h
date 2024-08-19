/*******************************************************************************
****** Author:	Atheer
******	Date:	03.09.2023	
******	Version: 1
******	Description: watchdog.
*******************************************************************************/
#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__


typedef enum {MEMORY_ERROR = 1, SYSTEM_ERROR = 5,PTHREAD_ERROR = 100} status_t;
/*******************************************************************************
Description		-	make the calling process immortal
Return value	-	non-zero value on fail.
Time Complexity	-	
Notes			-	* Call WatchdogDoNotResuscitate() before exiting.
					* Uses SIGUSR1/2.
					* Masks SIGUSR1/2, don't unmask them on your threads.
					* Should call this function from main thread before 
						creating any other thread.
					* argc <= 128
*******************************************************************************/
int WatchdogMakeMeImmortal(	int argc_, char * const * const argv_, size_t freq_,	
							int fail_count_);


/*******************************************************************************
Description		-	
Return value	-	
Time Complexity	-     
Notes			-    	
*******************************************************************************/
int WatchdogDoNotResuscitate();

#endif    /*__WATCH_DOG_H__*/ 
