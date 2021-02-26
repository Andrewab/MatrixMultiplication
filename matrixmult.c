#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>


//prototypes


int main(int argc, char **argv) {
  if(argc <= 2) {
    printf("Not enough arguments given\n");
    exit(0);
  }
  int numThreads = atoi(argv[1]);
  int sizeOfMatrices = atoi(argv[2]);
  printf("%d\n", numThreads);
  exit(0);
  return 0;
}
