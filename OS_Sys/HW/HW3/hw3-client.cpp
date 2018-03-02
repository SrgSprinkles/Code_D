#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

const int SHM_SIZE = 1024; /* make it a 1K shared memory segment */
const char FILE_NAME[] = "shm-example.dat";

int main(int argc, char *argv[])
{

    /*
     * NOTES:
     * 
     * -Include a shared variable that passes between the client and
     *  server, (1 when the server is busy and 0 for whent the client 
     *  can edit the text?)
     */

    key_t key;
    int shmid;
    char *data;
    int n;
    
    /* make the key: */
    if ((key = ftok(FILE_NAME, 1)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    /* connect to (and possibly create) the segment: */
    
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }
    
    /* modify the segment, based on the command line: */
    strncpy(data, "AAAAAAAA", SHM_SIZE);
    sleep(1.5);
    strncpy(data, "BBBBBBBB", SHM_SIZE);
    sleep(1.5);
    strncpy(data, "STOP", SHM_SIZE);
    
    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    
    return 0;
}