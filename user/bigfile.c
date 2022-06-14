#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

int
main()
{
  char buf[BSIZE];
  int fd, i, blocks;

  fd = open("big.file", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("bigfile: cannot open big.file for writing\n");
    exit(-1);
  }

  blocks = 0;
  while(1){
    *(int*)buf = blocks;
    int cc = write(fd, buf, sizeof(buf));
    if(cc <= 0)
      break;
    blocks++;
    if (blocks == 12){
        printf("Finished writing 12KB (direct)\n");
    }
    if (blocks == 12 + 256){
        printf("Finished writing 268KB (single direct)\n");
    }
    if (blocks == 12 + 256 + 256 * 256){
        printf("Finished writing 10MB\n");
    }
    if (blocks % 1000 == 0)
      printf("Finished another 1000 blocks\n");
  }

  printf("\nwrote %d blocks\n", blocks);
  if(blocks != 65804) {
    printf("bigfile: file is too small\n");
    exit(-1);
  }
  
  close(fd);
  fd = open("big.file", O_RDONLY);
  if(fd < 0){
    printf("bigfile: cannot re-open big.file for reading\n");
    exit(-1);
  }
  for(i = 0; i < blocks; i++){
    int cc = read(fd, buf, sizeof(buf));
    if(cc <= 0){
      printf("bigfile: read error at block %d\n", i);
      exit(-1);
    }
    if(*(int*)buf != i){
      printf("bigfile: read the wrong data (%d) for block %d\n",
             *(int*)buf, i);
      exit(-1);
    }
  }

  printf("bigfile done; ok\n"); 

  exit(0);
}