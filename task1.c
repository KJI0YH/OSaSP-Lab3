#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>

//get PID PPID and Time
void getTime(char* caller){
	struct timeval tv;
	
	if (gettimeofday(&tv, NULL) == -1)
		perror("Can not get current time\n");
	else {
	
		int mls = tv.tv_usec / 1000;
		int ss = tv.tv_sec % 60;
		int mm = (tv.tv_sec / 60) % 60;
		int hh = (tv.tv_sec / 3600 + 3) % 24;
	
		printf("%s PID %d PPID %d Time: %02d:%02d:%02d:%03d\n\n", caller, getpid(), getppid(), hh, mm, ss, mls);	
	}
}

//waiting a child process
void waitChild(pid_t pid) {
	if (waitpid(pid, NULL, 0) == -1)
		perror("wait pid failure\n");
}

void main(void){
	pid_t child1, child2;
	
	//parent process
	if ((child1 = fork())>0 && (child2 = fork())>0) {
		getTime("Parent");
		
		if (system("ps -x") == -1)
			perror("child (system) process could not be created\n");
		
		waitChild(child1);
		waitChild(child2);
	}
	
	//check child1
	switch (child1) {

		//child1 error
		case -1:
			perror("Child 1 process could not be created\n");
			break;
			
		//child1 process
		case 0:
			getTime("Child 1");
			return;
			
		//child1 is okay, check child2
		default: {
			switch (child2) {
	
				//child2 error
				case -1:
					perror("Child 2 process could not be created\n");
					waitChild(child1);
	
				//child2 process
				case 0:
					getTime("Child 2");
				default:
					return;
			}		
		}	
	}
	return;
}
