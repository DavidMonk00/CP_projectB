#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "backgroundfunctions.h"
#define NUM_THREADS 2

struct thread_data{
   double Rmax;
   int  thread_id;
   int  rows;
   int ny;
   double** V;
   double** boolarr;
};

struct thread_data thread_data_array[NUM_THREADS];

void* sorRowRed(void* threadarg) {
   struct thread_data* chunk = malloc(sizeof(struct thread_data));
   int taskid;
   int rows;
   double** V;
   double** boolarr;
   double Rmax;
   chunk = (struct thread_data *) threadarg;
   taskid = chunk->thread_id;
   rows = chunk->rows;
   V = chunk->V;
   Rmax = chunk->Rmax;
   boolarr = chunk->boolarr;
   V[1][2] = taskid;
   chunk->Rmax = taskid;
   pthread_exit((void*)chunk);
}

/*int main(int argc, char const *argv[]) {
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   int ny = 10000;
   int nx = 10000;
   int i; int j; long t;
   double** base_array = create2DArray(nx,ny);
   for (i = 0; i < nx; i++) {
      for (j = 0; j < ny; j++) {
         base_array[i][j] = i + j;
      }
   }
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (t = 0; t < NUM_THREADS; t++) {
      thread_data_array[t].thread_id = t;

      thread_data_array[t].ny = ny;
      if (t != NUM_THREADS-1){
         int rows = nx/NUM_THREADS;
         printf("Thread %ld, rows in chunk: %d\n", t+1, rows);
         thread_data_array[t].rows = rows;
         thread_data_array[t].V = create2DArray(rows,ny);
      } else {
         int rows = nx/NUM_THREADS + nx%NUM_THREADS;
         printf("Thread %ld, rows in chunk: %d\n", t+1, rows);
         thread_data_array[t].rows = rows;
         thread_data_array[t].V = create2DArray(rows,ny);
      }
      rc = pthread_create(&threads[t], &attr, sorRowRed, (void*)&thread_data_array[t]);
      if (rc) {
         printf("Error\n");
         exit(-1);
      }
   }
   for (t = 0; t < NUM_THREADS; t++) {
      struct thread_data* res;
      rc = pthread_join(threads[t], (void*)&res);
      double** array;
      double x;
      x = res->Rmax;
      array = res->V;
      //base_array[t] = array;
      printf("%f\n", x);
      printf("%f\n", array[1][2]);
   }
   pthread_exit(NULL);
}*/

/*LoopParams* getLoopParams(int** boolarr, int nx, int ny, int breaks) {
   int** loopstarts = create2DintArray(nx, breaks + 1);
   int** loopends = create2DintArray(nx, breaks + 1);
   for (int i = 0; i < nx; i++) {
      int b = 0;
      int breaks = 1;
      for (int j = 0; j < ny; j++) {
         if (boolarr[i][j] == 1 && b == 0) {
            b = 1;
            loopends[i][breaks- 1] = j;
         } else if (boolarr[i][j] == 0 && b == 1) {
            b = 0;
            loopstarts[i][breaks] = j;
            breaks++;
         }
      }
      loopends[i][breaks-1] = ny;
   }
   LoopParams* lp = malloc(nx*sizeof(LoopParams));
   for (int i = 0; i < nx; i++) {
      for (int j = 0; j < breaks + 1; j++) {
         if (!loopends[i][j]) {
            lp[i].loops = j-1;
         } else {
            lp[i].loops = breaks + 1;
         }
      }
      if (lp[i].loops == 0) {
         lp[i].loops++;
      }
   }
   for (int i = 0; i < nx; i++) {
      lp[i].starts = create1DintArray(lp[i].loops);
      lp[i].ends = create1DintArray(lp[i].loops);
      for (int j = 0; j < lp[i].loops; j++) {
         lp[i].starts[j] = loopstarts[i][j];
         lp[i].ends[j] = loopends[i][j];
      }
   }
   return lp;
}*/

int main() {
   /*int rows = 1000;
   int columns = 1000;
   int breaks = 1;
   int count = 0;
   int** arr = create2DintArray(rows,columns);
   for (int i = rows/3; i < 2*rows/3; i++) {
      for (int j = columns/3; j < 2*columns/3; j++) {
         arr[i][j] = 1;
         count++;
      }
   }
   printf("%d\n", 1000*1000-count);

   LoopParams* lp = getLoopParams(arr,rows,columns,breaks);
   int N = 0;
   for (int i = 0; i < rows; i++) {
      for (int l = 0; l < lp[i].loops; l++) {
         for (int j = lp[i].starts[l]; j < lp[i].ends[l]; j++) {
            N++;
            /*if (arr[i][j]){
               N++;
            }
         }
      }
   }
   printf("%d\n", N);*/
   double x = pow(2,4);
   printf("%f\n", x);
}
