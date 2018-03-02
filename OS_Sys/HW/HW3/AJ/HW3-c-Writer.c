#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>

int main(){
  char* memoryMap;
  int pid = getpid() % 10;
  int outputNumber = pid;
  int length = snprintf(NULL, 0, "%d", outputNumber);
  char* str = malloc(length + 1);
  int fd_sharedFile = open("./sharedFile", O_RDWR);
  snprintf(str, length + 1, "%d", outputNumber );

  memoryMap = (char *) mmap(NULL, sizeof(str)*10, PROT_WRITE|PROT_READ, MAP_SHARED, fd_sharedFile, 0);

  for(int i = 0; i < 20; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      *(memoryMap + j) = *str;
    }
    sleep(2);
  }

  fd_sharedFile.close();

  return 0;
}
