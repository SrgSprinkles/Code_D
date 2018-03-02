#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>

int main()
{
  char* memoryMap;
  int fd_sharedFile = open("./sharedFile", O_RDWR);
  
  memoryMap = (char*)mmap(NULL, sizeof(char)*10, PROT_WRITE | PROT_READ, MAP_SHARED, fd_sharedFile, 0);

  for(int i; i < 20; i++)
  {
    printf("Output: %s\n", memoryMap);
    fflush(stdout);
    sleep(2);
  }

  fd_sharedFile.close();

  return 0;
}
