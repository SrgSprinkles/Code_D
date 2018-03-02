/* Name:       Evan Pringle
 * Class:      EECE4029 / Opr. Systems
 * Assignment: Homework 3 - Part B
 * Due Date:   February 23, 2018
 */

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int SHM_SIZE = 1024; /* make it a 1K shared memory segment */
const char FILE_NAME[] = "shm-example.dat";

int main(int argc, char *argv[])
{
	key_t key;
	char *data;
	int shmid;
	int n;
	
	/* make the key: */
	if ((key = ftok(FILE_NAME, 1)) == -1) {
		perror("ftok");
		exit(1);
	}

	/* connect to (and possibly create) the segment: */
	if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}

	/* attach to the segment to get a pointer to it: */
	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1)) {
		perror("shmat");
		exit(1);
	}

	/* read the segment */
	n = 0;
	while ( (n < 10) && (strcmp(data, "STOP") != 0) )
	{
		printf("segment contains: \"%s\"\n", data);
		sleep(1.5);
		n++;
	}
	
	/* detach from the segment: */
	if (shmdt(data) == -1) {
		perror("shmdt");
		exit(1);
	}
	
	/* delete the IPC structure */
	shmctl(shmid, IPC_RMID, NULL);
	//printf("Closed Shared Memory Segment\n");
	
	return 0;
}