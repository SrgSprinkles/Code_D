#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//Checks if there is one arg. and if the arg. is a positive integer
	if (argc != 2) {
		printf("ERROR: Enter only one integer parameter\n");
		return 0;
	} else {
		if (atoi(argv[1]) <= 0) {
			printf("ERROR: Enter a postive integer parameter\n");
			return 0;
		}
	}

	//Creates the 3 children and displays their PIDs and PPIDs
	for (int i = 0; i < 3; i++) {
		if (!fork()) {
			for (int j = 0; j < atoi(argv[1]); j++) {
				printf("This is a child process, my PID is %d, my parent PID is %d\n", getpid(), getppid());
				sleep(1);
				wait(NULL);
			}
			
			exit(0);
		}
	}

	//Displays the parents PIDs N times
	for (int k = 0; k < atoi(argv[1]); k++) {
		printf("This is the main process, my PID is %d\n", getpid());
		sleep(1);
	}
	wait(NULL);			//Wait for children to terminate
	
	return 0;
}
