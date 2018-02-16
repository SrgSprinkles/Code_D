#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

int main(int argc, char *argv[])
{
	
	//Checks if there is one arg. and if the arg. is a positive integer
	if (argc != 2) {
		printf("ERROR: Enter only one \"filename\" parameter\n");
		return 0;
	} else {
		std::ifstream ifs (argv[1]);
		if (!ifs) {
			printf("ERROR: Enter a file that exists\n");
			return 0;
		}
		ifs.close();
	}

	//Creates the 3 children and goes through each one and each does different things
	for (int i = 0; i < 3; i++) {
		if (!fork()) {
			for (int j = 0; j < 1; j++) {
				if (i == 0) {
					execl("/bin/ls", "", "-l", NULL);
					exit(0);
				} else if (i == 1) {
					execl("/bin/ps", "", "-ef", NULL);
					exit(0);
				} else if (i == 2) {
					execl("/bin/cat", "", argv[1], NULL);
					exit(0);
				}
			}
			exit(0);
		} else {
			printf("This is the main process, my PID is %d\n", getpid());
		}
		sleep(1);
	}

	printf("Main process terminates\n");
	sleep(1);
	wait(NULL);

	return 0;
}
