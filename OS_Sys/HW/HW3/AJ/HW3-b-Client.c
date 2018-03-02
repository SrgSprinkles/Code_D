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

void set(int sem_id)
{
  semun arg;
  arg.val = 1;

  if (semctl(sem_id, 0, SETVAL, arg) < 0)
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

  if (semop(sem_id, &op1, 1) == -1)
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

  while (stop_alarm == 0)
  {
    delayCount++;
    basic_delay();
  }

  times(&t2);
  alarm(0);
  t = t2.tms_utime - t1.tms_utime;

  if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
  {
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
  int fd;
  int j,n;
  int size = 0;
  int written = 0;
  int sem_id = initial();
  char strvar[1001];

  fd = open("shared2", O_RDWR);
  
  if (fd < 0) 
  {
     perror("Failed to open file: ");
     exit(1);
  }

  shared_buffer = (char *) mmap(NULL, FILE_SIZE - 10, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);

  if (shared_buffer == (caddr_t) -1)
  {
     perror("Mmap Error: ");
     exit(2);
  }

  while(strvar[0] != 'S')
  {
    printf("Input message to send to the server: ");
    fgets(strvar, 1000, stdin);

    if ((strlen(strvar) > 0) && (strvar[strlen(strvar) - 1] == '\n'))
    {
      strvar[strlen(strvar) - 1] = '\0';
    }

    for(size = 0; size < 1000; size++)
    {
      if(strvar[size] == '\0')
      {
        break;
      }
    }

    printf("Input Characters: (%s). Characters Written: (%d)", strvar, size);
    lock(sem_id);

    for (n = 0; n < size; n++)
    {
      *(shared_buffer + n) = *(strvar + n);
      delay(3);
    }

    *(shared_buffer + FILE_SIZE - 1) = '*';
    unlock(sem_id);

    printf("Writer finished...\n");
  }
}