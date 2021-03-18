#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

struct thread_args {
  int size;
  double **MatrixA;
  double **MatrixB;
};
//prototypes
double** fill(int size);
double **MatrixMult(double **MatrixA, double **MatrixB, int size);
void threadedMatrixMult(int numThreads, int size, double **MatrixA, double **MatrixB);
void *crossProduct(void *_args);
double findSquaredDif(double **noThreadMatrix, int size);
//Global Variables
double **productMatrix;
int counter = 0;
pthread_mutex_t lock;

int main(int argc, char **argv) {
  if(argc <= 2){//checks if given correct num of args
    printf("Not enough arguments given\n");
    exit(0);
  }
  int numThreads = atoi(argv[1]);//number of requested threads
  int size = atoi(argv[2]);//size of matrices
  double **MatrixA = fill(size);//creates matixA with random nums
  double **MatrixB = fill(size);//Creates MatrixB with random Nums
  int fastestTime = INT_MAX;//initializes fastest time at largest val
  struct timeval start_time;
  struct timeval end_time;
  int t_overlap = 0;
  for(int i = 0; i < 5; i++) {//does matrix math on matrixA and MatrixB 5 times, takes fastest time and prints it
    gettimeofday(&start_time,NULL);
    productMatrix  = MatrixMult(MatrixA,MatrixB,size);
    gettimeofday(&end_time,NULL);
    if(end_time.tv_sec > start_time.tv_sec) {
      t_overlap = (end_time.tv_sec - start_time.tv_sec) * 1000000;
    }
    if(((end_time.tv_usec + t_overlap) - start_time.tv_usec) < fastestTime){
      fastestTime = (end_time.tv_usec + t_overlap) - start_time.tv_usec;
    }
    t_overlap = 0;
  }
  double **noThreadMatrix = productMatrix;
  printf("Fastest time without Multithreading: %d Microseconds\n", fastestTime);
  if(numThreads > 0) {// Only runs if requested num of threads is greater then 0
    int fastestTimeMultThread = INT_MAX;
    struct timeval start_time_multithread;
    struct timeval end_time_multithread;
    t_overlap = 0;
    /*
      Runs the non threaded approach to solving the matrix 5 times, then takes fastest time and stores that as fastestTimeMultThread
     */
    for(int j = 0; j < 5; j++) {
      gettimeofday(&start_time_multithread,NULL);
      threadedMatrixMult(numThreads, size, MatrixA, MatrixB);
      gettimeofday(&end_time_multithread,NULL);
      if(end_time_multithread.tv_sec > start_time_multithread.tv_sec) {
	t_overlap = (end_time_multithread.tv_sec - start_time_multithread.tv_sec) * 1000000;
      }
      if(((end_time_multithread.tv_usec + t_overlap) - start_time_multithread.tv_usec) < fastestTimeMultThread) {
	fastestTimeMultThread = (end_time_multithread.tv_usec + t_overlap) - start_time_multithread.tv_usec;
      }
      t_overlap = 0;
    } 
    int timeDif = fastestTime - fastestTimeMultThread;
    double squaredDif = findSquaredDif(noThreadMatrix, size);
    printf("Multiplying random matrices of size: %dx%d\n", size,size);
    printf("Fastest time %d threads: %d\n", numThreads, fastestTimeMultThread);
    double speedUpFactor = fastestTime / fastestTimeMultThread;
    printf("SpeedUp Factor is : %f\n", speedUpFactor);
    printf("Observed error: %f\n", squaredDif);
  }
  return 0;
}
/*---------------------------------------------------
findSquaredDif() finds the sum of the error difference
between the two solved matrices: the nonMultithreaded 
matrix and the Multithreaded Matrix. It will then return
that value.

Returns Double on Completion
---------------------------------------------------- */
double findSquaredDif(double **noThreadMatrix, int size){
  double error;
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      error += (noThreadMatrix[i][j] - productMatrix[i][j]);
    }
  }
  return error;
}
/*--------------------------------------------------
fill() allocates space for a 2DArray, then fill that
2DArray with doubles that range from values 0-99(inclusive)

Returns 2DArray on Completion
 ---------------------------------------------------*/
double **fill(int size) {//fills requested matrix with pseudorandom nums
  srand(time(0));
  double **Matrix;
  Matrix = malloc(sizeof(double*)*size);
  for(int i = 0; i < size; i++) {
    Matrix[i] = malloc(sizeof(double*)*size);
  }
  for(int row = 0; row < size; row++) {
    for(int col = 0; col < size; col++) {
      Matrix[row][col] = (double)(rand() % 100);//puts random num into matrix location
    }
  }
return Matrix;
}
/*----------------------------------------------------
MatrixMult() will create a 2DArray of doubles, then fill
that 2DArray with values based upon the crossProducts of
two given 2DArrays. It will then return the product 2DArray.
THis approach uses a linear approach.
Returns 2DArray on Completion
-----------------------------------------------------*/
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
/*---------------------------------------------------
threadedMatrixMult() will modify the global 2DArray
named productMatrix. It will multiply the values from
two given matrices and place the crossProduct values 
into productMatrix.This approach uses a multithreaded
approach.

NO return Value
----------------------------------------------------*/
void threadedMatrixMult(int numThreads, int size, double **MatrixA, double **MatrixB) {
  int totalThreads;
  if(numThreads > (size*size) ) {
    totalThreads = size;
  }
  else {
    totalThreads = numThreads;
  }
  pthread_t thid[totalThreads];
  struct thread_args *args = malloc(sizeof(struct thread_args));
  args->size = size;
  args->MatrixA = MatrixA;
  args->MatrixB = MatrixB;
  pthread_mutex_init(&lock,NULL);
  for(int i = 0; i < totalThreads; i++) {
    if(pthread_create(&thid[i],NULL,crossProduct,(void*)args) != 0){
      printf("pthread_create() error \n");
    }
  }
  for(int i = 0; i < totalThreads; i++) {
    if(pthread_join(thid[i], NULL) != 0) {
      perror("pthread_join error");
    }
  }
}
/*-------------------------------------------------------
crossProduct() is the function ran by individual execution
Threads. It will modify the global productMatrix and place in
the crossProduct value. 

Returns NULL on Completion
 -------------------------------------------------------*/
void *crossProduct(void *_args) {
  struct thread_args *args = (struct thread_args*)_args;
  int size = args->size;
  int col;
  int row;
  double **MatrixA = args->MatrixA;
  double **MatrixB = args->MatrixB;
  double crossProductSum = 0;
  pthread_mutex_lock(&lock);
  while(counter < size*size) {
    col = counter % size;
    row = counter / size;
    for(int i = 0; i < size; i++) {
      crossProductSum += MatrixA[row][i] * MatrixB[i][col];//sums cross product of matrix
    }
    productMatrix[row][col] = crossProductSum;// places crossprodcut into correct location in the matrix
    counter++;
    pthread_mutex_unlock(&lock);
    pthread_mutex_lock(&lock);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}
