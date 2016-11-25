#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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


int main() {
   double** arr = create2DArray(30,10);
   for (int i = 0; i < 30; i++) {
      for (int j = 0; j < 10; j++) {
         arr[i][j] = i + j;
      }
   }
   double** slice = slice2DArrayRows(arr,10, 6, 10);
   slice[1][2] = 103;
   //arr = addslice2DArrayRows(arr, slice,10, 1,4);
   printf("arr[7][2] = %f\n", arr[7][2]);
}
