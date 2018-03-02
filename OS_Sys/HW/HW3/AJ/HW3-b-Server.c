#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int FILE_SIZE = sizeof(char)*1024;

int delayCount = 0;
char* shared_buffer;

int delay100ms = 0;

typedef union 
{
  int val;
  struct sem_id_ds *buf;
  ushort *array;
}
  semun;

void basic_delay()
{
  long i, j, k;

  for (i = 0; i < 200L; i++)
  {
    for (j = 0; j < 400L; j++)
    {
      k = k + i;
    }
  }
}

void delay(int delay_time)
{
  int i, j;

  for (i = 0; i < delay_time; i++)
  {
    for (j = 0; j < delay100ms; j++)
    {
      basic_delay();
    }
  }
}

void set(int sem_id){
  semun arg;
  arg.val = 1;

  if (semctl(sem_id, 0, SETVAL, arg) < 0 )
  {
    perror("Semctl Failed: ");
    exit(-1);
  }
}

int initial()
{
  key_t key = (shared_buffer, 1);
  int sem_id = semget(key, 1, IPC_CREAT | 0666);

  if(sem_id < 0)
  {
    perror("Semget Failed: ");
    exit(-1);
  }

  set(sem_id);
  return sem_id;
}

void lock(int sem_id)
{
  struct sembuf op1;
  op1.sem_num = 0;
  op1.sem_op = -1;
  op1.sem_flg = SEM_UNDO;

  if (semop(sem_id,&op1,1) == -1)
  {
    perror("Thread 1 - Semop Failure: ");
    exit(-1);
  }
}

void unlock(int sem_id)
{
  struct sembuf op1;
  op1.sem_num = 0;
  op1.sem_op = 1;
  op1.sem_flg = SEM_UNDO;

  if (semop(sem_id, &op1, 1) == -1)
  {
    perror("Thread 1 - Semop Failure: ");
    exit(-1);
  }
}

int stop_alarm = 0;
static void sig_alrm(int signo)
{
  stop_alarm = 1;
}

void calcuate_delay()
{
  int i;
  struct tms t1;
  struct tms t2;
  long clktck;
  double td;
  clock_t t;

  printf("Calculating delay scale...\n");
  stop_alarm = 0;

  if (signal(SIGALRM, sig_alrm) == SIG_ERR)
  {
      perror("Set Alarm");
  }
  
  alarm(5);
  times(&t1);
  
  while (stop_alarm == 0) {
    delayCount++;
    basic_delay();
  }

  times(&t2);
  alarm(0);
  t = t2.tms_utime - t1.tms_utime;

  if ((clktck = sysconf(_SC_CLK_TCK)) < 0 ){
    perror("Sysconf Error: ");
  }

  td = t / (double)clktck;
  delay100ms = delayCount / td / 10;

  if (delay100ms == 0)
  {
    delay100ms++;
  }

  printf("...Done\n");
}


// Main Function
int main()
{
  int i;
  int fd_shared2;
  int copyi;
  char InitData[FILE_SIZE*2];

  strcpy(InitData, "&&&&&&&&&&");

  for(copyi = 0; copyi < 100; copyi++)
  {
    strcat(InitData,"&&&&&&&&&&");
  }

  int sem_id = initial();

  fd_shared2 = open("shared2", O_RDWR | O_CREAT | O_TRUNC, 0600);

  if (fd_shared2 == NULL) {
     perror("Failed to open shared2.dat: ");
     exit(1);
  }

  write(fd_shared2, InitData, FILE_SIZE);
  shared_buffer = (char *) mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shared2, 0);

  if ( shared_buffer == (caddr_t) -1) {
     perror("Mmap Error: ");
     exit(2);
  }

  char lastRead[1000];
  int j, n;
  char* results = (char*)malloc(FILE_SIZE);
  int fd_shared22 = open("serverwrite", O_RDWR | O_CREAT | O_APPEND, 0600);

  while(lastRead[0] != 'S')
  {
    if(shared_buffer[FILE_SIZE-1] != '*')
    {
      delay(1000000000);
      printf("Nothing to read\n");
      continue;
    }

    results = (char*)malloc(FILE_SIZE);
    lock(sem_id);

    for (j = 0; j < FILE_SIZE; j++)
    {
       results[j] = shared_buffer[j];
       delay(4);

       if(shared_buffer[j + 1] == '&' && shared_buffer[j + 2] == '&')
       {
        break;
       }
    }
    
    lseek(fd_shared2,0,SEEK_SET);
    write(fd_shared2, InitData, FILE_SIZE);
    delay(100000000);

    unlock(sem_id);
    write(fd_shared22, results, strlen(results));
      
    printf("Reader gets results = %s\n", results);
    free(results);
  }
}