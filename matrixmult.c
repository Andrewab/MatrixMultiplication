#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

struct thread_args {
  int size;
  double **MatrixA;
  double **MatrixB;
};
//prototypes
double** fill(int size);
double **MatrixMult(double **MatrixA, double **MatrixB, int size);
double **threadedMatrixMult(int numThreads, int size, double **MatrixA, double **MatrixB);
double crossProduct(int row, int col, int size, double MatrixA, double MatrixB);
pthread_t 
//Global Variables
double **productMatrix;
int counter = 0;
pthread_mutex_t lock;

int main(int argc, char **argv) {
  if(argc <= 2){
    printf("Not enough arguments given\n");
    exit(0);
  }
  int numThreads = atoi(argv[1]);
  int size = atoi(argv[2]);
  double **MatrixA = fill(size);
  double **MatrixB = fill(size);
  long int fastestTime = 2147483647;
  struct timeval start_time;
  struct timeval end_time;
  for(int i = 0; i < 5; i++) {
    gettimeofday(&start_time,NULL);
    productMatrix = MatrixMult(MatrixA,MatrixB,size);
    gettimeofday(&end_time,NULL);
    if((end_time.tv_usec - start_time.tv_usec) < fastestTime)
      fastestTime = end_time.tv_usec - start_time.tv_usec;
  }
  printf("Fastes time without Multithreading: %ld Microseconds\n", fastestTime);
  return 0;
}
double **fill(int size) {
  srand(time(0));
  double **Matrix;
  Matrix = malloc(sizeof(double*)*size);
  for(int i = 0; i < size; i++) {
    Matrix[i] = malloc(sizeof(double*)*size);
  }
  for(int row = 0; row < size; row++) {
    for(int col = 0; col < size; col++) {
      Matrix[row][col] = (double)(rand() % 100);
    }
  }
return Matrix;
}
double **MatrixMult(double **MatrixA, double **MatrixB, int size) {
  double sum = 0;
  double **MatrixC = malloc(sizeof(double*)*size);
  for(int j = 0; j < size; j++) {
    MatrixC[j] = malloc(sizeof(double*)*size);
  }
  for(int row = 0; row < size; row++) {
    for(int col = 0; col < size; col++) {
      for(int i = 0; i < size; i++) {
	sum += MatrixA[row][i] * MatrixB[i][col];
      }
      MatrixC[row][col] = sum;
      sum = 0;
    }
  }
  return MatrixC;
}

double **threadedMatrixMult(int numThreads, int size, double **MatrixA, double **MatrixB) {
  double **MatrixC;
  MatrixC = malloc(sizeof(double*)*size);
  for(int i = 0; i < size, i++) {
    MatrixC[i] = malloc(sizeof(double*)*size);
  }
  int totalThreads;
  int *threads;
  if(numThreads > (size*size) ) {
    totalThreads = size;
  }
  else {
    totalThreads = numThreads;
  }
  struct thread_args *args = malloc(sizeof(struct thread_args));
  args->size = size;
  args->MatrixA = MatrixA;
  args->MatrixB = MatrixB;
  for(int j = 0; j < totalThreads; j++) {
    pthread_create
  }
}

void crossProduct(void *_args) {
  struct thread_args *args = (struct thread_args*)_args;
  
  if(counter <= size*size) {
    pthread_mutex_lock(&lock);
    int size = args->size;
    int col = counter % size;
    int row = counter / size;
    double **MatrixA = args->MatrixA;
    double **MatrixB = args->MatrixB;
    double crossProduct = 0;
    for(int i = 0; i < size; i++) {
      crossProduct += MatrixA[row][i] * MatrixB[i][col];
    }
    productMatrix[row][col] = crossProduct;
    counter++;
    pthread_mutex_unlock(&lock);
    crossProduct(args);
    free(args);
    
  }
}


