/*******************************************************************************
****** Author 	- Atheer
****** Reviewer - 
****** Date		- 
****** Version	- 1
*******************************************************************************/
#include <stdio.h>	/*	printf	*/
#include <time.h>	/* time_t	*/
#include <unistd.h> /* sleep 	*/

#include "watchdog.h"	

int main(int argc, char *argv[])
{	
	time_t time_to_sleep = 60;
	WatchdogMakeMeImmortal(argc, argv, 3, 3);	
	
	while((time_to_sleep = sleep(time_to_sleep))){}
		
	WatchdogDoNotResuscitate();
	
	/*	do stuff	*/
	time_to_sleep = 20;
	while((time_to_sleep = sleep(time_to_sleep))){}
	
	printf("user- just finished \n");	

	return 0;
}
